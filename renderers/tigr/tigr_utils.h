#ifndef __TIGR_UTILS_H__
#define __TIGR_UTILS_H__

#include <math.h>

#include "tigr.h"

/* Fast arc drawing algorithm based on Bressenham Circle Routine
 *
 * taken from --> https://www.scattergood.io/arc-drawing-algorithm/
 */
void tigrArc(Tigr* ctx, int xc, int yc, int sa, int ea, int r, TPixel col);

/* Function to draw rectangles with arches in the angles of them
 * makes use of the tigrArc function
 *
 * This is the wire frame one
 */
void tigrArcRect(Tigr* ctx, int x, int y, int w, int h, int r, TPixel col);

/* Function to draw rectangles with arches in the angles of them
 * makes use of the tigrArc function
 *
 * This is the filled one
 */
void tigrFillArcRect(Tigr* ctx, int x, int y, int w, int h, int r, TPixel col);

/* ChatGPT generated blitScale function */
void tigrBlitScale(Tigr* dst, Tigr* src, int dx, int dy, int sx, int sy, int sw, int sh, int dw, int dh);

#endif /* __TIGR_UTILS_H__ */
