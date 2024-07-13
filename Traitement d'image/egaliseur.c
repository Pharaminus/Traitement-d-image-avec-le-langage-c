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

void egalisation_histogramme(PGMImage *image) {
    int hist[256] = {0};
    int cdf[256] = {0};
    int num_pixels = image->width * image->height;
    
    // Calculer l'histogramme
    for (int i = 0; i < num_pixels; i++) {
        hist[image->data[i]]++;
    }

    // Calculer la fonction de distribution cumulative (CDF)
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Appliquer la transformation d'égalisation
    for (int i = 0; i < num_pixels; i++) {
        image->data[i] = (unsigned char)(((double)cdf[image->data[i]] - cdf[0]) / (num_pixels - cdf[0]) * 255);
    }
}

int main() {
    PGMImage *image = readPGM("ImageFold/1.pgm");
    if (!image) {
        return 1;
    }

    egalisation_histogramme(image);
    writePGM("SAVE/output_histogram_equalized.pgm", image);

    free(image->data);
    free(image);
    return 0;
}
