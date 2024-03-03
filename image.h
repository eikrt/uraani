#ifndef TGA_LOADER_H
#define TGA_LOADER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
        int width;
        int height;
        unsigned char *data;
} TGAImage;

// Function to load a TGA image
TGAImage *loadTGA(const char *filename);

// Function to free memory allocated for TGA image
void freeTGA(TGAImage *image);

#endif /* TGA_LOADER_H */
