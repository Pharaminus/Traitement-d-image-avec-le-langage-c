#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pgm.h"
#include<unistd.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void saveResultImage(PGMImage *resultImage) {
    char *filename = "imageTraite.pgm";
    if (resultImage != NULL) {
        // printf("Entrez le nom du fichier pour enregistrer l'image resultante: ");
        // scanf("%s", filename);

        savePGM(filename, resultImage);
        printf("Image sauvegardee avec succes\n");
        system("open imageTraite.pgm");
        freePGM(resultImage);
    }
}

// Fonction pour lire une image PGM à partir d'un fichier
PGMImage* readPGM(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Utilisation de "rb" pour lecture binaire
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    PGMImage *image = (PGMImage *)malloc(sizeof(PGMImage));
    if (image == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        fclose(file);
        return NULL;
    }

    // Lecture de l'en-tête du fichier PGM
    fscanf(file, "%2s", image->format);
    fscanf(file, "%d %d", &image->width, &image->height);
    fscanf(file, "%d", &image->max_gray);
    fgetc(file); // Pour lire le caractère de fin de ligne après max_gray

    int num_pixels = image->width * image->height;
    image->pixels = (unsigned char *)malloc(num_pixels * sizeof(unsigned char));
    if (image->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(image);
        fclose(file);
        return NULL;
    }

    if (strcmp(image->format, "P2") == 0) {
        // Lecture des valeurs de pixels en format ASCII (P2)
        for (int i = 0; i < num_pixels; i++) {
            int pixel_value;
            fscanf(file, "%d", &pixel_value);
            image->pixels[i] = (unsigned char)pixel_value;
        }
    } else if (strcmp(image->format, "P5") == 0) {
        // Lecture des valeurs de pixels en format binaire (P5)
        fread(image->pixels, sizeof(unsigned char), num_pixels, file);
    } else {
        printf("Format PGM non supporté: %s\n", image->format);
        free(image->pixels);
        free(image);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return image;
}

// Fonction pour enregistrer une image PGM dans un fichier
void savePGM(const char *filename, PGMImage *image) {
    FILE *file = fopen(filename, "wb"); // Utilisation de "wb" pour écriture binaire
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    // Écriture de l'en-tête du fichier PGM
    fprintf(file, "%s\n", image->format);
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "%d\n", image->max_gray);

    int num_pixels = image->width * image->height;
    if (strcmp(image->format, "P2") == 0) {
        // Écriture des valeurs de pixels en format ASCII (P2)
        for (int i = 0; i < num_pixels; i++) {
            fprintf(file, "%d ", image->pixels[i]);
            if ((i + 1) % image->width == 0) {
                fprintf(file, "\n");
            }
        }
    } else if (strcmp(image->format, "P5") == 0) {
        // Écriture des valeurs de pixels en format binaire (P5)
        fwrite(image->pixels, sizeof(unsigned char), num_pixels, file);
    } else {
        printf("Format PGM non supporté: %s\n", image->format);
    }

    fclose(file);
}

// Fonction pour libérer la mémoire allouée à l'image PGM
void freePGM(PGMImage *image) {
    free(image->pixels);
    free(image);
}

// Fonction pour ajouter deux images PGM
PGMImage* addPGM(PGMImage *img1, PGMImage *img2) {
    if (img1->width != img2->width || img1->height != img2->height) {
        printf("Les dimensions des images ne correspondent pas pour l'addition\n");
        return NULL;
    }

    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *img1;
    result->pixels = (unsigned char *)malloc(result->width * result->height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    int num_pixels = result->width * result->height;
    for (int i = 0; i < num_pixels; i++) {
        int pixel_value = img1->pixels[i] + img2->pixels[i];
        if (pixel_value > result->max_gray) {
            pixel_value = result->max_gray;
        }
        result->pixels[i] = (unsigned char)pixel_value;
    }

    return result;
}

// Fonction pour soustraire deux images PGM
PGMImage* subtractPGM(PGMImage *img1, PGMImage *img2) {
    if (img1->width != img2->width || img1->height != img2->height) {
        printf("Les dimensions des images ne correspondent pas pour la soustraction\n");
        return NULL;
    }

    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *img1;
    result->pixels = (unsigned char *)malloc(result->width * result->height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    int num_pixels = result->width * result->height;
    for (int i = 0; i < num_pixels; i++) {
        int pixel_value = img1->pixels[i] - img2->pixels[i];
        if (pixel_value < 0) {
            pixel_value = 0;
        }
        result->pixels[i] = (unsigned char)pixel_value;
    }

    return result;
}

// Fonction pour multiplier une image PGM par une constante
PGMImage* multiplyPGM(PGMImage *image, double constant) {
    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *image;
    result->pixels = (unsigned char *)malloc(result->width * result->height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    int num_pixels = result->width * result->height;
    for (int i = 0; i < num_pixels; i++) {
        int pixel_value = round(image->pixels[i] * constant);
        if (pixel_value > result->max_gray) {
            pixel_value = result->max_gray;
        }
        result->pixels[i] = (unsigned char)pixel_value;
    }

    return result;
}
// Fonction pour augmenter la luminance d'une image PGM
PGMImage* increaseLuminance(PGMImage *image, int delta) {
    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *image;
    result->pixels = (unsigned char *)malloc(result->width * result->height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    int num_pixels = result->width * result->height;
    for (int i = 0; i < num_pixels; i++) {
        int pixel_value = image->pixels[i] + delta;
        if (pixel_value > result->max_gray) {
            pixel_value = result->max_gray;
        }
        result->pixels[i] = (unsigned char)pixel_value;
    }

    return result;
}

// Fonction pour diminuer la luminance d'une image PGM
PGMImage* decreaseLuminance(PGMImage *image, int delta) {
    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *image;
    result->pixels = (unsigned char *)malloc(result->width * result->height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    int num_pixels = result->width * result->height;
    for (int i = 0; i < num_pixels; i++) {
        int pixel_value = image->pixels[i] - delta;
        if (pixel_value < 0) {
            pixel_value = 0;
        }
        result->pixels[i] = (unsigned char)pixel_value;
    }

    return result;
}

// Fonction pour calculer l'histogramme d'une image PGM
void computeHistogram(PGMImage *image, int histogram[]) {
    for (int i = 0; i <= image->max_gray; i++) {
        histogram[i] = 0;
    }

    int num_pixels = image->width * image->height;
    for (int i = 0; i < num_pixels; i++) {
        histogram[image->pixels[i]]++;
    }
}

// Fonction pour afficher l'histogramme d'une image PGM
void computeAndDisplayHistogram(PGMImage *image) {
    int *histogram = (int *)malloc((image->max_gray + 1) * sizeof(int));
    if (histogram == NULL) {
        printf("Erreur d'allocation de mémoire pour l'histogramme\n");
        return;
    }

    computeHistogram(image, histogram);

    printf("Histogramme de l'image PGM:\n");
    for (int i = 0; i <= image->max_gray; i++) {
        printf("%d: %d\n", i, histogram[i]);
    }

    free(histogram);
}

// Fonction pour calculer le contraste d'une image PGM
double computeContrast(PGMImage *image) {
    int histogram[256] = {0};
    computeHistogram(image, histogram);

    int num_pixels = image->width * image->height;
    double mean = 0.0;
    for (int i = 0; i <= image->max_gray; i++) {
        mean += i * histogram[i];
    }
    mean /= num_pixels;

    double contrast = 0.0;
    for (int i = 0; i <= image->max_gray; i++) {
        contrast += histogram[i] * pow(i - mean, 2);
    }
    contrast = sqrt(contrast / num_pixels);

    return contrast;
}

// Fonction pour appliquer un filtre moyen à une image PGM
PGMImage* applyMeanFilter(PGMImage *image, int **kernel, int kernel_size) {
    int width = image->width;
    int height = image->height;
    int pad = kernel_size / 2;

    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *image;
    result->pixels = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sum = 0;
            int count = 0;
            for (int ky = -pad; ky <= pad; ky++) {
                for (int kx = -pad; kx <= pad; kx++) {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        sum += image->pixels[ny * width + nx] * kernel[ky + pad][kx + pad];
                        count += kernel[ky + pad][kx + pad];
                    }
                }
            }
            result->pixels[y * width + x] = sum / count;
        }
    }

    return result;
}


// Fonction pour appliquer un filtre médian à une image PGM
int compare(const void *a, const void *b) {
    return (*(unsigned char *)a - *(unsigned char *)b);
}

PGMImage* applyMedianFilter(PGMImage *image, int **kernel, int kernel_size) {
    int width = image->width;
    int height = image->height;
    int pad = kernel_size / 2;

    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *image;
    result->pixels = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    unsigned char *window = (unsigned char *)malloc(kernel_size * kernel_size * sizeof(unsigned char));
    if (window == NULL) {
        printf("Erreur d'allocation de mémoire pour la fenêtre\n");
        free(result->pixels);
        free(result);
        return NULL;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int count = 0;
            for (int ky = -pad; ky <= pad; ky++) {
                for (int kx = -pad; kx <= pad; kx++) {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        window[count++] = image->pixels[ny * width + nx];
                    }
                }
            }
            qsort(window, count, sizeof(unsigned char), compare);
            result->pixels[y * width + x] = window[count / 2];
        }
    }

    free(window);
    return result;
}


// Fonction pour appliquer un filtre gaussien à une image PGM
double **createGaussianKernel(int kernel_size, double sigma) {
    double **kernel = (double **)malloc(kernel_size * sizeof(double *));
    if (kernel == NULL) {
        printf("Erreur d'allocation de mémoire pour le noyau gaussien\n");
        return NULL;
    }

    double sum = 0.0;
    int pad = kernel_size / 2;
    double s = 2.0 * sigma * sigma;

    for (int y = -pad; y <= pad; y++) {
        kernel[y + pad] = (double *)malloc(kernel_size * sizeof(double));
        for (int x = -pad; x <= pad; x++) {
            double r = sqrt(x * x + y * y);
            kernel[y + pad][x + pad] = (exp(-(r * r) / s)) / (M_PI * s);
            sum += kernel[y + pad][x + pad];
        }
    }

    for (int y = 0; y < kernel_size; y++) {
        for (int x = 0; x < kernel_size; x++) {
            kernel[y][x] /= sum;
        }
    }

    return kernel;
}

void freeGaussianKernel(double **kernel, int kernel_size) {
    for (int i = 0; i < kernel_size; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

PGMImage* applyGaussianFilter(PGMImage *image, int kernel_size, double sigma) {
    int width = image->width;
    int height = image->height;
    int pad = kernel_size / 2;

    PGMImage *result = (PGMImage *)malloc(sizeof(PGMImage));
    if (result == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    *result = *image;
    result->pixels = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if (result->pixels == NULL) {
        printf("Erreur d'allocation de mémoire pour les pixels\n");
        free(result);
        return NULL;
    }

    double **kernel = createGaussianKernel(kernel_size, sigma);
    if (kernel == NULL) {
        free(result->pixels);
        free(result);
        return NULL;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum = 0.0;
            for (int ky = -pad; ky <= pad; ky++) {
                for (int kx = -pad; kx <= pad; kx++) {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        sum += image->pixels[ny * width + nx] * kernel[ky + pad][kx + pad];
                    }
                }
            }
            result->pixels[y * width + x] = (unsigned char)sum;
        }
    }

    freeGaussianKernel(kernel, kernel_size);
    return result;
}

