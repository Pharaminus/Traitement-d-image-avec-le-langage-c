#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "entete.h"

PGMImage* readPGM(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Cannot open file");
        return NULL;
    }

    PGMImage *image = (PGMImage*)malloc(sizeof(PGMImage));
    if (!image) {
        perror("Cannot allocate memory for image");
        fclose(file);
        return NULL;
    }

    char type[3];
    fscanf(file, "%2s", type);
    if (strcmp(type, "P5") != 0) {
        fprintf(stderr, "Not a valid PGM file\n");
        free(image);
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d %d", &(image->width), &(image->height));
    fscanf(file, "%d", &(image->max_value));
    fgetc(file); // consume newline

    image->data = (unsigned char*)malloc(image->width * image->height);
    if (!image->data) {
        perror("Cannot allocate memory for image data");
        free(image);
        fclose(file);
        return NULL;
    }

    fread(image->data, 1, image->width * image->height, file);
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


void freePGM(PGMImage *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}

// Implementations of the transformation and filtering functions
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

void transformation_lineaire(PGMImage *image, double alpha, double beta) {
    for (int i = 0; i < image->width * image->height; i++) {
        image->data[i] = (int)(alpha * image->data[i] + beta);
        if (image->data[i] > 255) image->data[i] = 255;
        if (image->data[i] < 0) image->data[i] = 0;
    }
}

void transformation_lineaire_saturation(PGMImage *image) {
    int min_val = 0;
    int max_val = 255;

    for (int i = 0; i < image->width * image->height; i++) {
        image->data[i] = (image->data[i] < min_val) ? min_val : (image->data[i] > max_val) ? max_val : image->data[i];
    }
}

// Implement other image processing functions as needed

void hough_transform(PGMImage *image, int **accumulator, int *acc_width, int *acc_height) {
    int width = image->width;
    int height = image->height;

    double houghHeight = (sqrt(2.0) * (height > width ? height : width)) / 2.0;

    double centerX = width / 2.0;
    double centerY = height / 2.0;

    *acc_width = 180;
    *acc_height = (int)(houghHeight * 2.0);

    *accumulator = (int*)calloc(*acc_width * *acc_height, sizeof(int));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image->data[y * width + x] > 250) {
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









void filtre_sobel(PGMImage *image) {
    int width = image->width;
    int height = image->height;
    int *output = (int*)malloc(width * height * sizeof(int));

    int gx, gy;
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            gx = -image->data[(y - 1) * width + (x - 1)] - 2 * image->data[y * width + (x - 1)] - image->data[(y + 1) * width + (x - 1)]
                 + image->data[(y - 1) * width + (x + 1)] + 2 * image->data[y * width + (x + 1)] + image->data[(y + 1) * width + (x + 1)];
            gy = -image->data[(y - 1) * width + (x - 1)] - 2 * image->data[(y - 1) * width + x] - image->data[(y - 1) * width + (x + 1)]
                 + image->data[(y + 1) * width + (x - 1)] + 2 * image->data[(y + 1) * width + x] + image->data[(y + 1) * width + (x + 1)];
            output[y * width + x] = abs(gx) + abs(gy);
        }
    }

    for (int i = 0; i < width * height; i++) {
        image->data[i] = output[i] > 255 ? 255 : output[i];
    }

    free(output);
}

void filtre_roberts(PGMImage *image) {
    int width = image->width;
    int height = image->height;
    int *output = (int*)malloc(width * height * sizeof(int));

    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int gx = image->data[y * width + x] - image->data[(y + 1) * width + (x + 1)];
            int gy = image->data[y * width + (x + 1)] - image->data[(y + 1) * width + x];
            output[y * width + x] = abs(gx) + abs(gy);
        }
    }

    for (int i = 0; i < width * height; i++) {
        image->data[i] = output[i] > 255 ? 255 : output[i];
    }

    free(output);
}

void filtre_prewitt(PGMImage *image) {
    int width = image->width;
    int height = image->height;
    int *output = (int*)malloc(width * height * sizeof(int));

    int gx, gy;
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            gx = -image->data[(y - 1) * width + (x - 1)] - image->data[y * width + (x - 1)] - image->data[(y + 1) * width + (x - 1)]
                 + image->data[(y - 1) * width + (x + 1)] + image->data[y * width + (x + 1)] + image->data[(y + 1) * width + (x + 1)];
            gy = -image->data[(y - 1) * width + (x - 1)] - image->data[(y - 1) * width + x] - image->data[(y - 1) * width + (x + 1)]
                 + image->data[(y + 1) * width + (x - 1)] + image->data[(y + 1) * width + x] + image->data[(y + 1) * width + (x + 1)];
            output[y * width + x] = abs(gx) + abs(gy);
        }
    }

    for (int i = 0; i < width * height; i++) {
        image->data[i] = output[i] > 255 ? 255 : output[i];
    }

    free(output);
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