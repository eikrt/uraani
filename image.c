#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

// Function to load a TGA image
TGAImage *loadTGA(const char *filename) {
        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
                fprintf(stderr, "Error: Failed to open file\n");
                return NULL;
        }

        // Read header
        unsigned char header[18];
        fread(header, sizeof(unsigned char), 18, file);

        int width = header[12] + (header[13] << 8);
        int height = header[14] + (header[15] << 8);
        int bpp = header[16] >> 3;  // Bytes per pixel

        // Skip image ID field
        fseek(file, header[0], SEEK_CUR);

        // Read image data
        unsigned char *data = (unsigned char *)malloc(width * height * bpp);
        fread(data, sizeof(unsigned char), width * height * bpp, file);

        fclose(file);

        // Create TGAImage structure
        TGAImage *image = (TGAImage *)malloc(sizeof(TGAImage));
        if (image == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                free(data);
                return NULL;
        }
        image->width = width;
        image->height = height;
        image->data = data;
        printf("%d\n", height);
        return image;
}

// Function to free memory allocated for TGA image
void freeTGA(TGAImage *image) {
        if (image != NULL) {
                free(image->data);
                free(image);
        }
}
