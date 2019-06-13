/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#pragma once

#ifdef _3DS
#include <3ds.h>
#define ESC(x) "\x1b[" #x
#define RESET   ESC(0m)
#define BLACK   ESC(30m)
#define RED     ESC(31;1m)
#define GREEN   ESC(32;1m)
#define YELLOW  ESC(33;1m)
#define BLUE    ESC(34;1m)
#define MAGENTA ESC(35;1m)
#define CYAN    ESC(36;1m)
#define WHITE   ESC(37;1m)
#else
#define ESC(x)
#define RESET
#define BLACK
#define RED
#define GREEN
#define YELLOW
#define BLUE
#define MAGENTA
#define CYAN
#define WHITE
#endif

void console_init(void);

__attribute__((format(printf,1,2)))
void console_set_status(const char *fmt, ...);

__attribute__((format(printf,1,2)))
void console_print(const char *fmt, ...);

__attribute__((format(printf,1,2)))
void debug_print(const char *fmt, ...);

void console_render(void);
