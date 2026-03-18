/*
Ziyan Chen
2024 Summer Term
header file of module
*/

#ifndef MODULE_H
#define MODULE_H

#include "image.h"
#include "graphics.h"
#include "curve.h"

#define MAX_LIGHTS 64

typedef enum { 
  ObjNone,
  ObjLine,
  ObjPoint,
  ObjPolyline,
  ObjPolygon,
  ObjIdentity,
  ObjMatrix,
  ObjColor,
  ObjBodyColor,
  ObjSurfaceColor,
  ObjSurfaceCoeff,
  ObjLight,
  ObjModule
} ObjectType;

typedef struct {
    ObjectType type;  
    void *obj;        
    void *next;       
} Element;

typedef struct {
    Element *head;
    Element *tail;
} Module;


typedef enum {
  ShadeFrame, //draw only the borders of objects, including polygons.
  ShadeConstant, //draw objects using the current foreground color, fill polygons. 
  ShadeDepth, //draw objects using their depth value.
  ShadeFlat, //draw objects using shading calculations, but each polygon is a constant value. 
  ShadeGouraud, //draw objects using Gouraud shading.
  ShadePhong, // draw objects using Phong shading.
} ShadeMethod;



typedef struct {
    Color color; // foreground color, used in the default drawing mode.
    Color flatColor; // color to flat-fill a polygon based on a shading calculation.
    Color body;
    Color surface;
    double surfaceCoeff;
    ShadeMethod shade;
    int zBufferFlag;
    View3D viewer;
} DrawState;

typedef enum {
  LightNone,
  LightAmbient,
  LightDirect,
  LightPoint,
  LightSpot,
} LightType;

typedef struct {
  LightType type;
  Color color;
  Vector direction;
  Point position;
  float cutoff; // stores the cosine of the cutoff angle of a spotlight
  float sharpness; // coefficient of the falloff function (power for cosine)
} Light;

typedef struct {
  int nLights;
  Light light[MAX_LIGHTS];
} Lighting;

//2D module functions
Element *element_create(void);

Element *element_init(ObjectType type, void *obj);

void element_delete(Element *e);

Module *module_create(void);

void module_add(Module *md, Element *e);

void module_clear(Module *md);

void module_color(Module *md, Color *c);

void module_surfaceColor(Module *md, Color *Cs);

void module_bodyColor(Module *md, Color *Cb);

void module_surfaceCoeff(Module *md, float sCoeff);

void module_delete(Module *md);

void module_insert(Module *md, Element *e);

void module_module(Module *md, Module *sub);

void module_point(Module *md, Point *p);

void module_line(Module *md, Line *p);

void module_polyline(Module *md, Polyline *p);

void module_polygon(Module *md, Polygon *p);

void module_bezierCurve(Module *m, BezierCurve *b, int divisions);

void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);

void module_cylinder( Module *mod, int sides );

void module_pyramid(Module *md);

void module_prism(Module *md);

void module_tetrahedron(Module *md);

void module_identity(Module *md);

void module_translate2D(Module *md, double tx, double ty);

void module_scale2D(Module *md, double sx, double sy);

void module_rotateZ(Module *md, double cth, double sth);

void module_shear2D(Module *md, double shx, double shy);

void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);

//3D module functions
void module_translate(Module *md, double tx, double ty, double tz);

void module_scale(Module *md, double sx, double sy, double sz);

void module_rotateX(Module *md, double cth, double sth);

void module_rotateY(Module *md, double cth, double sth);

void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

void module_cube(Module *md, int solid);

// Draw state functions
DrawState *drawstate_create( void );

void drawstate_init( DrawState *s );
// initialize the DrawState to default values

void drawstate_setColor( DrawState *ds, Color *color );

void drawstate_setBody( DrawState *ds, Color *color );

void drawstate_setSurface( DrawState *ds, Color *color );

void drawstate_setSurfaceCoeff( DrawState *ds, double coeff );

void drawstate_setShade( DrawState *ds, ShadeMethod s ); 
// set the shading method to s.
void drawstate_setViewer( DrawState *s, Point *v ); 
// set the DrawState view to v.

void drawstate_copy( DrawState *to, DrawState *from );

// Light functions
// initialize the light to default values.
void light_init( Light *light );

// copy the light information.
void light_copy( Light *to, Light *from );

// Lighting functions

//allocate and return a new lighting structure set to default values.
Lighting *lighting_create( void );

// delete the Lighting struct.
void lighting_delete( Lighting *lights );

// initialize the lighting structure to default values.
void lighting_init( Lighting *l );

// reset the Lighting struct to 0 lights.
void lighting_clear( Lighting *l );

// add a new light to the Lighting structure given the parameters, some of which may be NULL, depending upon the type. Make sure you don’t add more lights than MAX LIGHTS.
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness );

// calculate the proper color given the normal N, view vector V, 3D point P, body color Cb, surface color Cs, sharpness value s, the lighting, and whether the polygon is one-sided or two-sided. Put the result in the Color c.
void lighting_shading( Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c );

// Module Lighting
// add the Light struct to the module.
void module_addLight( Module *m, Light *light );

// traverse the module and all sub-modules, keep track of the LTM and GTM and apply all ObjMatrix and ObjIdentity elements. Recursively traverse all ObjModule elements.
void module_parseLighting( Module *m, Matrix *GTM, Lighting *lighting );

#endif