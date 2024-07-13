#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Structure pour stocker une image PGM
typedef struct {
    int width;
    int height;
    int max_value;
    uint8_t *data;
} PGMImage;

// Fonction pour lire une image PGM
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

    image->data = (uint8_t*)malloc(image->width * image->height * sizeof(uint8_t));
    if (!image->data) {
        fprintf(stderr, "Memory allocation error\n");
        free(image);
        fclose(file);
        return NULL;
    }

    fread(image->data, sizeof(uint8_t), image->width * image->height, file);
    fclose(file);
    return image;
}

// Fonction pour écrire une image PGM
void writePGM(const char *filename, const PGMImage *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return;
    }

    fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, image->max_value);
    fwrite(image->data, sizeof(uint8_t), image->width * image->height, file);
    fclose(file);
}

// Fonction pour libérer la mémoire d'une image PGM
void freePGM(PGMImage *image) {
    if (image) {
        if (image->data) {
            free(image->data);
        }
        free(image);
    }
}

#define DEG2RAD 0.017453293f // Conversion degrés en radians

void hough_transform(PGMImage *image, int **accumulator, int *acc_width, int *acc_height) {
    int width = image->width;
    int height = image->height;

    // Calcul de la diagonale de l'image
    double houghHeight = (sqrt(2.0) * (height > width ? height : width)) / 2.0;

    double centerX = width / 2.0;
    double centerY = height / 2.0;

    *acc_width = 180;
    *acc_height = (int)(houghHeight * 2.0);

    *accumulator = (int*)calloc(*acc_width * *acc_height, sizeof(int));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image->data[y * width + x] > 250) { // Seuillage pour détecter les points clairs (bord)
                for (int theta = 0; theta < 180; theta++) {
                    double r = ((x - centerX) * cos(theta * DEG2RAD)) + ((y - centerY) * sin(theta * DEG2RAD));
                    (*accumulator)[(int)((r + houghHeight) * 180.0) + theta]++;
                }
            }
        }
    }
}

void draw_hough_lines(PGMImage *image, int *accumulator, int acc_width, int acc_height, int threshold) {
    double centerX = image->width / 2.0;
    double centerY = image->height / 2.0;
    double houghHeight = (sqrt(2.0) * (image->height > image->width ? image->height : image->width)) / 2.0;

    for (int r = 0; r < acc_height; r++) {
        for (int theta = 0; theta < acc_width; theta++) {
            if (accumulator[r * acc_width + theta] >= threshold) {
                double rho = r - houghHeight;
                double t = theta * DEG2RAD;
                double cos_t = cos(t);
                double sin_t = sin(t);

                for (int x = 0; x < image->width; x++) {
                    int y = (int)(((rho - ((x - centerX) * cos_t)) / sin_t) + centerY);
                    if (y >= 0 && y < image->height) {
                        image->data[y * image->width + x] = 255;
                    }
                }

                for (int y = 0; y < image->height; y++) {
                    int x = (int)(((rho - ((y - centerY) * sin_t)) / cos_t) + centerX);
                    if (x >= 0 && x < image->width) {
                        image->data[y * image->width + x] = 255;
                    }
                }
            }
        }
    }
}

int main() {
    PGMImage *image = readPGM("ImageFold/1.pgm");
    if (!image) {
        return 1;
    }

    int *accumulator;
    int acc_width, acc_height;

    hough_transform(image, &accumulator, &acc_width, &acc_height);

    // Seuillage pour détecter les lignes
    int threshold = 100;
    draw_hough_lines(image, accumulator, acc_width, acc_height, threshold);

    writePGM("SAVE/output_hough.pgm", image);

    free(accumulator);
    freePGM(image);
    return 0;
}