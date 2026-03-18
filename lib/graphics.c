/*
Ziyan Chen
2024 Summer Term

library file of graphic primitives and functions including line, circle, ellipse, point, polyline
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include "image.h"
#include "graphics.h"
// #include "list.h"
// #include "curve.h"

#define USECPP 0

// draw a line with Brsenham's algorithm
void line_draw(Line *l, Image *src, Color c) {
    int x0 = l->p0.x;
    int y0 = l->p0.y;
    int x1 = l->p1.x;
    int y1 = l->p1.y;
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    
    if (dx >= dy) {
        int err = 3 * dy - 2 * dx;
        while (x0 != x1) {
            image_setColor(src, y0, x0, c);
            if (err > 0) {
                err -= 2 * dx;
                y0 += sy;
            }
            err += 2 * dy;
            x0 += sx;
        }
    } else {
        int err = 3 * dx - 2 * dy;
        while (y0 != y1) {
            image_setColor(src, y0, x0, c);
            if (err > 0) {
                err -= 2 * dy;
                x0 += sx;
            }
            err += 2 * dx;
            y0 += sy;
        }
    }
    image_setColor(src, y1, x1, c);
}


static void line_drawZ(Line *l, Image *src, Color c) {
    int x0 = l->p0.x;
    int y0 = l->p0.y;
    int x1 = l->p1.x;
    int y1 = l->p1.y;
    float z0 = l->p0.z;
    float z1 = l->p1.z;
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    
    float dz;
    float z;
    if (z0 == 0) z = 1;
    else z = 1.0 / z0;

    if (dx >= dy) {
        int err = 3 * dy - 2 * dx;
        dz = (1/z1 - 1/z0) / dx;
        while (x0 != x1) {
            if (x0 >= 0 && y0 >= 0 && x0 < src->cols && y0 < src->rows) {
                if (z > src->data[y0][x0].z) {
                    src->data[y0][x0].z = z;
                    image_setColor(src, y0, x0, c);
                }
            }
            if (err > 0) {
                err -= 2 * dx;
                y0 += sy;
            }
            err += 2 * dy;
            x0 += sx;
            z += dz;
        }
    } else {
        dz = (1/z1 - 1/z0) / dy;
        int err = 3 * dx - 2 * dy;
        while (y0 != y1) {
            if (x0 >= 0 && y0 >= 0 && x0 < src->cols && y0 < src->rows) {
                if (z > src->data[y0][x0].z) {
                    src->data[y0][x0].z = z;
                    image_setColor(src, y0, x0, c);
                }
            }
            if (err > 0) {
                err -= 2 * dy;
                x0 += sx;
            }
            err += 2 * dx;
            y0 += sy;
            z += dz;
        }
    }
    image_setColor(src, y1, x1, c);
}

// draw a line with Brsenham's algorithm with dash
void line_draw_dash(Line *l, Image *src, Color c) {
    int x0 = l->p0.x;
    int y0 = l->p0.y;
    int x1 = l->p1.x;
    int y1 = l->p1.y;
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int i = 0;

    if (dx >= dy) {
        int err = 3 * dy - 2 * dx;
        while (x0 != x1) {
            if (i % 5 != 0 && i % 5 != 1) {
                image_setColor(src, y0, x0, c);
            }
            i += 1;
            if (err > 0) {
                err -= 2 * dx;
                y0 += sy;
            }
            err += 2 * dy;
            x0 += sx;
        }
    } else {
        int err = 3 * dx - 2 * dy;
        while (y0 != y1) {
            if (i % 5 != 0 && i % 5 != 1) {
                image_setColor(src, y0, x0, c);
            }
            i += 1;
            if (err > 0) {
                err -= 2 * dy;
                x0 += sx;
            }
            err += 2 * dx;
            y0 += sy;
        }
    }
    image_setColor(src, y1, x1, c);
}

void line_set(Line *l, Point p0, Point p1) {
    l->p0.x = p0.x;
    l->p0.y = p0.y;
    l->p0.z = p0.z;
    l->p0.h = p0.h;
    l->p1.x = p1.x;
    l->p1.y = p1.y;
    l->p1.z = p1.z;
    l->p1.h = p1.h;
}


// set the line in 2D
void line_set2D(Line *l, double x0, double y0, double x1, double y1) {
    l->p0.x = x0;
    l->p0.y = y0;
    l->p1.x = x1;
    l->p1.y = y1;
    l->p0.z = 0.0;
    l->p0.h = 1.0;
    l->p1.z = 0.0;
    l->p1.h = 1.0;
}

void line_set3D(Line *l, double x0, double y0, double z0, double x1, double y1, double z1) {
    l->p0.x = x0;
    l->p0.y = y0;
    l->p0.z = z0;
    l->p0.h = 1.0;
    l->p1.x = x1;
    l->p1.y = y1;
    l->p1.z = z1;
    l->p1.h = 1.0;

}

void line_normalize(Line *l) {
    l->p0.x = l->p0.x / l->p0.h;
    l->p0.y = l->p0.y / l->p0.h;
    // l->p0.z = l->p0.z / l->p0.h;
    l->p0.h = 1.0;
    l->p1.x = l->p1.x / l->p1.h;
    l->p1.y = l->p1.y / l->p1.h;
    // l->p1.z = l->p1.z / l->p1.h;
    l->p1.h = 1.0;
}

void line_copy(Line *to, Line *from) {
    to->p0 = from->p0;
    to->p1 = from->p1;

}

// draw a circle with Bresenham's algorithm
void circle_draw(Circle *c, Image *src, Color p) {
    int x0 = c->c.x;
    int y0 = c->c.y;
    int r = c->r;
    int x = -1;
    int y = (int)(-(r + 1 / 2)); // -radius(rounded)
    int err = 1 + y;

    while (x >= y) {
        image_setColor(src, y0 + y, x0 + x, p);
        image_setColor(src, y0 + x, x0 + y, p);
        image_setColor(src, y0 + x, x0 - y, p);
        image_setColor(src, y0 + y, x0 - x, p);
        image_setColor(src, y0 - y, x0 - x, p);
        image_setColor(src, y0 - x, x0 - y, p);
        image_setColor(src, y0 - x, x0 + y, p);
        image_setColor(src, y0 - y, x0 + x, p);
        x--;
        if (err <= 0) {
            err += -2 * x + 1;
        } else {
            y += 1;
            err += -2 * (x - y) + 1;
        }
    }
}

// draw a circle with Bresenham's algorithm with dash
void circle_draw_dash(Circle *c, Image *src, Color p) {
    int x0 = c->c.x;
    int y0 = c->c.y;
    int r = c->r;
    int x = -1;
    int y = (int)(-(r + 1 / 2)); // -radius(rounded)
    int err = 1 + y;
    int i = 2;
    while (x >= y) {
        if (i % 5 != 0 && i % 5 != 1) {
            image_setColor(src, y0 + y, x0 + x, p);
            image_setColor(src, y0 + x, x0 + y, p);
            image_setColor(src, y0 + x, x0 - y, p);
            image_setColor(src, y0 + y, x0 - x, p);
            image_setColor(src, y0 - y, x0 - x, p);
            image_setColor(src, y0 - x, x0 - y, p);
            image_setColor(src, y0 - x, x0 + y, p);
            image_setColor(src, y0 - y, x0 + x, p);
        }
        i++;
        x--;
        if (err <= 0) {
            err += -2 * x + 1;
        } else {
            y += 1;
            err += -2 * (x - y) + 1;
        }
    }
}

// set the circle with center and radius
void circle_set(Circle *c, Point tc, double tr) {
    c->c = tc;
    c->r = tr;
}

// draw a point with color
void point_draw(Point *p, Image *src, Color c) {
    if (p->x >= 0 && p->x < src->rows && p->y >= 0 && p->y < src->cols) {
        image_setColor(src, p->y, p->x, c);
    }
}

// set the point with x and y
void point_set2D(Point *p, double x, double y) {
    p->x = x;
    p->y = y;
    p->z = 1;
    p->h = 1;
}

void point_set3D(Point *p, double x, double y, double z) {
    p->x = x;
    p->y = y;
    p->z = z;
    p->h = 1;
}

void point_set(Point *p, double x, double y, double z, double h) {
    p->x = x;
    p->y = y;
    p->z = z;
    p->h = h;
}

// BAM why didn't you make a point_print function for debugging?
void point_print( Vector *v, FILE *fp ) {
  fprintf(fp, "pnt: %.2f %.2f %.2f %.2f\n", v->x, v->y, v->z, v->h );
  return;
}


void point_normalize(Point *p){
    p->x = p->x / p->h;
    p->y = p->y / p->h;
    // p->z = p->z / p->h;
    p->h = 1;
}

void point_copy(Point *to, Point *from) {
    to->x = from->x;
    to->y = from->y;
    to->z = from->z;
    to->h = from->h;
}

// set position x, y in the image with color
void image_setColor(Image *src, int x, int y, Color c) {
    if (x >= 0 && x < src->rows && y >= 0 && y < src->cols) {
        src->data[x][y].r = c.r;
        src->data[x][y].g = c.g;
        src->data[x][y].b = c.b;
    }
}

void image_getColor(Image *src, int x, int y, Color *c) {
    if (x >= 0 && x < src->rows && y >= 0 && y < src->cols) {
        c->r = src->data[x][y].r;
        c->g = src->data[x][y].g;
        c->b = src->data[x][y].b;
    }
}

// 
Color color_alphaBlend(Color *c, Color *foreground, Color *background, float alpha) {
    c->r = alpha * foreground->r + (1 - alpha) * background->r;
    c->g = alpha * foreground->g + (1 - alpha) * background->g;
    c->b = alpha * foreground->b + (1 - alpha) * background->b;
    return *c;
}

// allocate polygon
Polygon *polygon_create(void) {
    Polygon *p = malloc(sizeof(Polygon));
    if (p == NULL) {
        fprintf(stderr, "polygon_create: malloc failed\n");
        exit(1);
    }
    polygon_init(p);
    return p;

}

// set the polygon with number of vertices and vertex list
Polygon *polygon_createp(int numV, Point *vlist) {
    Polygon *p = malloc(sizeof(Polygon));
    if (p == NULL) {
        fprintf(stderr, "polygon_createp: malloc failed\n");
        exit(1);
    }
    polygon_init(p);
    polygon_set(p, numV, vlist);
    return p;
}

// free the polygon and vertices
void polygon_free(Polygon *p) {
    if (p->vertex == NULL) {
        fprintf(stderr, "bad: null pointer\n");
        exit(1);
    }
    free(p->vertex);
    free(p);
}

// initialize the polygon
void polygon_init(Polygon *p) {
    p->oneSided = 1;
    p->nVertex = 0;
    p->vertex = NULL;
    p->normals = NULL;
    p->colors = NULL;
}


// set the polygon with number of vertices and vertex list
void polygon_set(Polygon *p, int numV, Point *vlist) {
    p->nVertex = numV;
    p->vertex = (Point *)malloc(numV * sizeof(Point));
    if (p->vertex == NULL) {
        fprintf(stderr, "polygon_set: malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < numV; i++) {
        p->vertex[i] = vlist[i];
    }
}

void polygon_normalize( Polygon *p ) {
    for (int i = 0; i < p->nVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}

// clear the polygon
void polygon_clear(Polygon *p) {
    if (p == NULL) {
        return;
    }
    if (p->vertex != NULL) {
        free(p->vertex);
        p->vertex = NULL;
    }
    if (p->normals != NULL) {
        free(p->normals);
        p->normals = NULL;
    }
    if (p->colors != NULL) {
        free(p->colors);
        p->colors = NULL;
    }
    p->nVertex = 0;
}

// set the polygon with oneSided
void polygon_setSided(Polygon *p, int oneSided) {
    p->oneSided = oneSided;
}

void polygon_setNormals(Polygon *p, int n, Vector *normals) {
    if (p == NULL || normals == NULL) {
        return;
    }
    if (p->normals != NULL) {
        free(p->normals);
    }
    p->normals = (Vector *)malloc(sizeof(Vector) * n);
    if (p->normals == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        p->normals[i] = normals[i];
    }
}

void polygon_setColors(Polygon *p, Color *colors) {
    if (p == NULL || colors == NULL) {
        return;
    }
    if (p->colors != NULL) {
        free(p->colors);
    }
    p->colors = (Color *)malloc(sizeof(Color) * p->nVertex);
    if (p->colors == NULL) {
        return;
    }
    for (int i = 0; i < p->nVertex; i++) {
        p->colors[i] = colors[i];
    }
}

// calculates the color at each vertex of the polygon given a DrawState and Lighting structure
void polygon_shade(Polygon *p, DrawState *ds, Lighting *lighting) {
    if (p == NULL) printf("polygon_shade: polygon is NULL\n");
    if (ds == NULL) printf("polygon_shade: drawstate is NULL\n");
    if (lighting == NULL) printf("polygon_shade: lighting is NULL\n");

    // BAM: check if p->normals is NULL
    if( p->normals == NULL ){
      printf("polygon_shade: normals don't exist\n");
      exit(-1);
    }

    if (p->colors != NULL) {
        free(p->colors);
    }

    p->colors = (Color *)malloc(sizeof(Color) * p->nVertex);
    if (p->colors == NULL) {
        printf("polygon_shade: malloc failed\n");
        return;
    }

    for (int i = 0; i < p->nVertex; i++) {
      Vector V; // BAM the V argument to lighting_shading is the view direction, not the VRP
      V.x = ds->viewer.vrp.x - p->vertex[i].x;
      V.y = ds->viewer.vrp.y - p->vertex[i].y;
      V.z = ds->viewer.vrp.z - p->vertex[i].z;
      V.h = 0.0;
      
      //      lighting_shading(lighting, &p->normals[i], &ds->viewer.vrp, &p->vertex[i], &ds->body, &ds->surface, ds->surfaceCoeff, ds->zBufferFlag, &p->colors[i]);
      // BAM the second to last argument to lighting_shading is the polygon one-sided value
      lighting_shading(lighting, &p->normals[i], &V, &p->vertex[i], &ds->body, &ds->surface, ds->surfaceCoeff, p->oneSided, &p->colors[i]);
	if( p->colors[i].r == 0.0  && 0) {
	  Vector L = { lighting->light[1].position.x - p->vertex[i].x,
	    lighting->light[1].position.y - p->vertex[i].y,
	    lighting->light[1].position.z - p->vertex[i].z,
	    0.0};
	  vector_normalize(&L);
	  
	  printf("point: %d\n", i);
	  printf("red: %f\n", p->colors[i].r);
	  printf("green: %f\n", p->colors[i].g);
	  printf("blue: %f\n", p->colors[i].b);
	  point_print( &(p->vertex[i]), stdout );
	  vector_print( &(p->normals[i]), stdout );
	  vector_print( &V, stdout );
	  vector_print( &L, stdout );
	}
    }
}



// copy the polygon from one to another
void polygon_copy(Polygon *to, Polygon *from) {
    to->oneSided = from->oneSided;
    to->nVertex = from->nVertex;
    to->vertex = (Point *)malloc(to->nVertex * sizeof(Point));

    if (to->vertex == NULL) {
        // printf("nvertex: %d\n", to->nVertex);
        fprintf(stderr, "polygon_copy: malloc failed\n");
        exit(1);
    }
    
    for (int i = 0; i < from->nVertex; i++) {
        to->vertex[i] = from->vertex[i];
    }
    if (from->normals) {
        to->normals = (Vector *)malloc(to->nVertex * sizeof(Vector));
        for (int i = 0; i < from->nVertex; i++) {
            to->normals[i] = from->normals[i];
        }
    }

    if (from->colors) {
        to->colors = (Color *)malloc(to->nVertex * sizeof(Color));
        for (int i = 0; i < from->nVertex; i++) {
            to->colors[i] = from->colors[i];
        }
    }
}


// print the polygon information
void polygon_print(Polygon *p, FILE *fp) {
    fprintf(fp, "Polygon\n oneSided: %d\n number of vertices: %d\n", p->oneSided, p->nVertex);
    for (int i = 0; i < p->nVertex; i++) {
        // BAM updated polygon_print to print the z value as well
        fprintf(fp, "  %d: %.2f %.2f %.2f\n", i, p->vertex[i].x, p->vertex[i].y, p->vertex[i].z);
    }
    if (p->normals) {
        for (int i = 0; i < p->nVertex; i++) {
        fprintf(fp, "  %d: %.2f %.2f %.2f\n", i, p->normals[i].x, p->normals[i].y, p->normals[i].z);
        }
    }
}


// draw the polygon with color and call line_draw
void polygon_draw(Polygon *p, Image *src, Color c) {
    if (p->nVertex < 2) {
        fprintf(stderr, "polygon_draw: vertices less than 3\n");
        exit(1);
    }
    for (int i = 0; i < p->nVertex - 1; i++) {
        Line l;
        line_set3D(&l, p->vertex[i].x, p->vertex[i].y, p->vertex[i].z,p->vertex[i + 1].x, p->vertex[i + 1].y, p->vertex[i + 1].z);
        line_draw(&l, src, c);
    }
    Line l;
    line_set3D(&l, p->vertex[p->nVertex - 1].x, p->vertex[p->nVertex - 1].y, p->vertex[p->nVertex - 1].z, p->vertex[0].x, p->vertex[0].y, p->vertex[0].z);
    line_draw(&l, src, c);
    printf("polygon_draw: polygon frame drawn\n");
}

/********************
Scanline Fill Algorithm
********************/

// define the struct here, because it is local to only this file
typedef struct tEdge {
	float x0, y0;                   /* start point for the edge */
	float x1, y1;                   /* end point for the edge */
	int yStart, yEnd;               /* start row and end row */
    float xIntersect, dxPerScan;    /* where the edge intersects the current scanline and how it changes */
	/* we'll add more here later */
    float zIntersect, dzPerScan;
    Color cIntersect, dcPerScan;
    Point pIntersect, dpPerScan;
    Vector nIntersect, dnPerScan;
    struct tEdge *next;
} Edge;


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
	Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	return(ea->yStart - eb->yStart);
}


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
	Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	if(ea->xIntersect < eb->xIntersect ) return(-1);
	if(ea->xIntersect > eb->xIntersect ) return(1);

	return(0);
}

/*
	Allocates, creates, fills out, and returns an Edge structure given
	the inputs.

	Current inputs are just the start and end location in image space.
	Eventually, the points will be 3D and we'll add color and texture
	coordinates.
 */
static Edge *makeEdgeRec( Point start, Point end, Color c0, Color c1, Vector n0, Vector n1, Image *src)
{
  Edge* edge;
  float dscan = end.y - start.y;

  /******
	 Your code starts here
  ******/

  // Check if the starting row is below the image or the end row is
  // above the image and skip the edge if either is true
  if (start.y >= src->rows || end.y < 0)  return NULL;

  // allocate an edge structure and set the x0, y0, x1, y1 values
  edge = (Edge*)malloc(sizeof(Edge));
  if (!edge) {
    printf("Bad bad: unable to allocate memory for edge\n");
    return NULL;
  }
  edge->x0 = start.x;
  edge->y0 = start.y;
  edge->x1 = end.x;
  edge->y1 = end.y;
  // turn on an edge only if the edge starts in the top half of it or
  // the lower half of the pixel above it.  In other words, round the
  // start y value to the nearest integer and assign it to
  // edge->yStart.
  edge->yStart = (int)(start.y + 0.5);
  edge->yEnd = (int)(end.y + 0.5) - 1;

  // Turn off the edge if it starts in the lower half or the top half
  // of the next pixel.  In other words, round the end y value to the
  // nearest integer and subtract 1, assigning the result to
  // edge->yEnd.

  // Clip yEnd to the number of rows-1.
  // if (edge->yEnd >= src->rows - 1) edge->yEnd = src->rows - 1;
	
  // Calculate the slope, dxPerScan
  // Calculate xIntersect, adjusting for the fraction of the point in the pixel.
  // Scanlines go through the middle of pixels
  // Move the edge to the first scanline it crosses
  edge->dxPerScan = (end.x - start.x) / dscan;
  edge->xIntersect = start.x + edge->dxPerScan * (edge->yStart - start.y + 0.5);

  // Calculate dzPerScan using 1/z values
  // similar to x
  edge->dzPerScan = (1.0 / end.z - 1.0/start.z) / dscan;
  edge->zIntersect = 1.0 / start.z + edge->dzPerScan * (edge->yStart - start.y + 0.5);

  // Color interpolation
  edge->dcPerScan.r = (c1.r / end.z - c0.r / start.z) / dscan;
  // edge->cIntersect.r = c0.r / start.z;
  edge->cIntersect.r = c0.r / start.z + edge->dcPerScan.r * (edge->yStart - start.y + 0.5);

  edge->dcPerScan.g = (c1.g / end.z - c0.g / start.z) / dscan;
  // edge->cIntersect.g = c0.g / start.z;
  edge->cIntersect.g = c0.g / start.z + edge->dcPerScan.g * (edge->yStart - start.y + 0.5);

  edge->dcPerScan.b = (c1.b / end.z - c0.b / start.z) / dscan;
  // edge->cIntersect.b = c0.b / start.z;
  edge->cIntersect.b = c0.b / start.z + edge->dcPerScan.b * (edge->yStart - start.y + 0.5);

  // point interpolation
  edge->dpPerScan.x = (end.x / end.z - start.x / start.z) / dscan;
  edge->pIntersect.x = start.x / start.z + edge->dpPerScan.x * (edge->yStart - start.y + 0.5);
  edge->dpPerScan.y = (end.y / end.z - start.y / start.z) / dscan;
  edge->pIntersect.y = start.y / start.z + edge->dpPerScan.y * (edge->yStart - start.y + 0.5);
  edge->dpPerScan.z = (1.0 / end.z - 1.0 / start.z) / dscan;
  edge->pIntersect.z = 1.0 / start.z + edge->dpPerScan.z * (edge->yStart - start.y + 0.5);

  // normal interpolation
  edge->dnPerScan.x = (n1.x / end.z - n0.x / start.z) / dscan;
  edge->nIntersect.x = n0.x / start.z + edge->dnPerScan.x * (edge->yStart - start.y + 0.5);
  edge->dnPerScan.y = (n1.y / end.z - n0.y / start.z) / dscan;
  edge->nIntersect.y = n0.y / start.z + edge->dnPerScan.y * (edge->yStart - start.y + 0.5);
  edge->dnPerScan.z = (n1.z / end.z - n0.z / start.z) / dscan;
  edge->nIntersect.z = n0.z / start.z + edge->dnPerScan.z * (edge->yStart - start.y + 0.5);
  // edge->dnPerScan.h = (1.0 / end.z - 1.0 / start.z) / dscan;
  // edge->nIntersect.h = 1.0 / start.z + edge->dnPerScan.h * (edge->yStart - start.y + 0.5);

  // adjust if the edge starts above the image
  // move the intersections down to scanline zero
  // if edge->y0 < 0
  //   update xIntersect
  //   update y0
  //   update x0
  //   update yStart
  // check for really bad cases with steep slopes where xIntersect has gone beyond the end of the edge

  if (edge->yStart < 0) {
    edge->xIntersect += edge->dxPerScan * -edge->yStart;
    edge->zIntersect += edge->dzPerScan * -edge->yStart;
    edge->cIntersect.r += edge->dcPerScan.r * -edge->yStart;
    edge->cIntersect.g += edge->dcPerScan.g * -edge->yStart;
    edge->cIntersect.b += edge->dcPerScan.b * -edge->yStart;
    edge->yStart = 0;
  }
    
  // return the newly created edge data structure
  return( edge );
}


/*
	Returns a list of all the edges in the polygon in sorted order by
	smallest row.
*/
static LinkedList *setupEdgeList( Polygon *p, Image *src) {
	LinkedList *edges = NULL;
	Point v1, v2;
    Color c1, c2;
    Vector n1, n2;
	int i;

    if (p->colors == NULL) {
        printf("colors are null\n");
    }

	// create a linked list
	edges = ll_new();

	// walk around the polygon, starting with the last point
	v1 = p->vertex[p->nVertex-1];
    c1 = p->colors[p->nVertex-1];
    n1 = p->normals[p->nVertex-1];

	for(i=0;i<p->nVertex;i++) {
		// printf("i value: %d\n", i);
		// the current point (i) is the end of the segment
		v2 = p->vertex[i];
        c2 = p->colors[i];
        n2 = p->normals[i];

		// if it is not a horizontal line
		if( (int)(v1.y+0.5) != (int)(v2.y+0.5) ) {
			Edge *edge;

			// if the first coordinate is smaller (top edge)
			if( v1.y < v2.y )
				edge = makeEdgeRec( v1, v2, c1, c2, n1, n2, src );
			else
				edge = makeEdgeRec( v2, v1, c2, c1, n2, n1, src );

			// insert the edge into the list of edges if it's not null
			if( edge )
				ll_insert( edges, edge, compYStart );
		}
		v1 = v2;
        c1 = c2;
        n1 = n2;
	}

	// check for empty edges (like nothing in the viewport)
	if( ll_empty( edges ) ) {
		ll_delete( edges, NULL );
		edges = NULL;
	}

	return(edges);
}

/*
	Draw one scanline of a polygon given the scanline, the active edges,
	a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan( int scan, LinkedList *active, Image *src, DrawState *ds, Lighting *lighting) {
  Edge *p1, *p2;
  int i;

	// loop over the list
  p1 = ll_head( active );
  while(p1) {
		// the edges have to come in pairs, draw from one to the next
	  p2 = ll_next( active );
	  if( !p2 ) {
		  printf("bad bad bad (your edges are not coming in pairs)\n");
		  break;
	  }

		// if the xIntersect values are the same, don't draw anything.
		// Just go to the next pair.
	  if( p2->xIntersect == p1->xIntersect ) {
		  p1 = ll_next( active );
		  continue;
	  }

        float zStart = p1->zIntersect;
        float zEnd = p2->zIntersect;
        float dzPerColumn = (zEnd - zStart) / (p2->xIntersect - p1->xIntersect);

		/**** Your code goes here ****/

        // interpolate point and normal for phong shading
        Point pStart = p1->pIntersect;
        Point pEnd = p2->pIntersect;
        Point dpPerColumn;
        dpPerColumn.x = (pEnd.x - pStart.x) / (p2->xIntersect - p1->xIntersect);
        dpPerColumn.y = (pEnd.y - pStart.y) / (p2->xIntersect - p1->xIntersect);
        dpPerColumn.z = (pEnd.z - pStart.z) / (p2->xIntersect - p1->xIntersect);

        Vector nStart = p1->nIntersect;
        Vector nEnd = p2->nIntersect;
        Vector dnPerColumn;
        dnPerColumn.x = (nEnd.x - nStart.x) / (p2->xIntersect - p1->xIntersect);
        dnPerColumn.y = (nEnd.y - nStart.y) / (p2->xIntersect - p1->xIntersect);
        dnPerColumn.z = (nEnd.z - nStart.z) / (p2->xIntersect - p1->xIntersect);


        Color cStart = p1->cIntersect;
        Color cEnd = p2->cIntersect;
        Color dcPerColumn;

        dcPerColumn.r = (cEnd.r - cStart.r) / (p2->xIntersect - p1->xIntersect);
        dcPerColumn.g = (cEnd.g - cStart.g) / (p2->xIntersect - p1->xIntersect);
        dcPerColumn.b = (cEnd.b - cStart.b) / (p2->xIntersect - p1->xIntersect);


	  // identify the starting column
	int xStart = (int)(p1->xIntersect + 0.5);
	  // clip to the left side of the image
	if (xStart < 0) {
            zStart += dzPerColumn * (-p1->xIntersect);
            xStart = 0;
            // BAM need to update cStart as well
            cStart.r += dcPerColumn.r * (-p1->xIntersect);
            cStart.g += dcPerColumn.g * (-p1->xIntersect);
            cStart.b += dcPerColumn.b * (-p1->xIntersect);
            pStart.x += dpPerColumn.x * (-p1->xIntersect);
            pStart.y += dpPerColumn.y * (-p1->xIntersect);
            pStart.z += dpPerColumn.z * (-p1->xIntersect);
            nStart.x += dnPerColumn.x * (-p1->xIntersect);
            nStart.y += dnPerColumn.y * (-p1->xIntersect);
            nStart.z += dnPerColumn.z * (-p1->xIntersect);  
            // nStart.h += dnPerColumn.h * (-p1->xIntersect);            
        }
	  // identify the ending column
		int xEnd = (int)(p2->xIntersect + 0.5);
	  // clip to the right side of the image
		if (xEnd > src->cols) xEnd = src->cols;

        float curZ = zStart;
        Color curC = cStart;
        Point curP = pStart;
        Vector curN = nStart;
		// loop from start to end and color in the pixels
	for (i = xStart; i < xEnd; i++) {
	  if (curZ > src->data[scan][i].z) {
	    src->data[scan][i].z = curZ;
	    printf("ds->shade: %d, %d\n", ds->shade, ShadeGouraud);
	    // BAM why don't you have a test here for what type of shading you're doing?
	    if (ds->shade == ShadeConstant) {
	      image_setColor(src, scan, i, ds->color);
	    } else if (ds->shade == ShadeDepth) {
	      Color color;
	      float bright = 1/curZ;
	      color_set(&color, 1-bright, 1-bright, 1-bright);
	      image_setColor(src, scan, i, color);
	      // color_set(&color, 1.4*c.r*(1-bright), 1.4*c.g*(1-bright), 1.4*c.b*(1-bright));
	    } else if (ds->shade == ShadeGouraud) {
	      Color resColor;
	      resColor.r = curC.r / curZ;
	      resColor.g = curC.g / curZ;
	      resColor.b = curC.b / curZ;
	      printf("fillscan: "); 
	      color_print(&resColor, stdout);
	      image_setColor(src, scan, i, resColor);
	    }     
	    else if(ds->shade == ShadePhong) {
	      Point resP;
	      resP.x = curP.x / curZ;
	      resP.y = curP.y / curZ;
	      resP.z = curP.z / curZ;
	      Vector resN;
	      // vector_normalize(&curN);
	      resN.x = curN.x / curZ;
	      resN.y = curN.y / curZ;
	      resN.z = curN.z / curZ;
	      vector_normalize(&resN);
	      // resN.h = curN.h / curZ;
	      Vector resVRP;
	      resVRP.x = ds->viewer.vrp.x - resP.x;
	      resVRP.y = ds->viewer.vrp.y - resP.y;
	      resVRP.z = ds->viewer.vrp.z - resP.z;
	      Color resColor;
	      lighting_shading(lighting, &resN, &resVRP, &resP, &ds->body, &ds->surface, ds->surfaceCoeff, ds->zBufferFlag, &resColor);
	      image_setColor(src, scan, i, resColor);
	    } else {
	      image_setColor(src, scan, i, ds->color);
	    }
	    // lighting_shading(lighting, &resN, &ds->viewer.vrp, &resP, &ds->body, &ds->surface, ds->surfaceCoeff, ds->zBufferFlag, &resColor);
	    // image_setColor(src, scan, i, resColor);
	    /*
                
	    */ 
	  }
	  curZ += dzPerColumn;
	  curC.r += dcPerColumn.r;
	  curC.g += dcPerColumn.g;
	  curC.b += dcPerColumn.b;
	  curP.x += dpPerColumn.x;
	  curP.y += dpPerColumn.y;
	  curP.z += dpPerColumn.z;
	  curN.x += dnPerColumn.x;
	  curN.y += dnPerColumn.y;
	  curN.z += dnPerColumn.z;
	  // curN.h += dnPerColumn.h;
	}

	// move ahead to the next pair of edges
	p1 = ll_next( active );
  }

	return;
}

/* 
	 Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList( LinkedList *edges, Image *src, DrawState *ds, Lighting *lighting) {
  LinkedList *active = NULL;
  LinkedList *tmplist = NULL;
  LinkedList *transfer = NULL;
  Edge *current;
  Edge *tedge;
  int scan = 0;

  active = ll_new( );
  tmplist = ll_new( );

  // get a pointer to the first item on the list and reset the current pointer
  current = ll_head( edges );

  // start at the first scanline and go until the active list is empty
  for(scan = current->yStart; scan < src->rows; scan++ ) {

    // grab all edges starting on this row
    while( current != NULL && current->yStart == scan ) {
      ll_insert( active, current, compXIntersect );
      current = ll_next( edges );
    }
    // current is either null, or the first edge to be handled on some future scanline

    if( ll_empty(active) ) {
      break;
    }

    // if there are active edges
    // fill out the scanline
    fillScan( scan, active, src, ds, lighting);

    // remove any ending edges and update the rest
    for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {

      // keep anything that's not ending
      if( tedge->yEnd > scan ) {

	// update the edge information with the dPerScan values
	tedge->xIntersect += tedge->dxPerScan;
	tedge->zIntersect += tedge->dzPerScan;
	
	tedge->cIntersect.r += tedge->dcPerScan.r;
	tedge->cIntersect.g += tedge->dcPerScan.g;
	tedge->cIntersect.b += tedge->dcPerScan.b;

	tedge->pIntersect.x += tedge->dpPerScan.x;
	tedge->pIntersect.y += tedge->dpPerScan.y;
	tedge->pIntersect.z += tedge->dpPerScan.z;

	tedge->nIntersect.x += tedge->dnPerScan.x;
	tedge->nIntersect.y += tedge->dnPerScan.y;
	tedge->nIntersect.z += tedge->dnPerScan.z;

	// adjust in the case of partial overlap
	if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
	  tedge->xIntersect = tedge->x1;
	}
	else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
	  tedge->xIntersect = tedge->x1;
	}

	ll_insert( tmplist, tedge, compXIntersect );
      }
    }

    transfer = active;
    active = tmplist;
    tmplist = transfer;

  }

  // get rid of the lists, but not the edge records
  ll_delete(active, NULL);
  ll_delete(tmplist, NULL);

  return(0);
}

/*
	Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, DrawState *ds, Lighting *lighting) {
	LinkedList *edges = NULL;

	// set up the edge list
	edges = setupEdgeList( p, src );
	if( !edges )
		return;

	// process the edge list (should be able to take an arbitrary edge list)
	processEdgeList( edges, src, ds, lighting);

	// clean up
	ll_delete( edges, (void (*)(const void *))free );

	return;
}

/****************************************
End Scanline Fill
*****************************************/

void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *lighting) {
    /*
    switch (ds->shade) {
        case ShadeFrame:
            polygon_draw(p, src, ds->color);
            printf("polygon_draw: polygon frame drawn\n");
            break;
        default:
            polygon_drawFill(p, src, ds, lighting);
            break;
    }     
    */
    polygon_drawFill(p, src, ds, lighting);
    printf("ds->shade: %u\n", ds->shade);
}


// helper function to compute the barycentric coordinate
static float computeBarycentric(Point c, Point a, Point b) {
    return b.x * (c.y - a.y) + b.y * (a.x - c.x) + c.x * a.y - a.x * c.y;
}

// fill the polygon using barycentric coordinate with color
void polygon_drawFillB(Polygon *p, Image *src, Color color) {
    if (p->nVertex < 3) {
        fprintf(stderr, "polygon_drawFillB: vertices less than 3\n");
        exit(1);
    }
    Point a = p->vertex[0];
    Point b = p->vertex[1];
    Point c = p->vertex[2];

    for (int y = 0; y < src->rows; y++) {
        for (int x = 0; x < src->cols; x++) {
            float cx = x + 0.5;
            float cy = y + 0.5;
            Point p = {cx, cy};
            float beta = computeBarycentric(c, a, p) / computeBarycentric(c, a, b);
            float gamma = computeBarycentric(a, b, p) / computeBarycentric(a, b, c);
            float alpha = 1 - beta - gamma;
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                image_setColor(src, y, x, color);
            }
        }
    }
}

// set the color with r, g, b channel
void color_set(Color *c, float r, float g, float b) {
    c->r = r;
    c->g = g;
    c->b = b;
}

// BAM why don't you have a color_print function?
void color_print( Color *c, FILE *fp ) {
  fprintf(fp, "%.2f %.2f %.2f\n", c->r, c->g, c->b);
}

// copy the color from one to another
void color_copy(Color *to, Color *from) {
    if (to == NULL || from == NULL) {
        printf("color_copy: null pointer\n");
        return;
    }
    to->r = from->r;
    to->g = from->g;
    to->b = from->b;
}


static float min(float a, float b) {
    return a < b ? a : b;
}

// add the color from one to another
void color_add(Color *c, Color *add, Color *res) {
    res->r = min(c->r + add->r, 1);
    res->g = min(c->g + add->g, 1);
    res->b = min(c->b + add->b, 1);
}


// subtract the color from one to another
void color_multiply(Color *c, Color *mul, Color *res) {
    res->r = min(c->r * mul->r, 1);
    res->g = min(c->g * mul->g, 1);
    res->b = min(c->b * mul->b, 1);
}


// scale the color with a float
void color_scale(Color *c, float scale, Color *res) {
    res->r = c->r * scale;
    res->g = c->g * scale;
    res->b = c->b * scale;
}

// initialize the polyline
void polyline_init(Polyline *p) {
    p->num_vertices = 0;
    p->vertex = NULL;
}

void polyline_copy(Polyline *to, Polyline *from) {
    to->num_vertices = from->num_vertices;
    to->vertex = (Point *)malloc(from->num_vertices * sizeof(Point));
    for (int i = 0; i < from->num_vertices; i++) {
        to->vertex[i] = from->vertex[i];
    }
}

// set the polyline with number of vertices and vertex list
void polyline_set(Polyline *p, int num, Point *v) {
    p->num_vertices = num;
    p->vertex = (Point *)malloc(num * sizeof(Point));
    for (int i = 0; i < num; i++) {
        p->vertex[i] = v[i];
    }
}

// draw the polyline with color and call line_draw
void polyline_draw(Polyline *p, Image *src, Color c) {
    for (int i = 0; i < p->num_vertices - 1; i++) {
        Line l;
        line_set2D(&l, p->vertex[i].x, p->vertex[i].y, p->vertex[i + 1].x, p->vertex[i + 1].y);
        line_draw(&l, src, c);
    }
}

// clear the polyline
void polyline_clear(Polyline *p) {
    p->num_vertices = 0;
    free(p->vertex);
}

// free the polyline
void polyline_free(Polyline *p) {
    free(p->vertex);
}

Polyline* polyline_create(void) {
    Polyline *p = (Polyline *)malloc(sizeof(Polyline));
    if (p == NULL) {
        fprintf(stderr, "polyline_create: malloc failed\n");
        exit(1);
    }
    polyline_init(p);
    return p;
}

// create a polyline with number of vertices and vertex list
Polyline* polyline_createp(int numV, Point *vlist) {
    Polyline *p = (Polyline *)malloc(sizeof(Polyline));
    polyline_init(p);
    polyline_set(p, numV, vlist);
    return p;
}

// set the ellipse with center, major axis, and minor axis
void ellipse_set(Ellipse* e, Point tc, double ta, double tb) {
    e->c = tc;
    e->ra = ta;
    e->rb = tb;
}

// draw the ellipse with Bresenham's algorithm
void ellipse_draw(Ellipse *e, Image *src, Color p) {
    int x0 = e->c.x;
    int y0 = e->c.y;
    int rx = e->ra;
    int ry = e->rb;
    int x = -1;
    int y = -ry;
    int px = 2 * ry * ry;
    int py = 2 * rx * rx * (-y);
    image_setColor(src, y0 + y, x0 + x, p);
    image_setColor(src, y0 - y, x0 + x, p);
    image_setColor(src, y0 - y, x0 - x, p);
    image_setColor(src, y0 + y, x0 - x, p);

    float err = ry * ry - rx * rx * ry + rx * rx / 4 + ry * ry + px;
    
    while (px < py) {
        x--;
        px = px + 2 * ry * ry;
        if (err < 0) {
            err = err + ry * ry + px;
        } else {
            y++;
            py = py - 2 * rx * rx;
            err = err + ry * ry + px - py;
        }
        image_setColor(src, y0 + y, x0 + x, p);
        image_setColor(src, y0 - y, x0 + x, p);
        image_setColor(src, y0 - y, x0 - x, p);
        image_setColor(src, y0 + y, x0 - x, p);
    }

    err = ry * ry * (x * x + x) + rx * rx * (y * y - 2 * y + 1) - rx * rx * ry * ry + rx * rx - py;

    while (y < 0) {
        y++;
        py = py - 2 * rx * rx;
        if (err > 0) {
            err = err + rx * rx - py;
        } else {
            x--;
            px = px + 2 * ry * ry;
            err = err + rx * rx - py + px;
        }
        image_setColor(src, y0 + y, x0 + x, p);
        image_setColor(src, y0 - y, x0 + x, p);
        image_setColor(src, y0 - y, x0 - x, p);
        image_setColor(src, y0 + y, x0 - x, p);
    }
}

// draw the ellipse with Bresenham's algorithm
void ellipse_draw_dash(Ellipse *e, Image *src, Color p) {
    int x0 = e->c.x;
    int y0 = e->c.y;
    int rx = e->ra;
    int ry = e->rb;
    int x = -1;
    int y = -ry;
    int px = 2 * ry * ry;
    int py = 2 * rx * rx * (-y);
    int i = 2;

    image_setColor(src, y0 + y, x0 + x, p);
    image_setColor(src, y0 - y, x0 + x, p);
    image_setColor(src, y0 - y, x0 - x, p);
    image_setColor(src, y0 + y, x0 - x, p);

    float err = ry * ry - rx * rx * ry + rx * rx / 4 + ry * ry + px;
    
    while (px < py) {
        x--;
        px = px + 2 * ry * ry;
        if (err < 0) {
            err = err + ry * ry + px;
        } else {
            y++;
            py = py - 2 * rx * rx;
            err = err + ry * ry + px - py;
        }
        if (i % 5 != 0 && i % 5 != 1) {
            image_setColor(src, y0 + y, x0 + x, p);
            image_setColor(src, y0 - y, x0 + x, p);
            image_setColor(src, y0 - y, x0 - x, p);
            image_setColor(src, y0 + y, x0 - x, p);
        }
        i++;
    }

    err = ry * ry * (x * x + x) + rx * rx * (y * y - 2 * y + 1) - rx * rx * ry * ry + rx * rx - py;

    while (y < 0) {
        y++;
        py = py - 2 * rx * rx;
        if (err > 0) {
            err = err + rx * rx - py;
        } else {
            x--;
            px = px + 2 * ry * ry;
            err = err + rx * rx - py + px;
        }
        if (i % 5 != 0 && i % 5 != 1) {
            image_setColor(src, y0 + y, x0 + x, p);
            image_setColor(src, y0 - y, x0 + x, p);
            image_setColor(src, y0 - y, x0 - x, p);
            image_setColor(src, y0 + y, x0 - x, p);
        }
        i++;
    }
}

// print the matrix information
void matrix_print(Matrix *m, FILE *fp) {
    for (int i = 0; i < 4; i++) {
        fprintf(fp, " | ");
        for (int j = 0; j < 4; j++){
            fprintf(fp, "%.3f ", m->m[i][j]);
        }
        fprintf(fp, " | \n");
    }
    fprintf(fp, "\n");
}

// clear value of the matrix
void matrix_clear(Matrix *m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            m->m[i][j] = 0;
        }
    }
}

// Set the matrix to the identity matrix.
void matrix_identity(Matrix *m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            if (i == j) m->m[i][j] = 1;
            else m->m[i][j] = 0;
        }
    }
}

// Return the element of the matrix at row r, column c.
double matrix_get(Matrix *m, int r, int c) {
    return m->m[r][c];
}

// Set the element of the matrix at row r, column c to v.
void matrix_set(Matrix *m, int r, int c, double v) {
    m->m[r][c] = v;
}

// Copy the src matrix into the dest matrix.
void matrix_copy(Matrix *dest, Matrix *src) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            dest->m[i][j] = src->m[i][j];
        }
    }
}

// Transpose the matrix m in place.
void matrix_transpose(Matrix *m) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++){
            double temp = m->m[i][j];
            m->m[i][j] = m->m[j][i];
            m->m[j][i] = temp; 
        }
    }
}

// Multiply left and right and put the result in m.
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m) {
    Matrix tempMatrix;
    matrix_clear(&tempMatrix);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                tempMatrix.m[i][j] += left->m[i][k] * right->m[k][j];
            }
        }
    }
    matrix_copy(m, &tempMatrix);
}

// Transform the points in line by the matrix m.
void matrix_xformLine(Matrix *m, Line *l) {
    Point p0, p1;
    matrix_xformPoint(m, &l->p0, &p0);
    matrix_xformPoint(m, &l->p1, &p1);
    l->p0 = p0;
    l->p1 = p1;
}

// Transform the point p by the matrix m and put the result in q. For this function, p and q need to be different variables.
void matrix_xformPoint(Matrix *m, Point *p, Point *q) {
    float temp[4];
    float point[4] = {p->x, p->y, p->z, p->h};
    for (int i = 0; i < 4; i++) {
        temp[i] = 0.0;
        for (int j = 0; j < 4; j++){
            temp[i] += m->m[i][j] * point[j];
        }
    }
    q->x = temp[0];
    q->y = temp[1];
    q->z = temp[2];
    q->h = temp[3];
}

void matrix_xformVector(Matrix *m, Vector *p, Vector *q) {
    float temp[4];
    float point[4] = {p->x, p->y, p->z, p->h};
    for (int i = 0; i < 4; i++) {
        temp[i] = 0.0;
        for (int j = 0; j < 4; j++){
            temp[i] += m->m[i][j] * point[j];
        }
    }
    q->x = temp[0];
    q->y = temp[1];
    q->z = temp[2];
    q->h = temp[3];

}

// Transform the points in polygon by the matrix m.
void matrix_xformPolygon(Matrix *m, Polygon *p) {
    for (int i = 0; i < p->nVertex; i++) {
        Point q;
        matrix_xformPoint(m, &p->vertex[i], &q);
        p->vertex[i] = q;

        if (p->normals != NULL) {
            Vector n;
            matrix_xformVector(m, &p->normals[i], &n);
            p->normals[i] = n;
        }

    }
}

void matrix_xformPolyline(Matrix *m, Polyline *p) {
    for (int i = 0; i < p->num_vertices; i++) {
        Point q;
        matrix_xformPoint(m, &p->vertex[i], &q);
        p->vertex[i] = q;
    }
}

void matrix_xformBezierCurve(Matrix *m, BezierCurve *b) {
    for (int i = 0; i < 4; i++) {
        Point q;
        matrix_xformPoint(m, &b->p[i], &q);
        b->p[i] = q;
    }
}

// Premultiply the matrix by a scale matrix parameterized by sx and sy.
void matrix_scale2D(Matrix *m, double sx, double sy) {
    Matrix scaledMatrix;
    matrix_identity(&scaledMatrix);

    scaledMatrix.m[0][0] = sx;
    scaledMatrix.m[1][1] = sy;

    matrix_multiply(&scaledMatrix, m, m);
}

void matrix_shear2D(Matrix *m, double shx, double shy) {
    Matrix shearMatrix;
    matrix_identity(&shearMatrix);

    shearMatrix.m[0][1] = shx;
    shearMatrix.m[1][0] = shy;

    matrix_multiply(&shearMatrix, m, m);

}

// Premultiply the matrix by a 2D translation matrix parameterized by tx and ty.
void matrix_translate2D(Matrix *m, double tx, double ty) {
    Matrix translateMatrix;
    matrix_identity(&translateMatrix);
    translateMatrix.m[0][3] = tx;
    translateMatrix.m[1][3] = ty;
    matrix_multiply(&translateMatrix, m, m);
}

void matrix_rotateX(Matrix *m, double cth, double sth) {
    Matrix rotateMatrix;
    matrix_identity(&rotateMatrix);
    rotateMatrix.m[1][1] = cth;
    rotateMatrix.m[1][2] = -sth;
    rotateMatrix.m[2][1] = sth;
    rotateMatrix.m[2][2] = cth;
    matrix_multiply(&rotateMatrix, m, m);

}

void matrix_rotateY(Matrix *m, double cth, double sth) {
    Matrix rotateMatrix;
    matrix_identity(&rotateMatrix);
    rotateMatrix.m[0][0] = cth;
    rotateMatrix.m[0][2] = sth;
    rotateMatrix.m[2][0] = -sth;
    rotateMatrix.m[2][2] = cth;
    matrix_multiply(&rotateMatrix, m, m);

}

// Premultiply the matrix by a Z-axis rotation matrix parameterized by cos and sin
void matrix_rotateZ(Matrix *m, double cth, double sth) {
    Matrix rotateMatrix;
    matrix_identity(&rotateMatrix);
    rotateMatrix.m[0][0] = cth;
    rotateMatrix.m[0][1] = -sth;
    rotateMatrix.m[1][0] = sth;
    rotateMatrix.m[1][1] = cth;
    matrix_multiply(&rotateMatrix, m, m);
}

// Premultiply the matrix by a 3D translation matrix parameterized by tx, ty, and tz.
void matrix_translate3D(Matrix *m, double tx, double ty, double tz) {
    Matrix translateMatrix;
    matrix_identity(&translateMatrix);
    translateMatrix.m[0][3] = tx;
    translateMatrix.m[1][3] = ty;
    translateMatrix.m[2][3] = tz;
    matrix_multiply(&translateMatrix, m, m);
}

// Premultiply the matrix by a scale matrix parameterized by sx, sy, and sz.
void matrix_scale3D(Matrix *m, double sx, double sy, double sz) {
    Matrix scaleMatrix;
    matrix_identity(&scaleMatrix);
    scaleMatrix.m[0][0] = sx;
    scaleMatrix.m[1][1] = sy;
    scaleMatrix.m[2][2] = sz;
    matrix_multiply(&scaleMatrix, m, m);
}

// Premultiply the matrix by a XYZ-axis rotation matrix parameterized by cos and sin.
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w) {
    Matrix rotateMatrix;
    matrix_identity(&rotateMatrix);
    rotateMatrix.m[0][0] = u->x;
    rotateMatrix.m[0][1] = u->y;
    rotateMatrix.m[0][2] = u->z;
    rotateMatrix.m[1][0] = v->x;
    rotateMatrix.m[1][1] = v->y;
    rotateMatrix.m[1][2] = v->z;
    rotateMatrix.m[2][0] = w->x;
    rotateMatrix.m[2][1] = w->y;
    rotateMatrix.m[2][2] = w->z;
    matrix_multiply(&rotateMatrix, m, m);
}

// Sets vtm to be the view transformation defined by the 2DView structure.
void matrix_setView2D(Matrix *vtm, View2D *view) {
    view->dv = view->du * view->rows / view->cols;
    // normalize view->nx, view->ny to length 1 before using them in matrix_rotateZ
    double length = sqrt(view->nx * view->nx + view->ny * view->ny);
    view->nx /= length;
    view->ny /= length;
    
    matrix_identity(vtm);
    Matrix translate;
    matrix_identity(&translate);
    matrix_translate2D(&translate, -view->vrp.x, -view->vrp.y);

    Matrix rotate;
    matrix_identity(&rotate);
    matrix_rotateZ(&rotate, view->nx, -view->ny);

    Matrix scale;
    matrix_identity(&scale);
    matrix_scale2D(&scale, view->cols / view->du, -view->rows / view->dv);

    Matrix translate2;
    matrix_identity(&translate2);
    matrix_translate2D(&translate2, view->cols / 2, view->rows / 2);

    matrix_multiply(&rotate, &translate, vtm);
    matrix_print(vtm, stdout);
    matrix_multiply( &scale,vtm, vtm);
    matrix_print(vtm, stdout);
    matrix_multiply(&translate2, vtm, vtm);
    matrix_print(vtm, stdout);
}

// Sets vtm to be the view transformation defined by the 3DView structure.
void matrix_setView3D(Matrix *vtm, View3D *view) {
    matrix_identity(vtm);
    vector_normalize(&view->vpn);
    //added
    // vector_normalize(&view->vup);
    // 1. claculate U as the cross product of VPN and VUP
    Vector u;
    vector_cross(&view->vup, &view->vpn, &u);
    vector_normalize(&u);

    // 2. recalculate v(VUP') as the cross product of VPN and U
    vector_cross(&view->vpn, &u, &view->vup);
    vector_normalize(&view->vup);

    // 3. translate VRP to the origin
    Matrix VTM1;
    matrix_identity(&VTM1);
    matrix_translate3D(&VTM1, -view->vrp.x, -view->vrp.y, -view->vrp.z);
    printf("VTM1 After VRP translation\n");
    matrix_print(&VTM1, stdout);

    // 4. orient view coords Rxyz
    Matrix VTM2;
    matrix_identity(&VTM2);
    matrix_rotateXYZ(&VTM2, &u, &view->vup, &view->vpn);
    matrix_multiply(&VTM2, &VTM1, &VTM2);
    printf("VTM2 After Rxyz\n");
    matrix_print(&VTM2, stdout);

    // 5. translate by d to move COP to the origin
    Matrix VTM3;
    matrix_identity(&VTM3);
    matrix_translate3D(&VTM3, 0, 0, view->d);
    matrix_multiply(&VTM3, &VTM2, &VTM3);
    // wrong: view->b += view->d;
    double B = view->b + view->d;
    printf("VTM3 After translating COP to origin\n");
    matrix_print(&VTM3, stdout);

    // 6. scale to canonical view volume
    Matrix VTM4;
    matrix_identity(&VTM4);
    matrix_scale3D(&VTM4, 2*view->d / (view->du * B), 2*view->d / (view->dv * B), 1 / B); 
    matrix_multiply(&VTM4, &VTM3, &VTM4);
    // wrong view->d /= view->b;
    double D = view->d / B;
    printf("VTM4 After scaling to CVV\n");
    matrix_print(&VTM4, stdout);

    // 7. projection using P(d') into 2D
    Matrix projection;
    matrix_identity(&projection);
    projection.m[3][3] = 0;  
    projection.m[3][2] = 1 / D;
    Matrix VTM5;
    matrix_identity(&VTM5);
    matrix_multiply(&projection, &VTM4, &VTM5);
    printf("VTM5 After perspective\n");
    matrix_print(&VTM5, stdout);

    // 8. scale shitf and scale to window coords
    Matrix scale2, translate3;
    matrix_identity(&scale2);
    matrix_identity(&translate3);
    matrix_scale2D(&scale2, -view->screenx / (2*D), -view->screeny / (2*D));
    matrix_translate2D(&translate3, view->screenx / 2, view->screeny / 2);
    printf("scale2\n");
    matrix_print(&scale2, stdout);
    printf("translate3\n");
    matrix_print(&translate3, stdout);

    Matrix VTM6;
    matrix_identity(&VTM6);
    matrix_multiply(&scale2, &VTM5, &VTM6);
    matrix_multiply(&translate3, &VTM6, &VTM6);
    printf("VTM6 After final translation to image coords\n");
    matrix_print(&VTM6, stdout);

    matrix_copy(vtm, &VTM6);
}

void view2D_set( View2D *view, Point *vrp, double du, Vector *v, int cols, int rows ) {
    view->vrp = *vrp;
    view->du = du;
    view->nx = v->x;
    view->ny = v->y;
    view->cols = cols;
    view->rows = rows;
    view->dv = view->du * view->rows / view->cols;
}

// Set the Vector to (x, y, z, 0.0).
void vector_set(Vector *v, double x, double y, double z) {
    v->x = x;
    v->y = y;
    v->z = z;
    v->h = 0.0;
}

// BAM why didn't you make a vector_print function for debugging?
void vector_print( Vector *v, FILE *fp ) {
  fprintf(fp, "vec: %.2f %.2f %.2f\n", v->x, v->y, v->z );
  return;
}

// Returns the Euclidean length of the vector. The homogeneous coordinate should be 0.
double vector_length(Vector *v) {
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

// Normalize the Vector to unit length. Do not modify the homogeneous coordinate.
void vector_normalize(Vector *v) {
    double length = vector_length(v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

void vector_add(Vector *a, Vector *b, Vector *res) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
    res->h = 0.0;
}

void vector_copy(Vector *to, Vector *from) {
    to->x = from->x;
    to->y = from->y;
    to->z = from->z;
    to->h = from->h;
}

// return the dot product of a and b
double vector_dot(Vector *a, Vector *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

// return the cross product of a and b
void vector_cross(Vector *a, Vector *b, Vector *c) {
    c->x = a->y * b->z - a->z * b->y;
    c->y = a->z * b->x - a->x * b->z;
    c->z = a->x * b->y - a->y * b->x;
    c->h = 0.0;
}

void vector_normal(Point *p0, Point *p1, Point *p2, Vector *n) {
    Vector u, v;

    vector_set(&u, p1->x - p0->x, p1->y - p0->y, p1->z - p0->z);
    vector_set(&v, p2->x - p0->x, p2->y - p0->y, p2->z - p0->z);

    vector_cross(&u, &v, n);

    vector_normalize(n);
}

void init_particle(Particle *p, float originX, float originY) {
    p->x = originX;
    p->y = originY;
    p->vx = ((float)rand() / RAND_MAX) * 200 - 100; 
    p->vy = ((float)rand() / RAND_MAX) * 200 - 100;
    color_set(&p->color, 1.0, 0.5, 0.0); 
    p->lifetime = ((float)rand() / RAND_MAX) * 2; 
    p->age = 0.0;
}

void init_particle_system(ParticleSystem *ps, int maxParticles, float particleLifetime, Point emitter) {
    ps->particles = (Particle *)malloc(sizeof(Particle) * maxParticles);
    ps->maxParticles = maxParticles;
    ps->particleLifetime = particleLifetime;
    ps->emitter = emitter;

    for (int i = 0; i < maxParticles; i++) {
        init_particle(&ps->particles[i], emitter.x, emitter.y);
    }
}

void update_particles(ParticleSystem *ps, float deltaTime) {
    for (int i = 0; i < ps->maxParticles; i++) {
        Particle *p = &ps->particles[i];
        if (p->age < p->lifetime) {
            // Update position based on velocity
            p->x += p->vx * deltaTime;
            p->y += p->vy * deltaTime;

            
            p->vy -= 9.8 * deltaTime * 0.1; 

            // Update age
            p->age += deltaTime;

            // Update color based on age
            float lifeRatio = p->age / p->lifetime;
            p->color.r = 1 - lifeRatio;
            p->color.g = (1 - lifeRatio) * 0.5;
            p->color.b = 0.0;
        } else {
            // Reinitialize particle if it has expired
            init_particle(p, ps->emitter.x, ps->emitter.y);
        }
    }
}

void render_particles(ParticleSystem *ps, Image *image, int width, int height) {
    for (int i = 0; i < ps->maxParticles; i++) {
        Particle *p = &ps->particles[i];
        if (p->lifetime > 0) {
            int x = (int)p->x;
            int y = (int)p->y;
            // printf("x: %d, y: %d\n", x, y);
            // printf("color: %f, %f, %f\n", p->color.r, p->color.g, p->color.b);
            image_setColor(image, y, x, p->color);
        }
    }
}

void reset_particle(Particle *p, Point emitterPosition, float speed, Color color, float lifetime) {
    p->x = emitterPosition.x;
    p->y = emitterPosition.y;
    p->vx = (rand() % 100 - 50) / 50.0 * speed;
    p->vy = (rand() % 100 - 50) / 50.0 * speed;
    p->color = color;
    p->lifetime = lifetime;
    p->age = 0.0;
}

void init_explosion(ParticleSystem *ps, Point emitter) {
    for (int i = 0; i < ps->maxParticles; i++) {
        Color explosionColor = {1.0, (rand() % 100) / 100.0, 0}; 
        reset_particle(&ps->particles[i], emitter, 4.0, explosionColor, 1.0);
    }
}

void init_fire(ParticleSystem *ps, Point emitter) {
    for (int i = 0; i < ps->maxParticles; i++) {
        Color fireColor = {1.0, (rand() % 50 + 50) / 100.0, 0}; 
        reset_particle(&ps->particles[i], emitter, 1.0, fireColor, 2.0);
    }
}

void init_exhaust(ParticleSystem *ps, Point emitter) {
    for (int i = 0; i < ps->maxParticles; i++) {
        Color exhaustColor = {0.5, 0.5, 0.5}; 
        reset_particle(&ps->particles[i], emitter, 2.0, exhaustColor, 1.5);
    }
}

void init_smoke(ParticleSystem *ps, Point emitter) {
    for (int i = 0; i < ps->maxParticles; i++) {
        Color smokeColor = {0.3, 0.3, 0.3}; 
        reset_particle(&ps->particles[i], emitter, 0.5, smokeColor, 3.0);
    }
}



