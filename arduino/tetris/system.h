
#define NULL				0
#define INVALID_INTEGER		((int)0x80000000)

#define COLOR(r,g,b) ((((r) & 0xF8) << 11) | (((g) & 0xFC) << 5) | ((b) >> 3))
#define C_BLACK COLOR(0, 0, 0)
#define C_RED COLOR(255, 0, 0)
#define C_GREEN COLOR(0, 255, 0)
#define C_BLUE COLOR(0, 0, 255)
#define C_YELLOW COLOR(255, 255, 0)
#define C_CYAN COLOR(0, 255, 255)
#define C_PURPLE COLOR(255, 0, 255)
#define C_WHITE COLOR(255, 255, 255)

extern int currentDrawingDisplay;

#define PUT_PIXEL(x,y,color) putPixel(x, y, color)

#define KEY_LEFT  0
#define KEY_RIGHT  1
#define KEY_DOWN  2
#define KEY_CLOCKWISE  3
#define KEY_COUNTER_CLOCKWISE  4

extern volatile int keyPressed[2][5];
extern volatile int anyKeyPressed;

void handleSerialCharacter(int c);
void waitForTimerTick();
void putPixel(int x, int y, uint16_t color);

