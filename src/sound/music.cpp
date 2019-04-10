/* This File is part of Universal-Upater!
* Diese Datei ist Teil von Universal-Updater!
 */

#include "music.hpp"
#include <vector>
#include "STDirectory.hpp"
#include <algorithm>
#include "random.hpp"

static Mix_Music* song;
static std::vector<std::string> songs;
static bool musicMutex = false;
static bool donePlaying = false;
static size_t currentSong = 0;
static u8 currentVolume = 0;

bool SDLH_Init(void)
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return false;
    }

    const int mix_flags = MIX_INIT_MP3;
    if ((Mix_Init(mix_flags) & mix_flags) != mix_flags)
    {
        fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    if (io::exists("/Universal-Updater/Music/"))
    {
        STDirectory d("/Universal-Updater/Music/");
        if (d.good() && d.count() > 0)
        {
            for (size_t i = 0; i < d.count(); i++)
            {
                if (!d.folder(i))
                {
                    std::string songName = d.item(i);
                    if (songName.substr(songName.size() - 3, 3) == "mp3")
                    {
                        songs.push_back("/Universal-Updater/Music/" + songName);
                    }
                }
            }
        }
    }

    std::sort(songs.begin(), songs.end());

    HIDUSER_GetSoundVolume(&currentVolume);
    
    return true;
}

void SDLH_Exit(void)
{
    if (musicMutex)
    {
        musicMutex = false;
    }
    else
    {
        donePlaying = true;
    }
    while (!donePlaying) svcSleepThread(125000000); // wait for SDLH_Play to be done
    if (song)
    {
        Mix_FreeMusic(song);
    }
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void SDLH_Play(void)
{
    musicMutex = !songs.empty();
    while (musicMutex)
    {
        HIDUSER_GetSoundVolume(&currentVolume);
        if (!Mix_PlayingMusic() || (currentVolume == 0 && songs.size() > 1))
        {
            if (song)
            {
                Mix_FreeMusic(song);
                song = nullptr;
            }
            {
                currentSong = randomNumbers() % songs.size();
            }
            song = Mix_LoadMUS(songs[currentSong].c_str());
            Mix_PlayMusic(song, 1);
        }
        if (currentVolume == 0)
        {
            Mix_PauseMusic();
        }
        while (currentVolume == 0 && musicMutex)
        {
            HIDUSER_GetSoundVolume(&currentVolume);
            svcSleepThread(250000000);
        }
        if (Mix_PausedMusic() && musicMutex)
        {
            Mix_ResumeMusic();
        }
        svcSleepThread(250000000);
    }
    donePlaying = true;
}