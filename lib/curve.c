/*
Ziyan Chen
2024 Summer Term

library file of Bezier curve and surface
*/

#include "graphics.h"
#include "curve.h"
#include "image.h"



// sets the zbuffer flag to 1 and the curve points to the X-axis between 0 and 1.
void bezierCurve_init(BezierCurve *b) {
  b->zBufferFlag = 1;
  for (int i = 0; i < 4; i++) {
    point_set3D(&b->p[i], 0, 0, 0);
  }
}

// sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) and (1, 1).
void bezierSurface_init(BezierSurface *b) {
  b->zBufferFlag = 1;
  for (int i = 0; i < 16; i++) {
    point_set3D(&b->p[i], 0, 0, 0);
  }
}

// sets the control points of the BezierCurve to the four points in the vlist array.
void bezierCurve_set(BezierCurve *b, Point *vlist) {
  for (int i = 0; i < 4; i++) {
    point_copy(&b->p[i], &vlist[i]);
  }
}


// sets the control points of the BezierSurface to the 16 points in the vlist array.
void bezierSurface_set(BezierSurface *b, Point *vlist) {
  for (int i = 0; i < 16; i++) {
    point_copy(&b->p[i], &vlist[i]);
  }
}

// set the Bezier surface point with index u, v to p.
void bezierSurface_setPoint( BezierSurface *bc, Point *p, int u, int v ) {
  if (bc == NULL || p == NULL || u < 0 || u > 3 || v < 0 || v > 3) {
    printf("bezierSurface_setPoint: invalid input\n");
    return;
  }
  point_copy(&bc->p[u * 4 + v], p);
}

// set the value of p to the Bezier surface point with index u, v.
void bezierSurface_getPoint( BezierSurface *bc, Point *p, int u, int v ) {
  if (bc == NULL || p == NULL || u < 0 || u > 3 || v < 0 || v > 3) {
    printf("bezierSurface_getPoint: invalid input\n");
    return;
  }
  point_copy(p, &bc->p[u * 4 + v]);
}

// sets the z-buffer flag to the given value.
void bezierCurve_zBuffer(BezierCurve *p, int flag) {
  p->zBufferFlag = flag;
}

// sets the z-buffer flag to the given value.
void bezierSurface_zBuffer(BezierCurve *p, int flag) {
  p->zBufferFlag = flag;
}

// set bounds of the curve
static double boundingBoxDimension(Point *p) {
    double minX = fmin(fmin(fmin(p[0].x, p[1].x), p[2].x), p[3].x);
    double maxX = fmax(fmax(fmax(p[0].x, p[1].x), p[2].x), p[3].x);
    double minY = fmin(fmin(fmin(p[0].y, p[1].y), p[2].y), p[3].y);
    double maxY = fmax(fmax(fmax(p[0].y, p[1].y), p[2].y), p[3].y);
    double width = maxX - minX;
    double height = maxY - minY;
    return fmax(width, height);
}

// recursive function to draw the Bezier curve
static void drawBezierCurveRecursive(Point *p, Image *src, Color c) {
    if (boundingBoxDimension(p) < 10) {
        // base case
        Line l;
        point_copy(&l.p0, &p[0]);
        point_copy(&l.p1, &p[1]);
        line_draw(&l, src, c);
        point_copy(&l.p0, &p[1]);
        point_copy(&l.p1, &p[2]);
        line_draw(&l, src, c);
        point_copy(&l.p0, &p[2]);
        point_copy(&l.p1, &p[3]);
        line_draw(&l, src, c);
        return;
    }

    // De Casteljau's algorithm 
    Point q0, q1, q2, q3;
    Point r0, r1, r2, r3;
    Point s0;

    // first half
    q0 = p[0];
    q1.x = (p[0].x + p[1].x) / 2.0;
    q1.y = (p[0].y + p[1].y) / 2.0;
    q1.z = (p[0].z + p[1].z) / 2.0;
    q2.x = (p[0].x + 2 * p[1].x + p[2].x) / 4.0;
    q2.y = (p[0].y + 2 * p[1].y + p[2].y) / 4.0;
    q2.z = (p[0].z + 2 * p[1].z + p[2].z) / 4.0;
    q3.x = (p[0].x + 3 * p[1].x + 3 * p[2].x + p[3].x) / 8.0;
    q3.y = (p[0].y + 3 * p[1].y + 3 * p[2].y + p[3].y) / 8.0;
    q3.z = (p[0].z + 3 * p[1].z + 3 * p[2].z + p[3].z) / 8.0;

    // second half
    r0 = q3;
    r1.x = (p[1].x + 2 * p[2].x + p[3].x) / 4.0;
    r1.y = (p[1].y + 2 * p[2].y + p[3].y) / 4.0;
    r1.z = (p[1].z + 2 * p[2].z + p[3].z) / 4.0;
    r2.x = (p[2].x + p[3].x) / 2.0;
    r2.y = (p[2].y + p[3].y) / 2.0;
    r2.z = (p[2].z + p[3].z) / 2.0;
    r3 = p[3];

    // middle point
    s0.x = (q3.x + r0.x) / 2.0;
    s0.y = (q3.y + r0.y) / 2.0;
    s0.z = (q3.z + r0.z) / 2.0;

    // Recursion
    Point left[4] = {q0, q1, q2, s0};
    Point right[4] = {s0, r1, r2, r3};

    drawBezierCurveRecursive(left, src, c);
    drawBezierCurveRecursive(right, src, c);
}


//Create a function bezierCurve_draw that draws a Bezier Curve into the image. This function assumes the control points are in screen coordinates. You can choose how to do this, but the easiest method is to subdivide the curve using the de Casteljau algorithm until the control points are close enough together that you can draw short lines between them to approximate the curve. The pretty way to do this is to use recursion.
// If the maximum dimension of a bounding box around the control points is less than a threshold (e.g. 10 pixels), draw the three line segments between the four control points and return.
// Otherwise, use the de Casteljau algorithm to divide the Bezier Curve into two new curves, each with four control points, and recursively call the draw function on each of the halves.
// draws the Bezier curve, given in screen coordinates, into the image using the given color. The function should be adaptive so that it uses an appropriate number of line segments to draw the curve. For example, if the bounding box of the control points is less than 10 pixels in the largest dimension, ßßßthen it is reasonable to draw the lines between the control points as an approximation to the curve.
void bezierCurve_draw(BezierCurve *b, Image *src, Color c) {
  drawBezierCurveRecursive(b->p, src, c);  
}