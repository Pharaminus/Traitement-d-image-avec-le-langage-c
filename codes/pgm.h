#ifndef PGM_H
#define PGM_H

typedef struct {
    char format[3];  // Format de l'image (P2 ou P5)
    int width;       // Largeur de l'image
    int height;      // Hauteur de l'image
    int max_gray;    // Valeur maximale de gris
    unsigned char *pixels; // Pointeur vers les données des pixels (gère les deux formats)
} PGMImage;
void clearScreen();
void saveResultImage(PGMImage *resultImage);
PGMImage* readPGM(const char *filename);
void savePGM(const char *filename, PGMImage *image);
void freePGM(PGMImage *image);
PGMImage* addPGM(PGMImage *img1, PGMImage *img2);
PGMImage* subtractPGM(PGMImage *img1, PGMImage *img2);
PGMImage* multiplyPGM(PGMImage *image, double constant);  // Mise à jour de la déclaration de la fonction
PGMImage* increaseLuminance(PGMImage *image, int delta);
PGMImage* decreaseLuminance(PGMImage *image, int delta);
void computeHistogram(PGMImage *image, int histogram[]);
void computeAndDisplayHistogram(PGMImage *image);
double computeContrast(PGMImage *image);
PGMImage* applyMeanFilter(PGMImage *image, int **kernel, int kernel_size);
PGMImage* applyMedianFilter(PGMImage *image, int **kernel, int kernel_size);
PGMImage* applyGaussianFilter(PGMImage *image, int kernel_size, double sigma);

#endif // PGM_H
