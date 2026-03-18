/*
Ziyan Chen
2024 Summer Term
image functions including image initialization, allocation, deallocation, setting, reset and write
*/

#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define USECPP 0

/*
initialize image structure
*/
void image_init(Image *src) {
    src->data = NULL;
    src->rows = 0;
    src->cols = 0;
    src->zbuffer = 1;
}

/*
create image with row and column input
*/
Image* image_create(int rows, int cols) {
    Image* img = (Image*)malloc(sizeof(Image));
    image_init(img);
    img->rows = rows;
    img->cols = cols;
    image_alloc(img, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img->data[i][j].z = 1;
        }
    }
    return img;
}

/*
free image memory mannually
*/
void image_free(Image *img) {
    image_dealloc(img);
    free(img);
}

/*
manullay allocate memory for image with row and column input
*/
void image_alloc(Image *img, int rows, int cols) {
    img->data = (FPixel **)malloc(rows * sizeof(FPixel *));
    for (int i = 0; i < rows; i++) {
        img->data[i] = (FPixel *)malloc(cols * sizeof(FPixel));
    }
}

/*
mannually deallocate memory for image and pointers for each row
*/
void image_dealloc(Image *img) {
    for (int i = 0; i < img->rows; i++) {
        free(img->data[i]);
    }
    free(img->data);
}

/*
set pixel value for image with row, column and rbg channel input
*/
void image_setc(Image *img, int row, int col, int channel, float value) {
    if (row >= 0 && row < img->rows && col >= 0 && col < img->cols) {
        if (channel == 0)
            img->data[row][col].r = value;
        else if (channel == 1)
            img->data[row][col].g = value;
        else if (channel == 2)
            img->data[row][col].b = value;
    }
}

/*
set image pixel value with row, column and FPixel input
*/
void image_setf(Image *img, int row, int col, FPixel p) {
    if (row >= 0 && row < img->rows && col >= 0 && col < img->cols) {
        img->data[row][col] = p;
    }
}

/*
reset the image to black
*/
void image_reset(Image *img) {
    for (int i = 0; i < img->rows; i++) {
        for (int j = 0; j < img->cols; j++) {
            img->data[i][j].r = 0;
            img->data[i][j].g = 0;
            img->data[i][j].b = 0;
            img->data[i][j].z = 1;
        }
    }
}

/*
read image from ppm file
*/
Image* image_read(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Unable to open file");
        exit(1);
    }
    
    int width, height, maxval;
    fscanf(fp, "P6\n%d %d\n%d\n", &width, &height, &maxval);
    Image *img = image_create(width, height);
    fread(img->data, sizeof(FPixel), width * height, fp);
    fclose(fp);
    
    return img;
}


/*
write the image to ppm file

*/
void image_write(Image *img, char *filename) {
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", img->cols, img->rows);
    for (int i = 0; i < img->rows; i++) {
        for (int j = 0; j < img->cols; j++) {
            fputc((unsigned char)(img->data[i][j].r * 255), fp);
            fputc((unsigned char)(img->data[i][j].g * 255), fp);
            fputc((unsigned char)(img->data[i][j].b * 255), fp);
        }
    }
    fclose(fp);
}

