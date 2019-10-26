#include <3ds.h>
#include <stdbool.h>

#ifndef PLAYBACK_H
#define PLAYBACK_H

/* Channel to play music on */
#define CHANNEL	0x09

typedef struct {
	bool has_meta;
	char title[31];
	char album[31];
	char artist[31];
	char year[5];
} Audio_Metadata;

extern Audio_Metadata metadata;

struct decoder_fn
{
	int (* init)(const char* file);
	uint32_t (* rate)(void);
	uint8_t (* channels)(void);
	size_t buffSize;
	uint64_t (* decode)(void*);
	void (* exit)(void);
};

struct playbackInfo_t
{
	char*				file;
	struct errInfo_t*	errInfo;
};

/**
 * Pause or play current file.
 *
 * \return	True if paused.
 */
bool togglePlayback(void);

/**
 * Stops current playback. Playback thread should exit as a result.
 */
void stopPlayback(void);

/**
 * Returns whether music is playing or paused.
 */
bool isPlaying(void);

/**
 * Returns whether music is playing or paused.
 *
 * \return	True if paused.
 */
bool isPaused(void);

/**
 * Should only be called from a new thread only, and have only one playback
 * thread at time. This function has not been written for more than one
 * playback thread in mind.
 *
 * \param	infoIn	Playback information.
 */
void playFile(void* infoIn);

/**
 * Stop the currently playing file (if there is one) and play another file.
 *
 * \param	ep_file			File to play.
 * \param	playbackInfo	Information that the playback thread requires to
 *							play file.
 */
int changeFile(const char* ep_file, struct playbackInfo_t* playbackInfo);

u64 Audio_GetLength(void);

u64 Audio_GetPosition(void);

int Audio_GetRate(void);

#endif