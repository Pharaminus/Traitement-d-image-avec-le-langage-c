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



void transformation_lineaire(PGMImage *image, double alpha, double beta) {
    for (int i = 0; i < image->width * image->height; i++) {
        image->data[i] = (int)(alpha * image->data[i] + beta);
        if (image->data[i] > 255) image->data[i] = 255;
        if (image->data[i] < 0) image->data[i] = 0;
    }
}

int main() {
    PGMImage *image = readPGM("ImageFold/1.pgm");
    if (!image) {
        return 1;
    }

    double alpha = 1.2; // Example value
    double beta = 10;   // Example value

    transformation_lineaire(image, alpha, beta);
    writePGM("SAVE/output_linear.pgm", image);

    free(image->data);
    free(image);
    return 0;
}