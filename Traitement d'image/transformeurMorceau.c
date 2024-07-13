#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    int max_value;
    unsigned char *data;
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

    image->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
    if (!image->data) {
        fprintf(stderr, "Memory allocation error\n");
        free(image);
        fclose(file);
        return NULL;
    }

    fread(image->data, sizeof(unsigned char), image->width * image->height, file);
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
    fwrite(image->data, sizeof(unsigned char), image->width * image->height, file);
    fclose(file);
}


void transformation_lineaire_par_morceau(PGMImage *image) {
    int a1 = 0, b1 = 50;
    int a2 = 50, b2 = 200;
    int a3 = 200, b3 = 255;

    for (int i = 0; i < image->width * image->height; i++) {
        if (image->data[i] < a2) {
            image->data[i] = (image->data[i] - a1) * (b2 - b1) / (a2 - a1) + b1;
        } else if (image->data[i] < a3) {
            image->data[i] = (image->data[i] - a2) * (b3 - b2) / (a3 - a2) + b2;
        } else {
            image->data[i] = b3;
        }
    }
}

int main() {
    PGMImage *image = readPGM("ImageFold/1.pgm");
    if (!image) {
        return 1;
    }

    transformation_lineaire_par_morceau(image);
    writePGM("SAVE/output_piecewise.pgm", image);

    free(image->data);
    free(image);
    return 0;
}