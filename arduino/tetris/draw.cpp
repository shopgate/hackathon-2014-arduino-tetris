
#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

extern RGBmatrixPanel matrix;

#include "system.h"
#include "draw.h"
#include "shapes.h"

const uint16_t colorTable[] = {
  C_BLACK,
  C_RED,
  C_GREEN,
  C_BLUE,
  C_YELLOW,
  C_CYAN,
  C_PURPLE,
  C_WHITE,
};

static const unsigned char titleScreenTemplate[16*32] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0,
	0, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 3, 3, 3, 3, 3, 3, 0, 0, 4, 4, 4, 4, 0, 0, 0,
	0, 3, 3, 3, 3, 3, 3, 0, 0, 4, 4, 4, 4, 4, 4, 0,
	0, 0, 0, 3, 3, 0, 0, 0, 0, 4, 4, 0, 0, 4, 4, 0,
	0, 0, 0, 3, 3, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0,
	0, 0, 0, 3, 3, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0,
	0, 0, 0, 3, 3, 0, 0, 0, 0, 4, 4, 0, 4, 4, 0, 0,
	0, 0, 0, 3, 3, 0, 0, 0, 0, 4, 4, 0, 4, 4, 4, 0,
	0, 0, 0, 3, 3, 0, 0, 0, 0, 4, 4, 0, 0, 4, 4, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
	0, 0, 0, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0,
	0, 0, 0, 5, 5, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0,
	0, 0, 0, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0, 0,
	0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0,
	0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0,
	0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0,
	0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const unsigned char backgroundTemplate[16*32] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void drawScreen(const unsigned char *screenTemplate)
{
	int i, j;
	for (i=0; i<16; i++) {
		for (j=0; j<32; j++) {
			PUT_PIXEL (i, j, colorTable[screenTemplate [j*16+i]]);
		}
	}
}

void drawBackground (void)
{
	drawScreen (backgroundTemplate);
}

void drawTitleScreen (void)
{
	drawScreen (titleScreenTemplate);
}

void drawClippedShapeOnScreen (int x, int y, int shapeIndex, char c, int minx, int miny, int maxx, int maxy)
{
	int i, j;
	unsigned char *occupationMatrix = shapeOccupationMatrices [shapeIndex];

	for (i=0; i<4; i++) {
		int x2 = x + i;
		if (x2 < minx || x2 > maxx) continue;

		for (j=0; j<4; j++) {
			int y2 = y + j;
			if (y2 < miny || y2 > maxy) continue;

			if (occupationMatrix [j*4+i])
				PUT_PIXEL (x2, y2, colorTable[c]);
		}
	}
}

void drawShapeOnScreen (int x, int y, int shapeIndex, char c)
{
	drawClippedShapeOnScreen (x, y, shapeIndex, c, 0, 0, 39, 29);
}


void drawShapeOnGameArea (int x, int y, int shapeIndex, char c)
{
	drawClippedShapeOnScreen (x + GAME_AREA_X_ON_SCREEN, y + GAME_AREA_Y_ON_SCREEN,
		shapeIndex, c,
		GAME_AREA_X_ON_SCREEN, GAME_AREA_Y_ON_SCREEN,
		GAME_AREA_X_ON_SCREEN + 9, GAME_AREA_Y_ON_SCREEN + 19);
}

void drawShapeInPreview (int previewIndex, int shapeIndex, char c)
{
  drawShapeOnScreen (PREVIEW_X_ON_SCREEN + previewIndex * PREVIEW_X_DELTA, PREVIEW_Y_ON_SCREEN, shapeIndex, c);
}

void drawPieceInPreview (int previewIndex, int pieceIndex, char c)
{
	drawShapeInPreview (previewIndex, normalShapeByPiece [pieceIndex], c);
}

void fillGameRow (int y, char c)
{
	int i;
	for (i=0; i<10; i++) {
		PUT_PIXEL (GAME_AREA_X_ON_SCREEN + i, GAME_AREA_Y_ON_SCREEN + y, colorTable[c]);
	}
}

void fillGameRows (int count, int *rows, char c)
{
	int i;
	for (i=0; i<count; i++)
		fillGameRow (rows [i], c);
}

void drawGameArea (unsigned char *data)
{
	int i, j;
	for (i=0; i<10; i++) {
		for (j=0; j<20; j++) {
			PUT_PIXEL (GAME_AREA_X_ON_SCREEN + i, GAME_AREA_Y_ON_SCREEN + j, colorTable[data [j*10+i]]);
		}
	}
}
