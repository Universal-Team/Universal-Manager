#include "music/all.h"
#include "music/error.h"
#include "music/file.h"
#include "music/flac.h"
#include "music/mp3.h"
#include "music/opus.h"
#include "music/playback.h"
#include "music/vorbis.h"
#include "music/wav.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static volatile bool stop = true;
static enum file_types file_type = FILE_TYPE_ERROR;
Audio_Metadata metadata;

/**
 * Pause or play current file.
 *
 * \return	True if paused.
 */
bool togglePlayback(void)
{
	bool paused = ndspChnIsPaused(CHANNEL);
	ndspChnSetPaused(CHANNEL, !paused);
	return !paused;
}

/**
 * Stops current playback. Playback thread should exit as a result.
 */
void stopPlayback(void)
{
	stop = true;
}

/**
 * Returns whether music is playing or paused.
 */
bool isPlaying(void)
{
	return !stop;
}

/**
 * Returns whether music is playing or paused.
 *
 * \return	True if paused.
 */
bool isPaused(void)
{
	return ndspChnIsPaused(CHANNEL);
}

/**
 * Should only be called from a new thread only, and have only one playback
 * thread at time. This function has not been written for more than one
 * playback thread in mind.
 *
 * \param	infoIn	Playback information.
 */
void playFile(void* infoIn)
{
	struct decoder_fn decoder;
	struct playbackInfo_t* info = infoIn;
	int16_t*		buffer1 = NULL;
	int16_t*		buffer2 = NULL;
	ndspWaveBuf		waveBuf[2];
	bool			lastbuf = false;
	int				ret = -1;
	const char*		file = info->file;
	bool			isNdspInit = false;

	static const Audio_Metadata empty;
	metadata = empty;

	/* Reset previous stop command */
	stop = false;

	file_type = getFileType(file);

	switch(file_type)
	{
		case FILE_TYPE_WAV:
			setWav(&decoder);
			break;

		case FILE_TYPE_FLAC:
			// setFlac(&decoder);
			break;

		case FILE_TYPE_OPUS:
			// setOpus(&decoder);
			break;

		case FILE_TYPE_MP3:
			setMp3(&decoder);
			break;

		case FILE_TYPE_VORBIS:
			setVorbis(&decoder);
			break;

		default:
			goto err;
	}

	if(ndspInit() < 0)
	{
		errno = NDSP_INIT_FAIL;
		goto err;
	}

	isNdspInit = true;

	if((ret = (*decoder.init)(file)) != 0)
	{
		errno = DECODER_INIT_FAIL;
		goto err;
	}

	if((*decoder.channels)() > 2 || (*decoder.channels)() < 1)
	{
		errno = UNSUPPORTED_CHANNELS;
		goto err;
	}

	buffer1 = linearAlloc(decoder.buffSize * sizeof(int16_t));
	buffer2 = linearAlloc(decoder.buffSize * sizeof(int16_t));

	ndspChnReset(CHANNEL);
	ndspChnWaveBufClear(CHANNEL);
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	ndspChnSetInterp(CHANNEL, NDSP_INTERP_POLYPHASE);
	ndspChnSetRate(CHANNEL, (*decoder.rate)());
	ndspChnSetFormat(CHANNEL,
			(*decoder.channels)() == 2 ? NDSP_FORMAT_STEREO_PCM16 :
			NDSP_FORMAT_MONO_PCM16);

	memset(waveBuf, 0, sizeof(waveBuf));
	waveBuf[0].nsamples = (*decoder.decode)(&buffer1[0]) / (*decoder.channels)();
	waveBuf[0].data_vaddr = &buffer1[0];
	ndspChnWaveBufAdd(CHANNEL, &waveBuf[0]);

	waveBuf[1].nsamples = (*decoder.decode)(&buffer2[0]) / (*decoder.channels)();
	waveBuf[1].data_vaddr = &buffer2[0];
	ndspChnWaveBufAdd(CHANNEL, &waveBuf[1]);

	/**
	 * There may be a chance that the music has not started by the time we get
	 * to the while loop. So we ensure that music has started here.
	 */
	while(ndspChnIsPlaying(CHANNEL) == false);

	while(stop == false)
	{
		svcSleepThread(100 * 1000);

		/* When the last buffer has finished playing, break. */
		if(lastbuf == true && waveBuf[0].status == NDSP_WBUF_DONE &&
				waveBuf[1].status == NDSP_WBUF_DONE)
			break;

		if(ndspChnIsPaused(CHANNEL) == true || lastbuf == true)
			continue;

		if(waveBuf[0].status == NDSP_WBUF_DONE)
		{
			size_t read = (*decoder.decode)(&buffer1[0]);

			if(read <= 0)
			{
				lastbuf = true;
				continue;
			}
			else if(read < decoder.buffSize)
				waveBuf[0].nsamples = read / (*decoder.channels)();

			ndspChnWaveBufAdd(CHANNEL, &waveBuf[0]);
		}

		if(waveBuf[1].status == NDSP_WBUF_DONE)
		{
			size_t read = (*decoder.decode)(&buffer2[0]);

			if(read <= 0)
			{
				lastbuf = true;
				continue;
			}
			else if(read < decoder.buffSize)
				waveBuf[1].nsamples = read / (*decoder.channels)();

			ndspChnWaveBufAdd(CHANNEL, &waveBuf[1]);
		}

		DSP_FlushDataCache(buffer1, decoder.buffSize * sizeof(int16_t));
		DSP_FlushDataCache(buffer2, decoder.buffSize * sizeof(int16_t));
	}
	stop = true;
	(*decoder.exit)();
out:
	if(isNdspInit == true)
	{
		ndspChnWaveBufClear(CHANNEL);
		ndspExit();
	}

	//delete(info->file); // This makes the Crash.
	linearFree(buffer1);
	linearFree(buffer2);

	/* Signal Watchdog thread that we've stopped playing */
	//*info->errInfo->error = -1; // This Too.
	//svcSignalEvent(*info->errInfo->failEvent); // This too.

	threadExit(0);
	return;

err:
	*info->errInfo->error = errno;
	svcSignalEvent(*info->errInfo->failEvent);
	goto out;
}

/**
 * Stop the currently playing file (if there is one) and play another file.
 *
 * \param	ep_file			File to play.
 * \param	playbackInfo	Information that the playback thread requires to
 *							play file.
 */
int changeFile(const char* ep_file, struct playbackInfo_t* playbackInfo)
{
	s32 prio;
	static Thread thread = NULL;

	if(ep_file != NULL && getFileType(ep_file) == FILE_TYPE_ERROR)
	{
		*playbackInfo->errInfo->error = errno;
		svcSignalEvent(*playbackInfo->errInfo->failEvent);
		return -1;
	}

	/**
	 * If music is playing, stop it. Only one playback thread should be playing
	 * at any time.
	 */
	if(thread != NULL)
	{
		/* Tell the thread to stop playback before we join it */
		stopPlayback();

		threadJoin(thread, U64_MAX);
		threadFree(thread);
		thread = NULL;
	}

	if(ep_file == NULL || playbackInfo == NULL)
		return 0;

	playbackInfo->file = strdup(ep_file);
	printf("Playing: %s\n", playbackInfo->file);

	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	thread = threadCreate(playFile, playbackInfo, 32 * 1024, prio - 1, -2, false);

	return 0;
}

u64 Audio_GetPosition(void) {
	u64 position = -1;

	switch(file_type) {
		case FILE_TYPE_FLAC:
			// position = FLAC_GetPosition();
			break;

		case FILE_TYPE_MP3:
			position = MP3_GetPosition();
			break;

		// case FILE_TYPE_OGG:
		// 	position = OGG_GetPosition();
		// 	break;

		case FILE_TYPE_OPUS:
			// position = OPUS_GetPosition();
			break;

		case FILE_TYPE_WAV:
			// position = WAV_GetPosition();
			break;

		default:
			break;
	}

	return position;
}

u64 Audio_GetLength(void) {
	u64 length = 0;

	switch(file_type) {
		case FILE_TYPE_FLAC:
			// length = FLAC_GetLength();
			break;

		case FILE_TYPE_MP3:
			length = MP3_GetLength();
			break;

		// case FILE_TYPE_OGG:
		// 	length = OGG_GetLength();
		// 	break;

		case FILE_TYPE_OPUS:
			// length = OPUS_GetLength();
			break;

		case FILE_TYPE_WAV:
			// length = WAV_GetLength();
			break;

		default:
			break;
	}

	return length;
}

int Audio_GetRate(void) {
	u64 rate = 0;

	switch(file_type) {
		case FILE_TYPE_FLAC:
			// rate = rateFlac();
			break;

		case FILE_TYPE_MP3:
			rate = rateMp3();
			break;

		// case FILE_TYPE_OGG:
			// rate = rateVorbis();
		// 	break;

		case FILE_TYPE_OPUS:
			// rate = rateOpus();
			break;

		case FILE_TYPE_WAV:
			rate = rateWav();
			break;

		default:
			break;
	}

	return rate;
}