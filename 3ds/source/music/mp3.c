#include "music/mp3.h"
#include "music/playback.h"

#include <mpg123.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t*			buffSize;
static mpg123_handle	*mh = NULL;
static uint32_t			rate;
static uint8_t			channels;

static u64 frames_read = 0, total_samples = 0;


// For MP3 ID3 tags
// Helper for v1 printing, get these strings their zero byte.
static void safe_print(char *tag, char *name, char *data, size_t size) {
	char safe[31];
	if (size > 30)
		return;
	memcpy(safe, data, size);
	safe[size] = 0;
	snprintf(tag, 34, "%s: %s\n", name, safe);
}


// For MP3 ID3 tags
// Print out ID3v1 info.
static void print_v1(Audio_Metadata *ID3tag, mpg123_id3v1 *v1) {
	safe_print(ID3tag->title, "",   v1->title,   sizeof(v1->title));
	safe_print(ID3tag->artist, "",  v1->artist,  sizeof(v1->artist));
	safe_print(ID3tag->album, "",   v1->album,   sizeof(v1->album));
	safe_print(ID3tag->year, "",    v1->year,    sizeof(v1->year));
	//safe_print(ID3tag->comment, "", v1->comment, sizeof(v1->comment));
	//safe_print(ID3tag->genre, "", genre_list[v1->genre], sizeof(genre_list[v1->genre]));
}

// For MP3 ID3 tags
// Split up a number of lines separated by \n, \r, both or just zero byte
// and print out each line with specified prefix.
static void print_lines(char *data, const char *prefix, mpg123_string *inlines) {
	size_t i;
	int hadcr = 0, hadlf = 0;
	char *lines = NULL;
	char *line  = NULL;
	size_t len = 0;

	if (inlines != NULL && inlines->fill) {
		lines = inlines->p;
		len   = inlines->fill;
	}
	else
		return;

	line = lines;
	for (i = 0; i < len; ++i) {
		if (lines[i] == '\n' || lines[i] == '\r' || lines[i] == 0) {
			char save = lines[i]; /* saving, changing, restoring a byte in the data */
			if (save == '\n')
				++hadlf;
			if (save == '\r')
				++hadcr;
			if ((hadcr || hadlf) && (hadlf % 2 == 0) && (hadcr % 2 == 0))
				line = "";

			if (line) {
				lines[i] = 0;
				if (data == NULL)
					printf("%s%s\n", prefix, line);
				else
					snprintf(data, 0x1F, "%s%s\n", prefix, line);
				line = NULL;
				lines[i] = save;
			}
		}
		else {
			hadlf = hadcr = 0;
			if (line == NULL)
				line = lines + i;
		}
	}
}

// For MP3 ID3 tags
// Print out the named ID3v2  fields.
static void print_v2(Audio_Metadata *ID3tag, mpg123_id3v2 *v2) {
	print_lines(ID3tag->title, "", v2->title);
	print_lines(ID3tag->artist, "", v2->artist);
	print_lines(ID3tag->album, "", v2->album);
	print_lines(ID3tag->year, "",    v2->year);
	//print_lines(ID3tag->comment, "", v2->comment);
	//print_lines(ID3tag->genre, "",   v2->genre);
}

/**
 * Set decoder parameters for MP3.
 *
 * \param	decoder Structure to store parameters.
 */
void setMp3(struct decoder_fn* decoder)
{
	decoder->init = &initMp3;
	decoder->rate = &rateMp3;
	decoder->channels = &channelMp3;
	/*
	 * buffSize changes depending on input file. So we set buffSize later when
	 * decoder is initialised.
	 */
	buffSize = &(decoder->buffSize);
	decoder->decode = &decodeMp3;
	decoder->exit = &exitMp3;
}

/**
 * Initialise MP3 decoder.
 *
 * \param	file	Location of MP3 file to play.
 * \return			0 on success, else failure.
 */
int initMp3(const char* file)
{
	int err = 0;
	int encoding = 0;

	if((err = mpg123_init()) != MPG123_OK)
		return err;

	if((mh = mpg123_new(NULL, &err)) == NULL)
	{
		printf("Error: %s\n", mpg123_plain_strerror(err));
		return err;
	}

	if(mpg123_open(mh, file) != MPG123_OK ||
			mpg123_getformat(mh, (long *) &rate, (int *) &channels, &encoding) != MPG123_OK)
	{
		printf("Trouble with mpg123: %s\n", mpg123_strerror(mh));
		return -1;
	}

	static const Audio_Metadata empty;
	metadata = empty;
	mpg123_id3v1 *v1;
	mpg123_id3v2 *v2;

	mpg123_seek(mh, 0, SEEK_SET);
	metadata.has_meta = mpg123_meta_check(mh);
	if (metadata.has_meta & MPG123_ID3 && mpg123_id3(mh, &v1, &v2) == MPG123_OK) {
		if (v1 != NULL)
			print_v1(&metadata, v1);
		if (v2 != NULL) {
			print_v2(&metadata, v2);
		}
	}

	/*
	 * Ensure that this output format will not change (it might, when we allow
	 * it).
	 */
	frames_read = 0;
	total_samples = mpg123_length(mh);
	mpg123_format_none(mh);
	mpg123_format(mh, rate, channels, encoding);

	/*
	 * Buffer could be almost any size here, mpg123_outblock() is just some
	 * recommendation. The size should be a multiple of the PCM frame size.
	 */
	*buffSize = mpg123_outblock(mh) * 16;

	return 0;
}

/**
 * Get sampling rate of MP3 file.
 *
 * \return	Sampling rate.
 */
uint32_t rateMp3(void)
{
	return rate;
}

/**
 * Get number of channels of MP3 file.
 *
 * \return	Number of channels for opened file.
 */
uint8_t channelMp3(void)
{
	return channels;
}

/**
 * Decode part of open MP3 file.
 *
 * \param buffer	Decoded output.
 * \return			Samples read for each channel.
 */
uint64_t decodeMp3(void* buffer)
{
	size_t done = 0;
	mpg123_read(mh, buffer, *buffSize, &done);
	frames_read += done/(sizeof(s16) * 2);
	return done / (sizeof(int16_t));
}

u64 MP3_GetPosition(void) {
	return frames_read;
}

u64 MP3_GetLength(void) {
	return total_samples;
}

/**
 * Free MP3 decoder.
 */
void exitMp3(void)
{
	mpg123_close(mh);
	mpg123_delete(mh);
	mpg123_exit();
}