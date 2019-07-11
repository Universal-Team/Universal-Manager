#include "graphics.h"
#include "lodepng.h"

#include <fstream>

std::vector<u16> font;

void initGraphics(void) {
    // Initialize video mode
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	// initialize all the VRAM banks
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);
	vramSetBankE(VRAM_E_TEX_PALETTE);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT4);
	vramSetBankG(VRAM_G_TEX_PALETTE_SLOT5); // 16Kb of palette ram, and font textures take up 8*16 bytes.
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	// Init oam with 1D mapping 128 byte boundary and no external palette support
	oamInit(&oamSub, SpriteMapping_Bmp_1D_128, false);
	oamInit(&oamMain, SpriteMapping_Bmp_1D_128, false);

	// Init for background
	REG_BG3CNT = BG_MAP_BASE(1) | BG_BMP16_256x256 | BG_PRIORITY(3);
	REG_BG3X = 0;
	REG_BG3Y = 0;
	REG_BG3PA = 1<<8;
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	REG_BG3PD = 1<<8;

	REG_BG3CNT_SUB = BG_MAP_BASE(1) | BG_BMP16_256x256 | BG_PRIORITY(3);
	REG_BG3X_SUB = 0;
	REG_BG3Y_SUB = 0;
	REG_BG3PA_SUB = 1<<8;
	REG_BG3PB_SUB = 0;
	REG_BG3PC_SUB = 0;
	REG_BG3PD_SUB = 1<<8;
}

void loadFont(void) {
	loadPng("nitro:/graphics/font.png", font);
}

ImageData loadBmp(std::string path, std::vector<u16>& imageBuffer) {
    FILE* file = fopen(path.c_str(), "rb");

	// Get width and height on image
	ImageData imageData;
	char buffer[4];
	fseek(file, 0x12, SEEK_SET); // Width
	fread(buffer, 4, 1, file);
	imageData.width = *(int*)&buffer[0];
	fseek(file, 0x16, SEEK_SET); // Height
	fread(buffer, 4, 1, file);
	imageData.height = *(int*)&buffer[0];

	if(file) {
		// Start loading
		fseek(file, 0xe, SEEK_SET);
		u8 pixelStart = (u8)fgetc(file) + 0xe;
		fseek(file, pixelStart, SEEK_SET);
		u16 bmpImageBuffer[imageData.width*imageData.height];
		fread(bmpImageBuffer, 2, imageData.width*imageData.height, file);
		for(uint y=imageData.height-1; y>0; y--) {
			u16* src = bmpImageBuffer+y*imageData.width;
			for(uint x=0;x<imageData.width;x++) {
				u16 val = *(src++);
				imageBuffer.push_back(((val>>10)&31) | (val&(31)<<5) | (val&(31))<<10 | BIT(15));
			}
		}
	}
	fclose(file);
    return imageData;
}

ImageData loadPng(std::string path, std::vector<u16>& imageBuffer) {
    std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::decode(image, width, height, path);
	for(unsigned i=0;i<image.size()/4;i++) {
  		imageBuffer.push_back(ARGB16(image[(i*4)+3], image[i*4]>>3, image[(i*4)+1]>>3, image[(i*4)+2]>>3));
	}

    ImageData imageData;
    imageData.width = width;
    imageData.height = height;
    return imageData;
}

void drawImage(int x, int y, int w, int h, std::vector<u16> imageBuffer, bool top) {
	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
			if(imageBuffer[(i*w)+j] != 0xfc1f && imageBuffer[(i*w)+j]>>15 != 0) {
				(top ? BG_GFX : BG_GFX_SUB)[(y+i+32)*256+j+x] = imageBuffer[(i*w)+j];
			}
		}
	}
}

void drawImageScaled(int x, int y, int w, int h, double scale, std::vector<u16> imageBuffer, bool top) {
	scale = 1/scale;
	for(double i=0;i<h;i+=scale) {
		u16 ii = (u16)i;
		u16 is=(u16)(i/scale);
		for(double j=0;j<w;j+=scale) {
			u16 jj=(u16)j;
			if(imageBuffer[(ii*w)+jj] != 0xfc1f && imageBuffer[(ii*w)+jj]>>15 != 0) {
				u16 js=(u16)(j/scale);
				(top ? BG_GFX : BG_GFX_SUB)[(y+is+32)*256+js+x] = imageBuffer[(ii*w)+jj];	
			}
		}
	}
}

void drawImageTinted(int x, int y, int w, int h, u16 color, std::vector<u16> imageBuffer, bool top) {
	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
			if(imageBuffer[(i*w)+j] != 0xfc1f && imageBuffer[(i*w)+j]>>15 != 0) {
				(top ? BG_GFX : BG_GFX_SUB)[(y+i+32)*256+j+x] = imageBuffer[(i*w)+j] & color;	
			}
		}
	}
}

void drawRectangle(int x, int y, int w, int h, int color, bool top) {
	h+=y;
    for(;y<h;y++) {
        dmaFillHalfWords(((color>>10)&0x1f) | ((color)&(0x1f<<5)) | (color&0x1f)<<10 | BIT(15), (top ? BG_GFX : BG_GFX_SUB)+((y+32)*256+x), w*2);
	}
}

/**
 * Get the index in the UV coordinate array where the letter appears
 */
unsigned int getTopFontSpriteIndex(const u16 letter) {
	unsigned int spriteIndex = 0;
	long int left = 0;
	long int right = FONT_NUM_IMAGES;
	long int mid = 0;

	while (left <= right) {
		mid = left + ((right - left) / 2);
		if (fontUtf16LookupTable[mid] == letter) {
			spriteIndex = mid;
			break;
		}

		if (fontUtf16LookupTable[mid] < letter) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	return spriteIndex;
}
void printText(std::string text, int xPos, int yPos, bool top) { printTextTinted(StringUtils::UTF8toUTF16(text), WHITE, xPos, yPos, top); }
void printText(std::u16string text, int xPos, int yPos, bool top) { printTextTinted(text, WHITE, xPos, yPos, top); }
void printTextTinted(std::string text, u16 color, int xPos, int yPos, bool top) { printTextTinted(StringUtils::UTF8toUTF16(text), color, xPos, yPos, top); }

void printTextTinted(std::u16string text, u16 color, int xPos, int yPos, bool top) {
	int x = 0;

	std::ofstream os("sd:/test.log");
	for (uint c = 0; c < text.length(); c++) {
		unsigned int charIndex = getTopFontSpriteIndex(text[c]);

		for (int y = 0; y < 16; y++) {
			int currentCharIndex = ((512*(fontTexcoords[1+(4*charIndex)]+y))+fontTexcoords[0+(4*charIndex)]);
			os << std::hex << text[c] << std::dec << " || " << charIndex << "u" << fontTexcoords[0+(4*charIndex)] << "v" << fontTexcoords[1+(4*charIndex)] << std::endl;

			for (u16 i = 0; i < fontTexcoords[2 + (4 * charIndex)]; i++) {
				if (font[currentCharIndex+i]>>15 != 0) { // Do not render transparent pixel
					(top ? BG_GFX : BG_GFX_SUB)[(y+32+yPos)*256+(i+x+xPos)] = font[currentCharIndex+i] & color;
				}
			}
		}
		x += fontTexcoords[2 + (4 * charIndex)];
	}
	os.close();
}