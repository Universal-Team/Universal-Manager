/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

// Errors
#define LOADBMP_NO_ERROR 0
#define LOADBMP_OUT_OF_MEMORY 1
#define LOADBMP_FILE_NOT_FOUND 2
#define LOADBMP_FILE_OPERATION 3
#define LOADBMP_INVALID_FILE_FORMAT 4
#define LOADBMP_INVALID_SIGNATURE 5
#define LOADBMP_INVALID_BITS_PER_PIXEL 6

# define LOADBMP_API

LOADBMP_API unsigned int loadbmp_decode_file(
	const char *filename, unsigned char **imageData, unsigned int *width, unsigned int *height);

// Disable Microsoft Visual C++ compiler security warnings for fopen, strcpy, etc being unsafe
#if defined(_MSC_VER) && (_MSC_VER >= 1310)
#	pragma warning(disable: 4996)
#endif

#include <stdio.h> /* fopen(), fwrite(), fread(), fclose() */
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memset(), memcpy() */

LOADBMP_API unsigned int loadbmp_decode_file(
	const char *filename, unsigned char **imageData, unsigned int *width, unsigned int *height)
{
	FILE *f = fopen(filename, "rb");

	if (!f)
		return LOADBMP_FILE_NOT_FOUND;

	unsigned char bmp_file_header[14];
	unsigned char bmp_info_header[40];
	unsigned char bmp_pad[3];

	unsigned int w, h;
	unsigned char *data = NULL;

	unsigned int x, y, i, padding;

	memset(bmp_file_header, 0, sizeof(bmp_file_header));
	memset(bmp_info_header, 0, sizeof(bmp_info_header));

	if (fread(bmp_file_header, sizeof(bmp_file_header), 1, f) == 0)
	{
		fclose(f);
		return LOADBMP_INVALID_FILE_FORMAT;
	}

	if (fread(bmp_info_header, sizeof(bmp_info_header), 1, f) == 0)
	{
		fclose(f);
		return LOADBMP_INVALID_FILE_FORMAT;
	}

	if ((bmp_file_header[0] != 'B') || (bmp_file_header[1] != 'M'))
	{
		fclose(f);
		return LOADBMP_INVALID_SIGNATURE;
	}

	if ((bmp_info_header[14] != 24) && (bmp_info_header[14] != 32))
	{
		fclose(f);
		return LOADBMP_INVALID_BITS_PER_PIXEL;
	}

	w = (bmp_info_header[4] + (bmp_info_header[5] << 8) + (bmp_info_header[6] << 16) + (bmp_info_header[7] << 24));
	h = (bmp_info_header[8] + (bmp_info_header[9] << 8) + (bmp_info_header[10] << 16) + (bmp_info_header[11] << 24));
	
	if ((w > 0) && (h > 0))
	{
		data = (unsigned char*)malloc(w * h * 4);

		if (!data)
		{
			fclose(f);
			return LOADBMP_OUT_OF_MEMORY;
		}

		for (y = (h - 1); y != -1; y--)
		{
			for (x = 0; x < w; x++)
			{
				i = (x + y * w) * 4;

				if (fread(data + i, 3, 1, f) == 0)
				{
					free(data);
					fclose(f);
					return LOADBMP_INVALID_FILE_FORMAT;
				}

				data[i + 3] = data[i + 2];
				data[i + 2] = data[i + 1];
				data[i + 1] = data[i];
				data[i] = 255;
			}

			padding = ((4 - (w * 3) % 4) % 4);

			if (fread(bmp_pad, 1, padding, f) != padding)
			{
				free(data);
				fclose(f);
				return LOADBMP_INVALID_FILE_FORMAT;
			}
		}
	}

	(*width) = w;
	(*height) = h;
	(*imageData) = data;

	fclose(f);
	return LOADBMP_NO_ERROR;
}