// header file for drawtext

#ifndef __DRAWTEXT_H__
#define __DRAWTEXT_H__

// data structure used to hold pixel representations of each possible drawable character
extern char fontdata_8x8[];

// draw a string str on the screen at top-left coord (xPos,yPos) in color color
// args:
// int xPos: x-position on screen's 2-D plane
// int yPos: y-position on screen's 2-D plane
// char* str: pointer to the string to write
// uint32_t color: 32-bit color value, 4 bytes representing T, R, G, B
void draw_text(int xPos, int yPos, char* str, uint32_t color);

// clear charsToClear characters of space starting at top-left coord (xPos,yPos)
// args:
// int xPos: x-position on screen's 2-D plane
// int yPos: y-position on screen's 2-D plane
// int charsToClear: width of area to clear in number of characters (each is 8 pixels wide)
void clear_text(int xPos, int yPos, int charsToClear);

// clear pixel space equal to the number of characters in str starting at coord (xPos,yPos),
// then write string into the newly-cleared space
// args:
// int xPos: x-position on screen's 2-D plane
// int yPos: y-position on screen's 2-D plane
// char* str: pointer to the string to write
// uint32_t color: 32-bit color value, 4 bytes representing T, R, G, B
void clear_and_draw_text(int xPos, int yPos, char* str, uint32_t color);

#endif