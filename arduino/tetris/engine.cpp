
#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

#include "system.h"
#include "draw.h"
#include "shapes.h"
#include "gamestate.h"
#include "engine.h"
#include "random.h"

typedef struct {

  	//
	GAMESTATE gameState;

        // which set of input keys to use
        int inputKeySet;
        
	// this flag signals outwards to stop immediately
	int gameOver;

	// fast-drop accumulation (for additional score)
	int fastDrop;

        // 
        int outgoingPushRows;
        
        //
        int incomingPushRows;
        
        // 
        int flashRowsEffect;

} ENGINE_DATA;

int delayByLevel [] = {
	30, 27, 24, 21, 18, 15, 12, 8, 5, 2
};
int delayLevels = 10;

void engineFrame (void)
{
  waitForTimerTick();
}

void engineFrames (int frameCount)
{
	int i;
	for (i=0; i<frameCount; i++)
		engineFrame ();
}

static const char flashRowsEffectColors[] = {7, 3, 0, 3, 7, 3, 0};
static const int flashRowsEffectTotalLength = 35;

void drawFlashRowsEffect (int count, int *rows, int frame)
{
  fillGameRows(count, rows, flashRowsEffectColors[frame / 5]);
}

void recolorTiles (ENGINE_DATA *data, unsigned char color)
{
	int i, j;
	unsigned char *gameArea = data->gameState.gameArea;

	for (i=0; i<10; i++) {
		for (j=0; j<20; j++) {
			if (gameArea [j*10+i] != 0)
				gameArea [j*10+i] = color;
		}
	}

	drawGameArea (gameArea);
}

void newLevel (ENGINE_DATA *data)
{
	// drawLevel (data->gameState.rows / 10);

  for (int i=0; i<3; i++) {
    for (int j=1; j<8; j++) {
      recolorTiles (data, j);
      engineFrames(2);
    }
  }
  recolorTiles (data, 3);
  engineFrames (5);
}

void gameOverFill (void)
{
	int i;
	for (i=19; i>=0; i--) {
		fillGameRow (i, 7);
		engineFrames (5);
	}
}

static void clearPreview (ENGINE_DATA *data)
{
	drawPieceInPreview (0, data->gameState.preview0 & 0xff, 0);
	drawPieceInPreview (1, data->gameState.preview1 & 0xff, 0);
	drawPieceInPreview (2, data->gameState.preview2 & 0xff, 0);
}

static void drawPreview (ENGINE_DATA *data)
{
	drawPieceInPreview (0, data->gameState.preview0 & 0xff, data->gameState.preview0 >> 8);
	drawPieceInPreview (1, data->gameState.preview1 & 0xff, data->gameState.preview1 >> 8);
	drawPieceInPreview (2, data->gameState.preview2 & 0xff, data->gameState.preview2 >> 8);
}

void engineNewGame (ENGINE_DATA *data, int inputKeySet)
{
	initializeGameState (&data->gameState);
        data->inputKeySet = inputKeySet;
	data->gameOver = 0;
	data->fastDrop = 0;
	data->outgoingPushRows = 0;
	data->incomingPushRows = 0;
        data->flashRowsEffect = 0;

	drawBackground ();
	drawPreview (data);
	// drawScore (data->gameState.score);
	// drawLevel (data->gameState.rows / 10);
}

void engineLeft (ENGINE_DATA *data)
{
	if (moveCurrentShapeLeft (&data->gameState)) {
		drawShapeOnGameArea (data->gameState.shapeX + 1, data->gameState.shapeY,
			data->gameState.shapeIndex, 0);
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			data->gameState.shapeIndex, data->gameState.shapeColor);
	}
}

void engineRight (ENGINE_DATA *data)
{
	if (moveCurrentShapeRight (&data->gameState)) {
		drawShapeOnGameArea (data->gameState.shapeX - 1, data->gameState.shapeY,
			data->gameState.shapeIndex, 0);
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			data->gameState.shapeIndex, data->gameState.shapeColor);
	}
}

int engineDown (ENGINE_DATA *data)
{
	if (moveCurrentShapeDown (&data->gameState)) {
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY - 1,
			data->gameState.shapeIndex, 0);
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			data->gameState.shapeIndex, data->gameState.shapeColor);
                return 0;
	} else {
		int completedRows [4];
		int count;

		if (pasteCurrentShape (&data->gameState)) {
			data->gameOver = 1;
			gameOverFill ();
			return 1;
		}

		data->gameState.score += data->fastDrop;
		data->fastDrop = 0;

		count = findCompletedRows (&data->gameState, data->gameState.shapeY, 4, completedRows);
                if (count == 0) {
                  clearPreview (data);
                  nextPiece (&data->gameState);
                  drawPreview (data);
                } else {
                  data->flashRowsEffect = 1;
                }
                
		// drawScore (data->gameState.score);
		return 1;
	}
}

void engineRotateClockwise (ENGINE_DATA *data)
{
	if (rotateCurrentShapeClockwise (&data->gameState)) {
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			shapeRotatedCounterClockwise [data->gameState.shapeIndex], 0);
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			data->gameState.shapeIndex, data->gameState.shapeColor);
	}
}

void engineRotateCounterClockwise (ENGINE_DATA *data)
{
	if (rotateCurrentShapeCounterClockwise (&data->gameState)) {
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			shapeRotatedClockwise [data->gameState.shapeIndex], 0);
		drawShapeOnGameArea (data->gameState.shapeX, data->gameState.shapeY,
			data->gameState.shapeIndex, data->gameState.shapeColor);
	}
}

void gameStep (ENGINE_DATA *data, long stepCounter)
{
        if (data->flashRowsEffect) {
          int completedRows [4];
          int count;
          count = findCompletedRows (&data->gameState, data->gameState.shapeY, 4, completedRows);
          if (data->flashRowsEffect == flashRowsEffectTotalLength) {
            data->flashRowsEffect = 0;
            removeRows (&data->gameState, count, completedRows);
            if (addRows (&data->gameState, count)) {
              newLevel (data);
            } else {
              drawGameArea (data->gameState.gameArea);
            }
            if (count > 1) {
              data->outgoingPushRows += (count == 4 ? 4 : count - 1);
            }
            clearPreview (data);
            nextPiece (&data->gameState);
            drawPreview (data);
          } else {
            drawFlashRowsEffect(count, completedRows, data->flashRowsEffect);
            data->flashRowsEffect++;
          }
          return;
        }
        
        if (data->incomingPushRows > 0) {
          pushHoleRows(&(data->gameState), data->incomingPushRows);
          data->incomingPushRows = 0;
	  drawGameArea (data->gameState.gameArea);
        }
  
	if (keyPressed[data->inputKeySet][KEY_LEFT]) {
		keyPressed[data->inputKeySet][KEY_LEFT] = 0;
		engineLeft(data);
	}

	if (keyPressed[data->inputKeySet][KEY_RIGHT]) {
		keyPressed[data->inputKeySet][KEY_RIGHT] = 0;
		engineRight(data);
	}

	if (keyPressed[data->inputKeySet][KEY_DOWN]) {
		keyPressed[data->inputKeySet][KEY_DOWN] = 0;
		while (!engineDown (data));
	} else {
		int level = data->gameState.rows / 10;
		if ((level > delayLevels) || (stepCounter % delayByLevel [level] == 0)) {
			engineDown (data);
		}
	}

	if (keyPressed[data->inputKeySet][KEY_CLOCKWISE]) {
		keyPressed[data->inputKeySet][KEY_CLOCKWISE] = 0;
		engineRotateClockwise (data);
	}

	if (keyPressed[data->inputKeySet][KEY_COUNTER_CLOCKWISE]) {
		keyPressed[data->inputKeySet][KEY_COUNTER_CLOCKWISE] = 0;
		engineRotateCounterClockwise (data);
	}

}

static int gameRunning = 0;
static long mainStepCounter = 0;
static ENGINE_DATA engineData[2];

void engineMainLoopTick (void)
{
    if (gameRunning) {
      if (engineData[0].gameOver || engineData[1].gameOver) {
        anyKeyPressed = 0;
        gameRunning = 0;
      } else {
        engineFrame();
        
        currentDrawingDisplay = 0;
        gameStep(engineData + 0, mainStepCounter);
        currentDrawingDisplay = 1;
        gameStep(engineData + 1, mainStepCounter);
        
        engineData[0].incomingPushRows += engineData[1].outgoingPushRows;
        engineData[1].incomingPushRows += engineData[0].outgoingPushRows;
        engineData[0].outgoingPushRows = 0;
        engineData[1].outgoingPushRows = 0;
        
        mainStepCounter++;
      }
    } else {
        currentDrawingDisplay = 0;
	drawTitleScreen ();
        currentDrawingDisplay = 1;
	drawTitleScreen ();
        if (anyKeyPressed) {
          for (int i=0; i<2; i++) {
            for (int j=0; j<5; j++) {
              keyPressed[i][j] = 0;
            }
          }
          autoSeedRandom();
          gameRunning = 1;
          currentDrawingDisplay = 0;
          engineNewGame(engineData + 0, 0);
          currentDrawingDisplay = 1;
          engineNewGame(engineData + 1, 1);
        } else {
          randomAutoSeederTick();
        }
    }
}
