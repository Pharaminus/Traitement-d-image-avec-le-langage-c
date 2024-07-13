#ifndef ENTETE_H
#define ENTETE_H

#define DEG2RAD 0.017453293f

typedef struct {
    int width;
    int height;
    int max_value;
    unsigned char *data;
} PGMImage;

// Function declarations
PGMImage* readPGM(const char *filename);
void writePGM(const char *filename, const PGMImage *image);


void transformation_lineaire_par_morceau(PGMImage *image);
void transformation_lineaire_saturation(PGMImage *image);
void egalisation_histogramme(PGMImage *image);
void transformation_lineaire(PGMImage *image, double alpha, double beta);

void hough_transform(PGMImage *image, int **accumulator, int *acc_width, int *acc_height);
void draw_hough_lines(PGMImage *image, int *accumulator, int acc_width, int acc_height, int threshold);



void filtre_roberts(PGMImage *image) ;
void filtre_prewitt(PGMImage *image);

void filtre_sobel(PGMImage *image);
void filtre_laplacien(PGMImage *image);

#endif // ENTETE_H