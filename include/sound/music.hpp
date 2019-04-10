

#ifndef MIXER_HPP
#define MIXER_HPP

#include <3ds.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include "io.hpp"

bool SDLH_Init(void);
void SDLH_Exit(void);

void SDLH_Play(void);

#endif