

#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <3ds.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include "io.hpp"

bool SDLH_Init_Chill(void);
bool SDLH_Init_Settings(void);
bool SDLH_Init_SD(void);
void SDLH_Exit(void);

void Music_SD(void);
void Music_Chill(void);
void Music_Settings(void);

#endif