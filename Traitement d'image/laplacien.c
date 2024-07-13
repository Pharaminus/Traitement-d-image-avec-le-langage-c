#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    int max_value;
    int *data;
} PGMImage;

PGMImage* readPGM(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    PGMImage *image = (PGMImage*)malloc(sizeof(PGMImage));
    if (!image) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return NULL;
    }

    fscanf(file, "P5\n%d %d\n%d\n", &image->width, &image->height, &image->max_value);

    image->data = (int*)malloc(image->width * image->height * sizeof(int));
    if (!image->data) {
        fprintf(stderr, "Memory allocation error\n");
        free(image);
        fclose(file);
        return NULL;
    }

    fread(image->data, sizeof(int), image->width * image->height, file);
    fclose(file);
    return image;
}

void writePGM(const char *filename, const PGMImage *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return;
    }

    fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, image->max_value);
    fwrite(image->data, sizeof(int), image->width * image->height, file);
    fclose(file);
}

void freePGM(PGMImage *image) {
    if (image) {
        if (image->data) {
            free(image->data);
        }
        free(image);
    }
}

void filtre_laplacien(PGMImage *image) {
    int width = image->width;
    int height = image->height;
    int *output = (int*)malloc(width * height * sizeof(int));

    int kernel[3][3] = {
        { 0, -1,  0 },
        {-1,  4, -1 },
        { 0, -1,  0 }
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    sum += image->data[(y + ky) * width + (x + kx)] * kernel[ky + 1][kx + 1];
                }
            }
            output[y * width + x] = sum;
        }
    }

    for (int i = 0; i < width * height; i++) {
        image->data[i] = output[i] > 255 ? 255 : output[i] < 0 ? 0 : output[i];
    }

    free(output);
}

int main() {
    PGMImage *image = readPGM("ImageFold/1.pgm");
    if (!image) {
        return 1;
    }

    filtre_laplacien(image);
    writePGM("SAVE/output_laplacian.pgm", image);

    freePGM(image);
    return 0;
}