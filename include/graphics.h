/*
Ziyan Chen
2024 Summer Term
header file of graphic primitives
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"



typedef struct {
  float x;
  float y;
  float z;
  float h;
} Point;

typedef Point Vector;

typedef struct {
  float r;
  float g;
  float b;
} Color;

#include "curve.h"

typedef struct {
    int oneSided;
    int nVertex;
    Point *vertex;
    Color color;
    int zBuffer;
    Color *colors;  // for Gouraud shading
    Vector *normals;  // for Gouraud shading
} Polygon;

typedef struct {
  Point p0;
  Point p1;
  int zBuffer;
} Line;

typedef struct {
  Point c;
  int r;
} Circle;

typedef struct {
    int num_vertices;
    Point *vertex;
    int zBuffer;
} Polyline;

typedef struct {
    Point c;
    double ra;
    double rb;
} Ellipse;

typedef struct {
    double m[4][4];
} Matrix;

typedef struct {
  Point vrp;
  double du;
  double dv;
  double nx;
  double ny;
  int cols;
  int rows;
} View2D;

typedef struct {
  Point vrp;
  Vector vpn;
  Vector vup;
  double d;
  double du;
  double dv;
  double f;
  double b;
  int screenx;
  int screeny;
} View3D;

typedef struct {
    float x, y; // 2D position
    float vx, vy; // 2D velocity
    Color color; // Color of the particle
    float age; // Remaining lifetime
    float lifetime; // Maximum lifetime
} Particle;

typedef struct {
    Particle *particles; // Array of particles
    int maxParticles; // Maximum number of particles
    float particleLifetime; // Maximum lifetime of particles
    Point emitter; // Emitter position
} ParticleSystem;

void line_draw(Line *l, Image *src, Color c);

void line_set(Line *l, Point p0, Point p1);

void line_set2D(Line *l, double x0, double y0, double x1, double y1);

void line_set3D(Line *l, double x0, double y0, double z0, double x1, double y1, double z1);

void line_normalize(Line *l);

void line_copy(Line *to, Line *from);

void circle_draw(Circle *c, Image *src, Color p);

void circle_set(Circle *c, Point tc, double tr);

void point_draw(Point *p, Image *src, Color c);

void point_normalize(Point *p);

void point_set2D(Point *p, double x, double y);

void point_set3D(Point *p, double x, double y, double z);

void point_set(Point *p, double x, double y, double z, double h);

void point_print( Vector *v, FILE *fp );

void point_copy(Point *to, Point *from);

void image_setColor(Image *src, int x, int y, Color c);

void image_getColor(Image *src, int x, int y, Color *c);

void color_set(Color *c, float r, float g, float b);

void color_print( Color *c, FILE *fp );

void color_copy(Color *to, Color *from);

void color_add(Color *c, Color *add, Color *res);

void color_multiply(Color *c, Color *mul, Color *res);

void color_scale(Color *c, float scale, Color *res);

Color color_alphaBlend(Color *c, Color *foreground, Color *background, float alpha);

Polygon *polygon_create(void);

Polygon *polygon_createp(int numV, Point *vlist);

void polygon_free(Polygon *p);

void polygon_init(Polygon *p);

void polygon_set(Polygon *p, int numV, Point *vlist);

void polygon_clear(Polygon *p);

void polygon_setSided(Polygon *p, int oneSided);

void polygon_setNormals(Polygon *p, int n, Vector *normals);

void polygon_setColors(Polygon *p, Color *colors);

#include "module.h"

void polygon_shade(Polygon *p, DrawState *ds, Lighting *lighting);

void polygon_copy(Polygon *to, Polygon *from);

void polygon_print(Polygon *p, FILE *fp);

void polygon_normalize( Polygon *p );

void polygon_draw(Polygon *p, Image *src, Color c);

void polygon_drawFill(Polygon *p, Image *src, DrawState *ds, Lighting *lighting );

void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *lighting);

void polygon_drawFillB(Polygon *p, Image *src, Color color);

void polyline_init(Polyline *p);

void polyline_copy(Polyline *to, Polyline *from);

void polyline_set(Polyline *p, int num, Point *v);

void polyline_draw(Polyline *p, Image *src, Color c);

void polyline_clear(Polyline *p);

void polyline_free(Polyline *p);

Polyline* polyline_create(void);

Polyline* polyline_createp(int numV, Point *vlist);

void ellipse_set(Ellipse *e, Point tc, double ta, double tb);

void ellipse_draw(Ellipse *e, Image *src, Color p);

void line_draw_dash(Line *l, Image *src, Color c);

void circle_draw_dash(Circle *c, Image *src, Color p);

void ellipse_draw_dash(Ellipse *e, Image *src, Color p);

void matrix_print(Matrix *m, FILE *fp);

void matrix_clear(Matrix *m);

void matrix_identity(Matrix *m);

double matrix_get(Matrix *m, int r, int c);

void matrix_set(Matrix *m, int r, int c, double v);

void matrix_copy(Matrix *dest, Matrix *src);

void matrix_transpose(Matrix *m);

void matrix_multiply(Matrix *left, Matrix *right, Matrix *m);

void matrix_xformLine(Matrix *m, Line *l);

void matrix_xformPoint(Matrix *m, Point *p, Point *q);

void matrix_xformVector(Matrix *m, Vector *p, Vector *q);

void matrix_xformPolygon(Matrix *m, Polygon *p);

void matrix_xformPolyline(Matrix *m, Polyline *p);

void matrix_xformBezierCurve(Matrix *m, BezierCurve *b);

void matrix_scale2D(Matrix *m, double sx, double sy);

void matrix_shear2D(Matrix *m, double shx, double shy);

void matrix_translate2D(Matrix *m, double tx, double ty);

void matrix_translate3D(Matrix *m, double tx, double ty, double tz);

void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w);

void matrix_scale3D(Matrix *m, double sx, double sy, double sz);

void matrix_rotateX(Matrix *m, double cth, double sth);

void matrix_rotateY(Matrix *m, double cth, double sth);

void matrix_rotateZ(Matrix *m, double cth, double sth);

void matrix_setView2D(Matrix *vtm, View2D *view);

void matrix_setView3D(Matrix *vtm, View3D *view); 

void view2D_set( View2D *view, Point *vrp, double du, Vector *v, int cols, int rows );

void vector_set(Vector *v, double x, double y, double z);

void vector_print( Vector *v, FILE *fp );

double vector_length(Vector *v);

void vector_normalize(Vector *v);

void vector_copy(Vector *to, Vector *from);

void vector_add(Vector *a, Vector *b, Vector *res);

double vector_dot(Vector *a, Vector *b);

void vector_cross(Vector *a, Vector *b, Vector *c);

void vector_normal(Point *p0, Point *p1, Point *p2, Vector *n);

void init_particle(Particle *p, float originX, float originY);
void init_particle_system(ParticleSystem *ps, int maxParticles, float particleLifetime, Point emitter);
void update_particles(ParticleSystem *ps, float deltaTime);
void render_particles(ParticleSystem *ps, Image *image, int width, int height);
void spawn_particle(Particle *p, Point emitterPosition, float speed, Color color, float lifetime);
void init_explosion(ParticleSystem *ps, Point emitter);

void init_fire(ParticleSystem *ps, Point emitter);

void init_exhaust(ParticleSystem *ps, Point emitter);

void init_smoke(ParticleSystem *ps, Point emitter);

#endif
