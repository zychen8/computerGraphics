/*
Ziyan Chen
2024 Summer Term

module library for hierachical modeling
*/

#include "image.h"
#include "graphics.h"
#include "module.h"
#include "curve.h"


//2D module functions
// Create a new element and initialize the fields.
Element *element_create(void) {
    Element *e = malloc(sizeof(Element));
    if (e == NULL) {
        return NULL;
    }
    e->type = ObjNone;
    e->obj = NULL;
    e->next = NULL;
    return e;
}

// Initialize the fields of the element.
Element *element_init(ObjectType type, void *obj) {
    Element *e = malloc(sizeof(Element));
    if (e == NULL) {
        return NULL;
    }
    e->type = type;
    e->obj = obj;
    e->next = NULL;
    return e;
}

// Free the memory associated with the element.
void element_delete(Element *e) {
    if (e == NULL) {
        return;
    }
    free(e);
}

// Create a new module and initialize the fields.
Module *module_create(void) {
    Module *md = malloc(sizeof(Module));
    if (md == NULL) {
        return NULL;
    }
    md->head = NULL;
    md->tail = NULL;
    return md;
}

// Add e to the tail of the module’s list.
void module_add(Module *md, Element *e) {
    if (md == NULL || e == NULL) return;
    if (md->head == NULL) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e;
        md->tail = e;
    }

}

void module_surfaceColor(Module *md, Color *Cs) {
    if (md == NULL || Cs == NULL) return;
    Element *e = element_init(ObjSurfaceColor, Cs);
    module_insert(md, e);
}

void module_bodyColor(Module *md, Color *Cb) {
    if (md == NULL || Cb == NULL) return;
    Element *e = element_init(ObjBodyColor, Cb);
    module_insert(md, e);
}

void module_surfaceCoeff(Module *md, float sCoeff) {
    if (md == NULL) return;
    Element *e = element_init(ObjSurfaceCoeff, &sCoeff);
    module_insert(md, e);
}

// Free the memory associated with the module.
void module_delete(Module *md) {
    module_clear(md);
    free(md);
}

// Set the current color to c.
void module_color(Module *md, Color *c) {
    Element *e = element_init(ObjColor, c);
    module_insert(md, e);
}

// Clear the module’s list of Elements.
void module_clear(Module *md) {
    Element *e = md->head;
    while (e != NULL) {
        Element *temp = e;
        e = e->next;
        element_delete(temp);
    }
    md->head = NULL;
    md->tail = NULL;
}

// Insert e into the module at the tail.
void module_insert(Module *md, Element *e) {
    if (md->head == NULL) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e;
        md->tail = e;
    }
}

// Adds sub to the tail of the module’s list.
void module_module(Module *md, Module *sub) {
    Element *e = element_create();
    e->type = ObjModule;
    e->obj = sub;
    module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_point(Module *md, Point *p) {
    Point *copy = (Point *)malloc(sizeof(Point));
    if (copy == NULL) {
        fprintf(stderr, "module_point: malloc failed\n");
        exit(1);
    }
    point_copy(copy, p);
    copy->z = 1;
    copy->h = 1;
    Element *e = element_init(ObjPoint, copy);
    module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_line(Module *md, Line *p) {
    Line *copy = (Line *)malloc(sizeof(Line));
    if (copy == NULL) {
        fprintf(stderr, "module_line: malloc failed\n");
        exit(1);
    }
    line_copy(copy, p);
    Element *e = element_init(ObjLine, copy);
    module_insert(md, e);
    
}



// Adds p to the tail of the module’s list.
void module_polyline(Module *md, Polyline *p) {
    Polyline *copy = polyline_create();
    if (copy == NULL) {
        fprintf(stderr, "module_polyline: malloc failed\n");
        exit(1);
    }
    polyline_copy(copy, p);
    Element *e = element_init(ObjPolyline, copy);
    module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_polygon(Module *md, Polygon *p) {
    Polygon *copy = polygon_create();
    if (copy == NULL) {
        fprintf(stderr, "module_polygon: malloc failed\n");
        exit(1);
    }
    polygon_copy(copy, p);
    Element *e = element_init(ObjPolygon, copy);
    module_insert(md, e);
}

// Function to subdivide a Bezier curve
static void bezierCurve_subdivide(Module *m, Point *p, int divisions) {
    if (divisions == 0) {
        // base case
        Line l;
        point_set3D(&l.p0, p[0].x, p[0].y, p[0].z);
        point_set3D(&l.p1, p[1].x, p[1].y, p[1].z);
        module_line(m, &l);
        point_set3D(&l.p0, p[1].x, p[1].y, p[1].z);
        point_set3D(&l.p1, p[2].x, p[2].y, p[2].z);
        module_line(m, &l);
        point_set3D(&l.p0, p[2].x, p[2].y, p[2].z);
        point_set3D(&l.p1, p[3].x, p[3].y, p[3].z);
        module_line(m, &l);
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

    bezierCurve_subdivide(m, left, divisions - 1);
    bezierCurve_subdivide(m, right, divisions - 1);
}

// use the de Casteljau algorithm to subdivide the Bezier curve divisions times, then add the lines connecting the control points to the module. For example, if divisions is 1, the four original Bezier curve control points will be used to generate eight control points and two new Bezier curves. Then the algorithm will add six lines to the module, three for each of the smaller Bezier curves.
void module_bezierCurve(Module *md, BezierCurve *b, int divisions) {
    bezierCurve_subdivide(md, b->p, divisions);
}


// Function to subdivide a Bezier surface
static void bezierSurface_subdivide(Module *md, Point *p, int divisions, int solid) {
    if (divisions == 0) {
        if (solid == 0) {
            // Base case: draw the 4x4 grid
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                    Line l;
                    point_set3D(&l.p0, p[i * 4 + j].x, p[i * 4 + j].y, p[i * 4 + j].z);
                    point_set3D(&l.p1, p[i * 4 + j + 1].x, p[i * 4 + j + 1].y, p[i * 4 + j + 1].z);
                    module_line(md, &l);

                    point_set3D(&l.p0, p[j * 4 + i].x, p[j * 4 + i].y, p[j * 4 + i].z);
                    point_set3D(&l.p1, p[(j + 1) * 4 + i].x, p[(j + 1) * 4 + i].y, p[(j + 1) * 4 + i].z);
                    module_line(md, &l);
                }
            }
        } else {
            Polygon poly;
            Point pt[4];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    point_set3D(&pt[0], p[i * 4 + j].x, p[i * 4 + j].y, p[i * 4 + j].z);
                    point_set3D(&pt[1], p[i * 4 + j + 1].x, p[i * 4 + j + 1].y, p[i * 4 + j + 1].z);
                    point_set3D(&pt[2], p[(i + 1) * 4 + j + 1].x, p[(i + 1) * 4 + j + 1].y, p[(i + 1) * 4 + j + 1].z);
                    polygon_set(&poly, 3, pt);
                    module_polygon(md, &poly);

                    point_set3D(&pt[0], p[i * 4 + j].x, p[i * 4 + j].y, p[i * 4 + j].z);
                    point_set3D(&pt[1], p[(i + 1) * 4 + j].x, p[(i + 1) * 4 + j].y, p[(i + 1) * 4 + j].z);
                    point_set3D(&pt[2], p[(i + 1) * 4 + j + 1].x, p[(i + 1) * 4 + j + 1].y, p[(i + 1) * 4 + j + 1].z);

                    polygon_set(&poly, 3, pt);
                    module_polygon(md, &poly);
                }
            }

        }
        return;
    }

    // Recursive subdivision
    // Allocate memory for new control points
    BezierSurface q[4];
    Point vertical[4][8];
    Point horizontal[8][8];

    // Subdivide into 4 parts vertically
    for (int i = 0; i < 4; i++) {
        Point row[4];
        row[0] = p[4*i];
        row[1] = p[4*i + 1];
        row[2] = p[4*i + 2];
        row[3] = p[4*i + 3];

        Point q0, q1, q2, q3;
        Point r0, r1, r2, r3;
        Point s0;

        // first half
        q0 = row[0];
        q1.x = (row[0].x + row[1].x) / 2.0;
        q1.y = (row[0].y + row[1].y) / 2.0;
        q1.z = (row[0].z + row[1].z) / 2.0;
        q2.x = (row[0].x + 2 * row[1].x + row[2].x) / 4.0;
        q2.y = (row[0].y + 2 * row[1].y + row[2].y) / 4.0;
        q2.z = (row[0].z + 2 * row[1].z + row[2].z) / 4.0;
        q3.x = (row[0].x + 3 * row[1].x + 3 * row[2].x + row[3].x) / 8.0;
        q3.y = (row[0].y + 3 * row[1].y + 3 * row[2].y + row[3].y) / 8.0;
        q3.z = (row[0].z + 3 * row[1].z + 3 * row[2].z + row[3].z) / 8.0;

        // Second half
        r0 = q3;
        r1.x = (row[1].x + 2 * row[2].x + row[3].x) / 4.0;
        r1.y = (row[1].y + 2 * row[2].y + row[3].y) / 4.0;
        r1.z = (row[1].z + 2 * row[2].z + row[3].z) / 4.0;
        r2.x = (row[2].x + row[3].x) / 2.0;
        r2.y = (row[2].y + row[3].y) / 2.0;
        r2.z = (row[2].z + row[3].z) / 2.0;
        r3 = row[3];

        // middle point
        s0.x = (q3.x + r0.x) / 2.0;
        s0.y = (q3.y + r0.y) / 2.0;
        s0.z = (q3.z + r0.z) / 2.0;

        vertical[i][0] = q0;
        vertical[i][1] = q1;
        vertical[i][2] = q2;
        vertical[i][3] = s0;
        vertical[i][4] = s0;
        vertical[i][5] = r1;
        vertical[i][6] = r2;
        vertical[i][7] = r3;
    }

    // Subdivide into 4 parts vertically
    for (int j = 0; j < 8; j++) {
        Point column[4];
        column[0] = vertical[0][j];
        column[1] = vertical[1][j];
        column[2] = vertical[2][j];
        column[3] = vertical[3][j];

        Point q0, q1, q2, q3;
        Point r0, r1, r2, r3;
        Point s0;

        // first half
        q0 = column[0];
        q1.x = (column[0].x + column[1].x) / 2.0;
        q1.y = (column[0].y + column[1].y) / 2.0;
        q1.z = (column[0].z + column[1].z) / 2.0;
        q2.x = (column[0].x + 2 * column[1].x + column[2].x) / 4.0;
        q2.y = (column[0].y + 2 * column[1].y + column[2].y) / 4.0;
        q2.z = (column[0].z + 2 * column[1].z + column[2].z) / 4.0;
        q3.x = (column[0].x + 3 * column[1].x + 3 * column[2].x + column[3].x) / 8.0;
        q3.y = (column[0].y + 3 * column[1].y + 3 * column[2].y + column[3].y) / 8.0;
        q3.z = (column[0].z + 3 * column[1].z + 3 * column[2].z + column[3].z) / 8.0;

        // Second half
        r0 = q3;
        r1.x = (column[1].x + 2 * column[2].x + column[3].x) / 4.0;
        r1.y = (column[1].y + 2 * column[2].y + column[3].y) / 4.0;
        r1.z = (column[1].z + 2 * column[2].z + column[3].z) / 4.0;
        r2.x = (column[2].x + column[3].x) / 2.0;
        r2.y = (column[2].y + column[3].y) / 2.0;
        r2.z = (column[2].z + column[3].z) / 2.0;
        r3 = column[3];

        // middle point
        s0.x = (q3.x + r0.x) / 2.0;
        s0.y = (q3.y + r0.y) / 2.0;
        s0.z = (q3.z + r0.z) / 2.0;

        horizontal[0][j] = q0;
        horizontal[1][j] = q1;
        horizontal[2][j] = q2;
        horizontal[3][j] = s0;
        horizontal[4][j] = s0;
        horizontal[5][j] = r1;
        horizontal[6][j] = r2;
        horizontal[7][j] = r3;
    }

    // assign control points to q
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            q[0].p[i*4 + j] = horizontal[i][j];
            q[1].p[i*4 + j] = horizontal[i][j + 4];
            q[2].p[i*4 + j] = horizontal[i + 4][j];
            q[3].p[i*4 + j] = horizontal[i + 4][j + 4];
        }
    }
    // Recursively subdivide each sub-surface
    for (int i = 0; i < 4; i++) {
        bezierSurface_subdivide(md, q[i].p, divisions - 1, solid);
    }
}

// use the de Casteljau algorithm to subdivide the Bezier surface divisions times
void module_bezierSurface(Module *md, BezierSurface *b, int divisions, int solid) {
    bezierSurface_subdivide(md, b->p, divisions, solid);
}

void module_cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];
    Vector n[4];
    int j;

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    for(j=0;j<3;j++)
	    vector_set( &(n[j]), 0, 1, 0 );
    polygon_setNormals( &p, 3, n );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    for(j=0;j<3;j++)
	    vector_set( &(n[j]), 0, -1, 0 );
    polygon_setNormals( &p, 3, n );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );

    vector_set( &n[0], x1, 0.0, z1 );
    vector_set( &n[1], x2, 0.0, z2 );
    vector_set( &n[2], x2, 0.0, z2 );
    vector_set( &n[3], x1, 0.0, z1 );

    polygon_set( &p, 4, pt );
    polygon_setNormals( &p, 4, n );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// Function to draw a prism
void module_prism(Module *mod) {
    Polygon p;
    Point vertices[6];
    Point pt[4];
    
    polygon_init(&p);
    
    // Define vertices
    // Assuming a regular tetrahedron centered at the origin
    point_set3D(&vertices[0],  0.0, -1.0, -1.5);
    point_set3D(&vertices[1], -1.0, -1.0,  1.0);
    point_set3D(&vertices[2],  1.0, -1.0,  1.0);
    point_set3D(&vertices[3],  0.0,  1.0, -1.5);
    point_set3D(&vertices[4], -1.0,  1.0,  1.0);
    point_set3D(&vertices[5],  1.0,  1.0,  1.0);
    

    // top and bottom faces
    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[1]);
    point_copy(&pt[2], &vertices[2]);
    polygon_set(&p, 3, pt);
    module_polygon(mod, &p);

    point_copy(&pt[0], &vertices[3]);
    point_copy(&pt[1], &vertices[4]);
    point_copy(&pt[2], &vertices[5]);
    polygon_set(&p, 3, pt);
    module_polygon(mod, &p);
    
    // side faces
    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[1]);
    point_copy(&pt[2], &vertices[4]);
    point_copy(&pt[3], &vertices[3]);
    polygon_set(&p, 4, pt);
    module_polygon(mod, &p);
    
    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[2]);
    point_copy(&pt[2], &vertices[5]);
    point_copy(&pt[3], &vertices[3]);
    polygon_set(&p, 4, pt);
    module_polygon(mod, &p);

    point_copy(&pt[0], &vertices[1]);
    point_copy(&pt[1], &vertices[2]);
    point_copy(&pt[2], &vertices[5]);
    point_copy(&pt[3], &vertices[4]);
    polygon_set(&p, 4, pt);
    module_polygon(mod, &p);
    
    polygon_clear(&p);
}

//  Function to draw a pyramid
void module_pyramid(Module *md) {
    Polygon p;
    Point vertices[5];
    Point pt[4];
    
    polygon_init(&p);
    
    // Apex vertex
    point_set3D(&vertices[0], 0.0, 1.0, 0.0);
    // Base vertices
    point_set3D(&vertices[1], -0.5, 0.0, -0.5);
    point_set3D(&vertices[2], 0.5, 0.0, -0.5);
    point_set3D(&vertices[3], 0.5, 0.0, 0.5);
    point_set3D(&vertices[4], -0.5, 0.0, 0.5);
    
    // side faces
    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[1]);
    point_copy(&pt[2], &vertices[2]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);

    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[2]);
    point_copy(&pt[2], &vertices[3]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);

    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[4]);
    point_copy(&pt[2], &vertices[3]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);

    point_copy(&pt[0], &vertices[1]);
    point_copy(&pt[1], &vertices[4]);
    point_copy(&pt[2], &vertices[0]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);    
    
    // base face
    point_copy(&pt[0], &vertices[1]);
    point_copy(&pt[1], &vertices[2]);
    point_copy(&pt[2], &vertices[3]);
    point_copy(&pt[3], &vertices[4]);
    polygon_set(&p, 4, pt);
    module_polygon(md, &p);   
    
    polygon_clear(&p);
}

//  Function to draw a pyramid
void module_tetrahedron(Module *md) {
    Polygon p;
    Point vertices[4];
    Point pt[3];
    
    polygon_init(&p);
    
    // Apex vertex
    point_set3D(&vertices[0], 0.0, 1.0, 0.0);
    // Base vertices
    point_set3D(&vertices[1], -1, 0.0, 1.0);
    point_set3D(&vertices[2], 1, 0.0, 1.0);
    point_set3D(&vertices[3], 0, 0.0, -1.5);
    
    // side faces
    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[1]);
    point_copy(&pt[2], &vertices[2]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);

    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[2]);
    point_copy(&pt[2], &vertices[3]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);

    point_copy(&pt[0], &vertices[0]);
    point_copy(&pt[1], &vertices[1]);
    point_copy(&pt[2], &vertices[3]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);   
    
    // base face
    point_copy(&pt[0], &vertices[1]);
    point_copy(&pt[1], &vertices[2]);
    point_copy(&pt[2], &vertices[3]);
    polygon_set(&p, 3, pt);
    module_polygon(md, &p);   
    
    polygon_clear(&p);
}

// Object that sets the current transform to the identity, placed at the tail of the module’s list.
void module_identity(Module *md) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    Element *e = element_init(ObjIdentity, m);
    module_insert(md, e);
}



// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module *md, double tx, double ty) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_translate2D(m, tx, ty);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);
}

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module *md, double sx, double sy) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_scale2D(m, sx, sy);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);
}

// Matrix operand to add a rotation matrix to the tail of the module’s list.
void module_rotateZ(Module *md, double cth, double sth) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_rotateZ(m, cth, sth);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);

}

// Matrix operand to add a shear matrix to the tail of the module’s list.
void module_shear2D(Module *md, double shx, double shy) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_shear2D(m, shx, shy);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);

}

// Draw the module into the image using the given view transformation matrix [VTM], Lighting and DrawState by traversing the list of Elements. (For now, Lighting can be an empty structure.)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src) {
    if (md == NULL || VTM == NULL || GTM == NULL || ds == NULL || src == NULL) return;
    if (lighting == NULL) printf("module_draw1: lighting is NULL\n");
    Element *e = md->head;
    Matrix LTM;
    matrix_identity(&LTM);

    while (e != NULL) {
        switch (e->type) {
            case ObjColor: {
                // color_copy(&ds->color, e->obj);
                ds->color = *(Color *)e->obj;
                break;
            }
            case ObjBodyColor: {
                color_copy(&ds->body, e->obj);
                break;
            }
            case ObjSurfaceColor: {
                // ds->surface = *(Color *)e->obj;
                color_copy(&ds->surface, e->obj);
                break;
            }
            case ObjSurfaceCoeff: {
                ds->surfaceCoeff = *(float *)e->obj;
                break;
            }
            case ObjPoint: {
                // copy the point data in E to X
                Point x;
                point_copy(&x, e->obj);
                // transform X by the LTM
                matrix_xformPoint(&LTM, &x, &x);
                // transform X by the GTM
                matrix_xformPoint(GTM, &x, &x);
                // transform X by the VTM
                matrix_xformPoint(VTM, &x, &x);
                // normalize X by the homogeneous coord
                point_normalize(&x);
                // draw X  using DS->color (if X is in the image)
                point_draw(&x, src, ds->color);
                break;
            }
            case ObjLine: {
                Line l;
                line_copy(&l, e->obj);
                matrix_xformLine(&LTM, &l);
                matrix_xformLine(GTM, &l);
                matrix_xformLine(VTM, &l);
                line_normalize(&l); 
                line_draw(&l, src, ds->color);
                // printf("line drawn\n");
                break;
            }
            case ObjPolygon: {
                printf("module_draw: processing polygon\n");
                Polygon p;
                polygon_init(&p);
                polygon_copy(&p, e->obj);
                matrix_xformPolygon(&LTM, &p);
                matrix_xformPolygon(GTM, &p);
                if (lighting == NULL) printf("module_draw2: lighting is NULL\n");
                polygon_shade(&p, ds, lighting);
		// print the polygon and shading here
		polygon_print(&p, stdout);
                printf("module_draw: after shading\n");
                matrix_xformPolygon(VTM, &p);
                polygon_normalize(&p);
                polygon_drawShade(&p, src, ds, lighting);  
                polygon_clear(&p);
                printf("polygon drawn\n");
                break;
            }
            case ObjMatrix: {
                Matrix *m = (Matrix *)e->obj;
                matrix_multiply(m, &LTM, &LTM);
                break;
            }
            case ObjIdentity: {
                matrix_identity(&LTM);
                break;
            }
            case ObjModule: {
                Matrix TM;
                DrawState tempDS;
                matrix_identity(&TM);
                matrix_multiply(GTM, &LTM, &TM);
                drawstate_copy(&tempDS, ds);
                module_draw(e->obj, VTM, &TM, &tempDS, lighting, src);
                break;
            }

            default: {
                break;
            }
        }
        e = e->next;
    }
    printf("done\n");

}


//3D module functions
// Matrix operand to add a 3D translation to the Module.
void module_translate(Module *md, double tx, double ty, double tz) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_translate3D(m, tx, ty, tz);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);

}

// Matrix operand to add a 3D scale to the Module.
void module_scale(Module *md, double sx, double sy, double sz) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_scale3D(m, sx, sy, sz);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);

}

//  Matrix operand to add a rotation about the X-axis to the Module.
void module_rotateX(Module *md, double cth, double sth) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_rotateX(m, cth, sth);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);
}


// Matrix operand to add a rotation about the Y-axis to the Module.
void module_rotateY(Module *md, double cth, double sth) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_rotateY(m, cth, sth);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);

}

// Matrix operand to add a rotation that orients to the orthonormal axes ~u, ~v, w~ .
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w) {
    Matrix *m = malloc(sizeof(Matrix));
    if (m == NULL) {
        return;
    }
    matrix_identity(m);
    matrix_rotateXYZ(m, u, v, w);
    Element *e = element_init(ObjMatrix, m);
    module_insert(md, e);

}

// Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines. If solid is non-zero, use polygons. Make sure each polygon has surface normals defined for it.
void module_cube(Module *md, int solid) {
    Point pt[8];
    Polygon p;
    polygon_init(&p);
    Vector n[4];

    point_set3D(&pt[0], -0.5, -0.5, -0.5);
    point_set3D(&pt[1],  0.5, -0.5, -0.5);
    point_set3D(&pt[2],  0.5,  0.5, -0.5);
    point_set3D(&pt[3], -0.5,  0.5, -0.5);
    point_set3D(&pt[4], -0.5, -0.5,  0.5);
    point_set3D(&pt[5],  0.5, -0.5,  0.5);
    point_set3D(&pt[6],  0.5,  0.5,  0.5);
    point_set3D(&pt[7], -0.5,  0.5,  0.5);

    if (solid == 0) {
        Line line;
        
        line_set(&line, pt[0], pt[1]);
        module_line(md, &line);
        line_set(&line, pt[1], pt[2]);
        module_line(md, &line);
        line_set(&line, pt[2], pt[3]);
        module_line(md, &line);
        line_set(&line, pt[3], pt[0]);
        module_line(md, &line);
        line_set(&line, pt[4], pt[5]);
        module_line(md, &line);
        line_set(&line, pt[5], pt[6]);
        module_line(md, &line);
        line_set(&line, pt[6], pt[7]);
        module_line(md, &line);
        line_set(&line, pt[7], pt[4]);
        module_line(md, &line);
        line_set(&line, pt[0], pt[4]);
        module_line(md, &line);
        line_set(&line, pt[1], pt[5]);
        module_line(md, &line);
        line_set(&line, pt[2], pt[6]);
        module_line(md, &line);
        line_set(&line, pt[3], pt[7]);
        module_line(md, &line);
    } else {
        Point face1[] = {pt[0], pt[1], pt[2], pt[3]};
        Point face2[] = {pt[4], pt[5], pt[6], pt[7]};
        Point face3[] = {pt[0], pt[1], pt[5], pt[4]};
        Point face4[] = {pt[2], pt[3], pt[7], pt[6]};
        Point face5[] = {pt[0], pt[3], pt[7], pt[4]};
        Point face6[] = {pt[1], pt[2], pt[6], pt[5]};
        
        
        polygon_set(&p, 4, face1);
        for(int i=0;i<4;i++)
	        vector_set( &(n[i]), 0, 0, -1 );
        polygon_setNormals( &p, 4, n );
        module_polygon(md, &p);
        
        polygon_set(&p, 4, face2);
        for(int i=0;i<4;i++)
	        vector_set( &(n[i]), 0, 0, 1 );
        polygon_setNormals( &p, 4, n );        
        module_polygon(md, &p);
        
        polygon_set(&p, 4, face3);
        for(int i=0;i<4;i++)
            vector_set( &(n[i]), 0, -1, 0 );
        polygon_setNormals( &p, 4, n );
        module_polygon(md, &p);
        
        polygon_set(&p, 4, face4);
        for(int i=0;i<4;i++)
            vector_set( &(n[i]), 0, 1, 0 );
        polygon_setNormals( &p, 4, n );
        module_polygon(md, &p);
        
        polygon_set(&p, 4, face5);
        for(int i=0;i<4;i++)
            vector_set( &(n[i]), -1, 0, 0 );
        polygon_setNormals( &p, 4, n );
        module_polygon(md, &p);
        
        polygon_set(&p, 4, face6);
        for(int i=0;i<4;i++)
            vector_set( &(n[i]), 1, 0, 0 );
        polygon_setNormals( &p, 4, n );
        module_polygon(md, &p);
        /*
        polygon_init( &p );
        point_set3D( &pt[0], -0.5, -0.5, -0.5 );
        point_set3D( &pt[1], -0.5, -0.5,  0.5 );
        point_set3D( &pt[2], -0.5,  0.5,  0.5 );
        point_set3D( &pt[3], -0.5,  0.5, -0.5 );
        polygon_set( &p, 4, pt );

        module_polygon( md, &p );

        point_set3D( &pt[0], 0.5, -0.5, -0.5 );
        point_set3D( &pt[1], 0.5, -0.5,  0.5 );
        point_set3D( &pt[2], 0.5,  0.5,  0.5 );
        point_set3D( &pt[3], 0.5,  0.5, -0.5 );
        polygon_set( &p, 4, pt );
        module_polygon( md, &p );

        point_set3D( &pt[0], -0.5, -0.5, -0.5 );
        point_set3D( &pt[1], -0.5, -0.5,  0.5 );
        point_set3D( &pt[2],  0.5, -0.5,  0.5 );
        point_set3D( &pt[3],  0.5, -0.5, -0.5 );
        polygon_set( &p, 4, pt );
        module_polygon( md, &p );

        point_set3D( &pt[0], -0.5, 0.5, -0.5 );
        point_set3D( &pt[1], -0.5, 0.5,  0.5 );
        point_set3D( &pt[2],  0.5, 0.5,  0.5 );
        point_set3D( &pt[3],  0.5, 0.5, -0.5 );
        polygon_set( &p, 4, pt );
        module_polygon( md, &p );

        point_set3D( &pt[0], -0.5, -0.5, -0.5 );
        point_set3D( &pt[1], -0.5,  0.5, -0.5 );
        point_set3D( &pt[2],  0.5,  0.5, -0.5 );
        point_set3D( &pt[3],  0.5, -0.5, -0.5 );
        polygon_set( &p, 4, pt );
        module_polygon( md, &p );

        point_set3D( &pt[0], -0.5, -0.5, 0.5 );
        point_set3D( &pt[1], -0.5,  0.5, 0.5 );
        point_set3D( &pt[2],  0.5,  0.5, 0.5 );
        point_set3D( &pt[3],  0.5, -0.5, 0.5 );
        polygon_set( &p, 4, pt );
        module_polygon( md, &p );
        */
    }
}

/*
Draw state functions
*/

// create a new DrawState structure and initialize the fields.
DrawState *drawstate_create( void ) {
    DrawState *ds = malloc(sizeof(DrawState));
    if (ds == NULL) {
        return NULL;
    }
    color_set(&(ds->color), 1, 1, 1);
    color_set(&(ds->body), 1, 1, 1);
    color_set(&(ds->surface), .1, .1, .1); // BAM this really should not be all 1s
    ds->surfaceCoeff = 10; // 0; BAM setting this to zero means surface reflection is 1 (bad)
    return ds;

}

void drawstate_init( DrawState *s ) {
    if (s == NULL) {
        printf("drawstate_init: s is NULL\n");
        return;
    }
    color_set(&(s->color), 1, 1, 1);
    color_set(&(s->body), 1, 1, 1);
    color_set(&(s->surface), 1, 1, 1);
    s->surfaceCoeff = 0;
    s->shade = ShadeFlat;
    point_set3D(&(s->viewer.vrp), 0, 0, 1);
}

// set the color field to c.
void drawstate_setColor( DrawState *ds, Color *color ) {
    if (ds == NULL || color == NULL) return;
    color_set(&ds->color, color->r, color->g, color->b);

}

// set the body field to c.
void drawstate_setBody( DrawState *ds, Color *color ) {
    if (ds == NULL || color == NULL) return;
    color_set(&ds->body, color->r, color->g, color->b);
}

// set the surface field to c.
void drawstate_setSurface( DrawState *ds, Color *color ) {
    if (ds == NULL || color == NULL) return;
    color_set(&ds->surface, color->r, color->g, color->b);
}

// set the surface coefficient to coeff.
void drawstate_setSurfaceCoeff( DrawState *ds, double coeff ) {
    if (ds == NULL) return;
    ds->surfaceCoeff = coeff;
}

void drawstate_setShade( DrawState *ds, ShadeMethod s ) {
    if (ds == NULL) {
        printf("drawstate_setShade: ds is NULL\n");
        return;
    }
    ds->shade = s;
}

void drawstate_setViewer( DrawState *s, Point *v ) {
    if (s == NULL || v == NULL) {
        printf("drawstate_setViewer: s or v is NULL\n");
        return;
    }
    point_copy(&s->viewer.vrp, v);
}

// copy the DrawState data.
void drawstate_copy( DrawState *to, DrawState *from ) {
    if (to == NULL || from == NULL) return;
    // BAM you aren't copying all of the fields
    *to = *from;

    /*
    color_set(&(to->color), from->color.r, from->color.g, from->color.b);
    color_set(&(to->body), from->body.r, from->body.g, from->body.b);
    color_set(&(to->surface), from->surface.r, from->surface.g, from->surface.b);
    to->surfaceCoeff = from->surfaceCoeff;
    */
}

// initialize the light to default values.
void light_init( Light *light ) {
    if (light == NULL) return;
    light->type = LightNone;
    color_set(&(light->color), 1, 1, 1);
    point_set3D(&(light->position), 0, 0, 0);
    vector_set(&(light->direction), 0, 0, 1);
    light->cutoff = 0;
    light->sharpness = 0;
}

// copy the light information.
void light_copy( Light *to, Light *from ) {
    if (to == NULL || from == NULL) return;
    to->type = from->type;
    color_set(&to->color, from->color.r, from->color.g, from->color.b);
    point_set3D(&to->position, from->position.x, from->position.y, from->position.z);
    vector_set(&to->direction, from->direction.x, from->direction.y, from->direction.z);
    to->cutoff = from->cutoff;
    to->sharpness = from->sharpness;

}

// Lighting functions
//allocate and return a new lighting structure set to default values.
Lighting *lighting_create( void ) {
    Lighting *l = (Lighting *)malloc(sizeof(Lighting));
    if (l == NULL) {
        printf("lighting_create: malloc failed\n");
    }
    l->nLights = 0;

    return l;
}

// delete the Lighting struct.
void lighting_delete( Lighting *lights ) {
    if (lights == NULL) return;
    free(lights);

}

// initialize the lighting structure to default values.
void lighting_init( Lighting *l ) {
    if (l == NULL) printf("lighting_init: l is NULL\n");
    l->nLights = 0;
    // l->light = NULL;
}

// reset the Lighting struct to 0 lights.
void lighting_clear( Lighting *l ) {
    if (l == NULL) return;
    l->nLights = 0;
}

// add a new light to the Lighting structure given the parameters, some of which may be NULL, depending upon the type. Make sure you don’t add more lights than MAX LIGHTS.
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness ) {
    if (l == NULL || l->nLights >= 64) printf("lighting_add: l is NULL or nLights >= 64\n");

    Light *light = &(l->light[l->nLights]);
    light->type = type;

    if (c != NULL) {
        color_copy(&light->color, c);
        // light->color = *c;
    }
    if (dir != NULL) {
        vector_copy(&light->direction, dir);
    }
    if (pos != NULL) {
        point_copy(&light->position, pos);
    }
    light->cutoff = cutoff;
    light->sharpness = sharpness;
    l->nLights++;
    // prints out the sum of all the shading calculated so far
    // printf("nLights: %d\n", l->nLights);
}

// calculate the proper color given the normal N, view vector V, 3D point P, body color Cb, surface color Cs, sharpness value s, the lighting, and whether the polygon is one-sided or two-sided. Put the result in the Color c.
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c) {
  if (l == NULL) printf("l is NULL\n");
  if (N == NULL) printf("N is NULL\n");
  if (V == NULL) printf("V is NULL\n");
  if (p == NULL) printf("p is NULL\n");
  if (Cb == NULL) printf("Cb is NULL\n");
  if (Cs == NULL) printf("Cs is NULL\n");
  if (c == NULL) printf("c is NULL\n");

  // if (l == NULL || N == NULL || V == NULL || p == NULL || Cb == NULL || Cs == NULL || c == NULL) return;
  color_set(c, 0.0, 0.0, 0.0);
  Color tempColor;
  vector_normalize(N);
  vector_normalize(V);
    
  for (int i = 0; i < l->nLights; i++) {
    Light *light = &l->light[i];
    Color lightColor;
    color_copy(&lightColor, &light->color);
    if (light->type == LightAmbient) {
      color_multiply(&lightColor, Cb, &tempColor);
      color_add(c, &tempColor, c);
    } else {
      Vector L;
      if (light->type == LightDirect) {
        vector_copy(&L, &light->direction);
        vector_normalize(&L);
      } else {
        vector_set(&L, light->position.x - p->x, light->position.y - p->y, light->position.z - p->z);
        vector_normalize(&L);
      }
      
      float theta = vector_dot(&L, N);
      printf("L*N %.2f\n", theta);
      
      if (oneSided && theta < 0) {
        color_multiply(&lightColor, Cb, &tempColor);
        color_scale(&tempColor, -theta, &tempColor);
        color_add(c, &tempColor, c);
        continue;
      } else { // BAM Why have the else here?
            float sigma = vector_dot(V, N);
            if ((theta > 0 && sigma < 0) || (theta < 0 && sigma > 0)) {
                continue;
            } else {
                Vector H;
                vector_add(&L, V, &H);
                vector_normalize(&H);

                float beta = vector_dot(&H, N);
                if (theta < 0 && !oneSided) {
                    theta = -theta;
                    beta = -beta;
                }

                color_multiply(&lightColor, Cb, &tempColor);
                color_scale(&tempColor, theta, &tempColor);
                color_add(c, &tempColor, c);
                float sR = pow(beta, s);
                printf("H: ");
                vector_print( &H, stdout);
                printf("L: ");
                vector_print( &L, stdout);
                printf("V: ");
                vector_print( V, stdout);
                printf("N: ");
                vector_print( N, stdout);
                printf("L*N %.2f beta %.2f sR %.2f s %.2f\n", theta, beta, sR, s);
                color_multiply(&lightColor, Cs, &tempColor);
                color_scale(&tempColor, sR, &tempColor);
                color_add(c, &tempColor, c); 
            }

      }

      /* 
	 printf("Light: %d Type: %d, theta: %f, beta: %f\n", i, light->type, theta, beta);
	 printf("Diffuse color: %f %f %f\n", tempColor.r, tempColor.g, tempColor.b);
	 printf("Accumulated color: %f %f %f\n", c->r, c->g, c->b);
           
	 float NL = vector_dot(N, &L);
	 if (NL > 0 || (!oneSided && NL < 0)) {
	 float VN = vector_dot(V, N);
	 if ((VN > 0 && NL > 0) || (VN < 0 && NL < 0)) {
	 Vector H;
	 vector_add(&L, V, &H);
	 vector_normalize(&H);
	 float NH = vector_dot(N, &H);
                    
	 if (NL < 0) {
	 NL = -NL;
	 NH = -NH;
	 }
	 color_multiply(&lightColor, Cb, &tempColor);
	 color_scale(&tempColor, NL, &tempColor);
	 color_add(c, &tempColor, c);

	 float sR = pow(NH, s);
	 color_multiply(&lightColor, Cs, &tempColor);
	 color_scale(&tempColor, sR, &tempColor);
	 color_add(c, &tempColor, c);
	 }
                
	 }
      */ 
    }
  }
  printf("color: ");
  color_print( c, stdout);
  
  // printf("shading done\n");
}

// Module Lighting
// add the Light struct to the module. Be sure to copy the information in the Light struct.
void module_addLight( Module *m, Light *light ) {
    if (m == NULL || light == NULL) return;
    Element *e = element_init(ObjLight, light);
    module_insert(m, e);
}

// traverse the module and all sub-modules, keep track of the LTM and GTM and apply all ObjMatrix and ObjIdentity elements. Recursively traverse all ObjModule elements.
void module_parseLighting( Module *m, Matrix *GTM, Lighting *lighting ) {
    if (m == NULL || GTM == NULL || lighting == NULL) return;
    Element *e = m->head;
    Matrix LTM;
    matrix_identity(&LTM);
    while (e != NULL) {
        switch (e->type) {
            case ObjLight: {
                Light *light = (Light *)e->obj;
                Light *newLight = malloc(sizeof(Light));
                if (newLight == NULL) {
                    return;
                }
                light_copy(newLight, light);
                lighting_add(lighting, newLight->type, &newLight->color, &newLight->direction, &newLight->position, newLight->cutoff, newLight->sharpness);
                break;
            }
            case ObjMatrix: {
                Matrix *m = (Matrix *)e->obj;
                matrix_multiply(m, &LTM, &LTM);
                break;
            }
            case ObjIdentity: {
                matrix_identity(&LTM);
                break;
            }
            case ObjModule: {
                module_parseLighting(e->obj, &LTM, lighting);
                break;
            }
            default: {
                break;
            }
        }
        e = e->next;
    }
}
