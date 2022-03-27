// header file for drawtext

#ifndef __DRAWTEXT_H__
#define __DRAWTEXT_H__

// data structure used to hold pixel representations of each possible drawable character
extern char fontdata_8x8[];

// draw a string str on the screen at top-left coord (xPos,yPos) in color color
void draw_text(int xPos, int yPos, char* str, u_int32_t color);

// clear charsToClear characters of space starting at top-left coord (xPos,yPos)
void clear_text(int xPos, int yPos, int charsToClear);

// clear pixel space equal to the number of characters in str starting at coord (xPos,yPos),
// then write string into the newly-cleared space
void clear_and_draw_text(int xPos, int yPos, char* str, u_int32_t color);

#endif