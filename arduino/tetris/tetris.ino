
#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>

// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

#define CLK 11  // Uno: 8, Mega: 11
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// ------------------------------------------------------------------------------------------------------------------------------------------------
// --- includes
// ------------------------------------------------------------------------------------------------------------------------------------------------

#include "system.h"
#include "draw.h"
#include "engine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------
// --- system functions
// ------------------------------------------------------------------------------------------------------------------------------------------------

#define TIMER_TICK_DURATION_MSEC 30
long previousTimerTickTime = 0;
int currentDrawingDisplay;

void setup() {
  matrix.begin();
  matrix.setTextWrap(true);
  matrix.setTextSize(1);
  Serial.begin(115200);
  while (Serial.available()) {
    Serial.read();
  }
}

void loop() {
  engineMainLoopTick();
}

volatile int keyPressed[2][5];
volatile int anyKeyPressed;

void serialEvent() {
  while (Serial.available()) {
    switch (Serial.read()) {
    
      case 'a':
        keyPressed[0][KEY_LEFT] = 1;
        break;
        
      case 'd':
        keyPressed[0][KEY_RIGHT] = 1;
        break;
      
      case 's':
        keyPressed[0][KEY_DOWN] = 1;
        break;
      
      case 'e':
        keyPressed[0][KEY_CLOCKWISE] = 1;
        break;
      
      case 'q':
        keyPressed[0][KEY_COUNTER_CLOCKWISE] = 1;
        break;
    
      case 'j':
        keyPressed[1][KEY_LEFT] = 1;
        break;
        
      case 'l':
        keyPressed[1][KEY_RIGHT] = 1;
        break;
      
      case 'k':
        keyPressed[1][KEY_DOWN] = 1;
        break;
      
      case 'u':
        keyPressed[1][KEY_CLOCKWISE] = 1;
        break;
      
      case 'o':
        keyPressed[1][KEY_COUNTER_CLOCKWISE] = 1;
        break;
        
    }
    anyKeyPressed = 1;
  }
}

void waitForTimerTick() {
  while (millis() - previousTimerTickTime < TIMER_TICK_DURATION_MSEC);
  previousTimerTickTime = millis();
}

void putPixel(int x, int y, uint16_t color) {
  int x2, y2;
  if (currentDrawingDisplay) {
    x2 = 32 + y;
    y2 = 15 - x;
  } else {
    x2 = 31 - y;
    y2 = x;
  }
  matrix.drawPixel(x2, y2, color);
}


