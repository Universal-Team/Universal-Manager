#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics/stringUtils.hpp"

#include <nds.h>
#include <string>
#include <vector>

extern std::vector<u16> font;

// Converts 3 0-255 (0x0-0xff) values to a BGR15 value
#define BGR15(b, g, r)   ((b)|((g)<<5)|((r)<<10))


struct ImageData {
    uint width;
    uint height;
};

// Initializes the screens for drawing
void initGraphics(void);

/*
 * Loads a .bmp image into a vector of raw pixel data
 * std::string path is the path of the .bmp file
 * std::vector<u16>& imageBuffer is the vector to load the raw pixel data into
 * Returns an ImageData with the Width and Height of the image
 */
ImageData loadBmp(std::string path, std::vector<u16>& imageBuffer);

/*
 * Loads a .png image into a vector of raw pixel data
 * std::string path is the path of the .png file
 * std::vector<u16>& imageBuffer is the vector to load the raw pixel data into
 * Returns an ImageData with the Width and Height of the image
 */
ImageData loadPng(std::string path, std::vector<u16>& imageBuffer);

void loadFont(void);
/*
 * Draws an image to the screen from a vector of raw pixel data
 * int x is the X position
 * int y is the Y position
 * int w is the Width
 * int h is the Height
 * std::vector<u16> imageBuffer is the raw pixel data
 * bool top is whether to draw on the top or bottom screen
 */
void drawImage(int x, int y, int w, int h, std::vector<u16> imageBuffer, bool top);

/*
 * Draws a scaled image to the screen from a vector of raw pixel data
 * int x is the X position
 * int y is the Y position
 * int w is the Width
 * int h is the Height
 * double scaleX is the Scale to draw the X at
 * double scaleY is the Scale to draw the Y at
 * std::vector<u16> &imageBuffer is the raw pixel data
 * bool top is whether to draw on the top or bottom screen
 */
void drawImageScaled(int x, int y, int w, int h, double scaleX, double scaleY, std::vector<u16> &imageBuffer, bool top);

/*
 * Draws a scaled image to the screen from a vector of raw pixel data
 * int x is the X position
 * int y is the Y position
 * int w is the Width
 * int h is the Height
 * u16 color is the color to tint the image
 * std::vector<u16> imageBuffer is the raw pixel data
 * bool top is whether to draw on the top or bottom screen
 */
void drawImageTinted(int x, int y, int w, int h, u16 color, std::vector<u16> imageBuffer, bool top);

/*
 * Draws a rectangle of a given size at a given position
 * int x is the X position
 * int y is the Y position
 * int w is the Width
 * int h is the Height
 * int color is the color in RGB15
 * bool top is whether to draw on the top or bottom screen
 */
void drawRectangle(int x, int y, int w, int h, int color, bool top);

/**
 * Prints text to the a screen
 * std::string test is the text that will be printed
 * int xPos is the X position to print at
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printText(std::string text, int xPos, int yPos, bool top, bool invert = false);

/**
 * Prints text centered on a screen
 * std::u16string test is the text that will be printed
 * int xOffset is the X offset from the center to center on
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextCentered(std::string text, int xOffset, int yPos, bool top, bool invert = false);

/**
 * Prints colored text centered on a screen
 * u16 color is the color to tint the text
 * std::string test is the text that will be printed
 * int xOffset is the X offset from the center to center on
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextCenteredTinted(std::string text, u16 color, int xOffset, int yPos, bool top, bool invert = false);

/**
 * Prints colored text to the a screen
 * u16 color is the color to tint the text
 * std::string test is the text that will be printed
 * int xPos is the X position to print at
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextTinted(std::string text, u16 color, int xPos, int yPos, bool top, bool invert = false);


/**
 * Prints text to the a screen at a scaled size
 * std::string test is the text that will be printed
 * double scaleX is the width scale the text should be draw at
 * double scaleY is the height scale the text should be draw at
 * int xPos is the X position to print at
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextScaled(std::string text, double scaleX, double scaleY, int xPos, int yPos, bool top, bool invert = false);

/**
 * Prints centered text to the a screen at a scaled size
 * std::string test is the text that will be printed
 * double scaleX is the width scale the text should be draw at
 * double scaleY is the height scale the text should be draw at
 * int xOffset is the X offset from the center to center on
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextCenteredScaled(std::string text, double scaleX, double scaleY, int xOffset, int yPos, bool top, bool invert = false);


/**
 * Prints colored text to the a screen
 * u16 color is the color to tint the text
 * std::u16string test is the text that will be printed
 * int xPos is the X position to print at
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextTinted(std::u16string text, u16 color, int xPos, int yPos, bool top, bool invert = false);


/**
 * Prints colored text to the a screen at a scaled size
 * std::u16string test is the text that will be printed
 * double scaleX is the width scale the text should be draw at
 * double scaleY is the height scale the text should be draw at
 * u16 color is the color to tint the text
 * int xPos is the X position to print at
 * int yPos is the Y position to print at
 * bool top is whether to draw on the top or bottom screen
 * bool invert is whether to swap the colors
 */
void printTextTintedScaled(std::u16string text, double scaleX, double scaleY, u16 color, int xPos, int yPos, bool top, bool invert = false);



/**
 * Gets the width of a std::string of text with a max
 * std::string text is the text to get the width of
 * returns the width of the text
 */
int getTextWidthMaxW(std::string text, int w);

/**
 * Gets the width of a std::string of scaled text
 * std::string text is the text to get the width of
 * returns the width of the text
 */
int getTextWidthScaled(std::string text, double scale);

/**
 * Gets the width of a std::string of text
 * std::string text is the text to get the width of
 * returns the width of the text
 */
int getTextWidth(std::string text);

/**
 * Gets the width of a std::u16string of text
 * std::u16string text is the text to get the width of
 * returns the width of the text
 */
int getTextWidth(std::u16string text);

// Color Stuff. ;P

#define BLACK 0
#define DARK_BLUE 0x800C
#define DARK_GRAY 0x98C6
#define WHITE 0xFFFF
#define GRAY 0xBDEF
#define BARCOLOR 0xA0FD

#endif //GRAPHICS_H
