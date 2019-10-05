#include "graphics/graphics.h"
#include "graphics/lodepng.h"

#include <fstream>

std::vector<u16> font;
std::vector<char> fontTiles;
std::vector<char> fontWidths;
std::vector<u16> fontMap;
std::u16string newline = StringUtils::UTF8toUTF16("»");
u16 tileSize, tileWidth, tileHeight;

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
	FILE* font = fopen("nitro:/graphics/font.nftr", "rb");

	// Get file size
	fseek(font, 0, SEEK_END);
	u32 fileSize = ftell(font);

	// Load font info
	fseek(font, 0x30, SEEK_SET);
	u32 chunkSize;
	fread(&chunkSize, 4, 1, font);
	fseek(font, 0x34, SEEK_SET);
	tileWidth = fgetc(font);
	fseek(font, 0x35, SEEK_SET);
	tileHeight = fgetc(font);
	fseek(font, 0x36, SEEK_SET);
	fread(&tileSize, 2, 1, font);

	// Load character glyphs
	int tileAmount = ((chunkSize-0x10)/tileSize);
	fontTiles = std::vector<char>(tileSize*tileAmount);
	fseek(font, 0x3C, SEEK_SET);
	fread(fontTiles.data(), tileSize, tileAmount, font);

	// Fix top row
	for(int i=0;i<tileAmount;i++) {
		fontTiles[i*tileSize] = 0;
		fontTiles[i*tileSize+1] = 0;
		fontTiles[i*tileSize+2] = 0;
	}

	// Load character widths
	fseek(font, 0x24, SEEK_SET);
	u32 locHDWC;
	fread(&locHDWC, 4, 1, font);
	fseek(font, locHDWC-4, SEEK_SET);
	fread(&chunkSize, 4, 1, font);
	fseek(font, 8, SEEK_CUR);
	fontWidths = std::vector<char>(3*tileAmount);
	fread(fontWidths.data(), 3, tileAmount, font);

	// Load character maps
	fontMap = std::vector<u16>(0xffe5);
	fseek(font, 0x28, SEEK_SET);
	u32 locPAMC, mapType;
	fread(&locPAMC, 4, 1, font);

	while(locPAMC < fileSize) {
		u16 firstChar, lastChar;
		fseek(font, locPAMC, SEEK_SET);
		fread(&firstChar, 2, 1, font);
		fread(&lastChar, 2, 1, font);
		fread(&mapType, 4, 1, font);
		fread(&locPAMC, 4, 1, font);

		switch(mapType) {
			case 0: {
				u16 firstTile;
				fread(&firstTile, 2, 1, font);
				for(unsigned i=firstChar;i<=lastChar;i++) {
					fontMap[i] = firstTile+(i-firstChar);
				}
				break;
			} case 1: {
				for(int i=firstChar;i<=lastChar;i++) {
					u16 tile;
					fread(&tile, 2, 1, font);
					fontMap[i] = tile;
				}
				break;
			} case 2: {
				u16 groupAmount;
				fread(&groupAmount, 2, 1, font);
				for(int i=0;i<groupAmount;i++) {
					u16 charNo, tileNo;
					fread(&charNo, 2, 1, font);
					fread(&tileNo, 2, 1, font);
					fontMap[charNo] = tileNo;
				}
				break;
			}
		}
	}
	fclose(font);
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

void drawImageFromSheet(int x, int y, int w, int h, std::vector<u16> &imageBuffer, int imageWidth, int xOffset, int yOffset, bool top) {
	for(int i=0;i<h;i++) {
		for(int j=0;j<w;j++) {
			if(imageBuffer[((i+yOffset)*imageWidth)+j+xOffset]>>15 != 0) { // Do not render transparent pixel
				(top ? BG_GFX : BG_GFX_SUB)[((y+i)*256)+j+x] = imageBuffer[((i+yOffset)*imageWidth)+j+xOffset];
			}
		}
	}
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

void drawImageScaled(int x, int y, int w, int h, double scaleX, double scaleY, std::vector<u16> &imageBuffer, bool top) {
	if(scaleX == 1 && scaleY == 1)	drawImage(x, y, w, h, imageBuffer, top);
	else {
		for(int i=0;i<(h*scaleY);i++) {
			for(int j=0;j<(w*scaleX);j++) {
				if(imageBuffer[(((int)(i/scaleY))*w)+(j/scaleX)]>>15 != 0) { // Do not render transparent pixel
					(top ? BG_GFX : BG_GFX_SUB)[(y+i)*256+x+j] = imageBuffer[(((int)(i/scaleY))*w)+(j/scaleX)];
				}
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

void printText(std::string text, int xPos, int yPos, bool top, bool invert) { printTextTinted(StringUtils::UTF8toUTF16(text), WHITE, xPos, yPos, top, invert); }
void printTextCentered(std::string text, int xOffset, int yPos, bool top, bool invert) { printTextTinted(StringUtils::UTF8toUTF16(text), WHITE, ((256-getTextWidth(StringUtils::UTF8toUTF16(text)))/2)+xOffset, yPos, top, invert); }
void printTextCenteredTinted(std::string text, u16 color, int xOffset, int yPos, bool top, bool invert) { printTextTinted(StringUtils::UTF8toUTF16(text), color, ((256-getTextWidth(StringUtils::UTF8toUTF16(text)))/2)+xOffset, yPos, top, invert); }
void printTextTinted(std::string text, u16 color, int xPos, int yPos, bool top, bool invert) { printTextTinted(StringUtils::UTF8toUTF16(text), color, xPos, yPos, top, invert); }
void printTextScaled(std::string text, double scaleX, double scaleY, int xPos, int yPos, bool top, bool invert) { printTextTintedScaled(StringUtils::UTF8toUTF16(text), scaleX, scaleY, WHITE, xPos, yPos, top, invert); }
void printTextCenteredScaled(std::string text, double scaleX, double scaleY, int xOffset, int yPos, bool top, bool invert) { printTextTintedScaled(StringUtils::UTF8toUTF16(text), scaleX, scaleY, WHITE, ((256-getTextWidthScaled(text, scaleX))/2)+xOffset, yPos, top, invert); }

void printTextTinted(std::u16string text, u16 color, int xPos, int yPos, bool top, bool invert) {
	int x=xPos;
	u16 color1 = color & (invert ? 0xBDEF : 0xFBDE);
	u16 color2 = color & (invert ? 0xFBDE : 0xBDEF);
	u16 pallet[4] = {0, color1, color2, 0};
	for(unsigned c=0;c<text.size();c++) {
		if(text[c] == newline[0]) {
			x = xPos;
			yPos += tileHeight;
			continue;
		}

		int t = fontMap[text[c]];
		std::vector<u16> image;
		for(int i=0;i<tileSize;i++) {
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>6 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>4 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>2 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)] & 3]);
		}

		x += fontWidths[t*3];
		if(x > 256) {
			x = xPos+fontWidths[t*3];
			yPos += tileHeight;
		}
		drawImage(x, yPos, tileWidth, tileHeight, image, top);
		x += fontWidths[(t*3)+1];
	}
}


void printTextTintedScaled(std::u16string text, double scaleX, double scaleY, u16 color, int xPos, int yPos, bool top, bool invert) {
	if(scaleX == 1 && scaleY == 1) {
		printTextTinted(text, color, xPos, yPos, top, invert);
		return;
	}

	int x=xPos;
	u16 color1 = color & (invert ? 0xBDEF : 0xFBDE);
	u16 color2 = color & (invert ? 0xFBDE : 0xBDEF);
	u16 pallet[4] = {0, color1, color2, 0};
	for(unsigned c=0;c<text.size();c++) {
		if(text[c] == newline[0]) {
			x = xPos;
			yPos += tileHeight;
			continue;
		}

		int t = fontMap[text[c]];
		std::vector<u16> image;
		for(int i=0;i<tileSize;i++) {
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>6 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>4 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)]>>2 & 3]);
			image.push_back(pallet[fontTiles[i+(t*tileSize)] & 3]);
		}

		x += fontWidths[t*3];
		if(x > 256) {
			x = xPos+fontWidths[t*3];
			yPos += tileHeight;
		}
		drawImageScaled(x, yPos, tileWidth, tileHeight, scaleX, scaleY, image, top);
		x += fontWidths[(t*3)+1]*scaleX;
	}
}

int getTextWidthMaxW(std::string text, int w);
int getTextWidthScaled(std::string text, double scale) { return getTextWidth(StringUtils::UTF8toUTF16(text))*scale; }
int getTextWidth(std::string text);

int getTextWidth(std::u16string text) {
	int textWidth = 0;
	for(unsigned c=0;c<text.size();c++) {
		textWidth += fontWidths[(fontMap[text[c]]*3)+2];
	}
	return textWidth;
}