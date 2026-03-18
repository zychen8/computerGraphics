/*
Ziyan Chen
2024 Summer Term
header file of curve
*/

#ifndef CURVE_H
#define CURVE_H

#include "graphics.h"
#include "image.h"


typedef struct {
  int zBufferFlag;
  Point p[4];
} BezierCurve; 

typedef struct {
  int zBufferFlag;
  Point p[16];
} BezierSurface;

// sets the zbuffer flag to 1 and the curve points to the X-axis between 0 and 1.
void bezierCurve_init(BezierCurve *b);

// sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) and (1, 1).
void bezierSurface_init(BezierSurface *b);

// sets the control points of the BezierCurve to the four points in the vlist array.
void bezierCurve_set(BezierCurve *b, Point *vlist);

// sets the control points of the BezierSurface to the 16 points in the vlist array.
void bezierSurface_set(BezierSurface *b, Point *vlist);

// set the Bezier surface point with index u, v to p.
void bezierSurface_setPoint( BezierSurface *bc, Point *p, int u, int v );

// set the value of p to the Bezier surface point with index u, v.
void bezierSurface_getPoint( BezierSurface *bc, Point *p, int u, int v );

// sets the z-buffer flag to the given value.
void bezierCurve_zBuffer(BezierCurve *p, int flag);

// sets the z-buffer flag to the given value.
void bezierSurface_zBuffer(BezierCurve *p, int flag);

// draws the Bezier curve, given in screen coordinates, into the image using the given color. The function should be adaptive so that it uses an appropriate number of line segments to draw the curve. For example, if the bounding box of the control points is less than 10 pixels in the largest dimension, ßßßthen it is reasonable to draw the lines between the control points as an approximation to the curve.
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);

#endif