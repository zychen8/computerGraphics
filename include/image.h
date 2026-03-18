/*
Ziyan Chen
2024 Summer Term
header file of image
*/

#ifndef IMAGE_H
#define IMAGE_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  float r;
  float g; 
  float b;
  float z;
} FPixel;

typedef struct {
  FPixel **data;
  int rows;
  int cols;
  int zbuffer;
} Image;


void mandelbrot( Image *dst, float x0, float y0, float dx);
void julia(Image *dst, float x0, float y0, float dx);

Image* image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
void image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);
Image* image_read(const char *filename);
void image_setc(Image *src, int row, int col, int channel, float value);
void image_setf(Image *src, int row, int col, FPixel p);
void image_reset(Image *src);
void image_write(Image *img, char *filename);
void sine_cosine_pattern(Image *img, float fx, float fy, float phix, float phiy);
void blend_images(Image *background, Image *foreground, float transperancy);

#endif
