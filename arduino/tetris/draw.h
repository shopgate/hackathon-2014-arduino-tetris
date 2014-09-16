
// offet of the game area in screen coordinates
#define GAME_AREA_X_ON_SCREEN	3
#define GAME_AREA_Y_ON_SCREEN	10

// offet of the (leftmost) preview box screen coordinates
#define PREVIEW_X_ON_SCREEN		1
#define PREVIEW_Y_ON_SCREEN		2

// x offset of one preview box to the next
#define PREVIEW_X_DELTA			5

// draw the screen background (i.e. anything that doesn't change during the game)
void drawBackground (void);

// draw the title screen
void drawTitleScreen (void);

/* These drawing functions draw a shape using colors. The shapeIndex selects
 * one of 19 shapes as defined in the "shapes" module. All pixels
 * which are occupied by the shape are overdrawn with the c color; all other
 * positions are left alone.
 */

/* Draw a shape using absolute screen coordinates, clipped against the
 * specified clipping rectangle.
 */
void drawClippedShapeOnScreen (int x, int y, int shapeIndex, char c, int minx, int miny, int maxx, int maxy);

/* Draw a shape using absolute screen coordinates and clipped against
 * the screen borders.
 */
void drawShapeOnScreen (int x, int y, int shapeIndex, char c);

/* Draw a shape using game-area coordinates. The shape is automatically clipped
 * against the borders of the game area.
 */
void drawShapeOnGameArea (int x, int y, int shapeIndex, char c);

/* Draw a shape in one of the preview boxes. The preview index must be one of 0, 1, 2.
 * NOTE: Only the preview shapes may be used, otherwise the shape may not fit into
 * the preview box.
 */
void drawShapeInPreview (int previewIndex, int shapeIndex, char c);

/* Draw the preview shape of the specified piece in one of the preview boxes.
 * The preview index must be one of 0, 1, 2.
 */
void drawPieceInPreview (int previewIndex, int pieceIndex, char c);

/* Fill a row in the game area with the specified color
 */
void fillGameRow (int y, char c);

/* Fill multiple rows in the game area with the specified color
 */
void fillGameRows (int count, int *rows, char c);

/* Copy color data to the game area. The data array must be of size 10*20.
 */
void drawGameArea (unsigned char *data);

