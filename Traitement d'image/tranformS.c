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




void transformation_lineaire_saturation(PGMImage *image) {
    int min_val = 0;   // Minimum possible value
    int max_val = 255; // Maximum possible value

    for (int i = 0; i < image->width * image->height; i++) {
        image->data[i] = (image->data[i] < min_val) ? min_val : (image->data[i] > max_val) ? max_val : image->data[i];
    }
}

int main() {
    PGMImage *image = readPGM("ImageFold/1.pgm");
    if (!image) {
        return 1;
    }

    transformation_lineaire_saturation(image);
    writePGM("SAVE/output_saturation.pgm", image);

    free(image->data);
    free(image);
    return 0;
}