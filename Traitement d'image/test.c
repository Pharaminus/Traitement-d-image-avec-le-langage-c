// #include <stdio.h>
// #include <stdlib.h>

// typedef struct {
//     int width;
//     int height;
//     int max_gray;
//     int **pixels;
// } PGMImage;

// void readPGM(const char *filename, PGMImage *img) {
//     FILE *file = fopen(filename, "r");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     char format[3];
//     fscanf(file, "%s", format);
//     fscanf(file, "%d %d", &img->width, &img->height);
//     fscanf(file, "%d", &img->max_gray);

//     img->pixels = (int **)malloc(img->height * sizeof(int *));
//     for (int i = 0; i < img->height; i++) {
//         img->pixels[i] = (int *)malloc(img->width * sizeof(int));
//         for (int j = 0; j < img->width; j++) {
//             fscanf(file, "%d", &img->pixels[i][j]);
//         }
//     }

//     fclose(file);
// }

// void writePGM(const char *filename, const PGMImage *img) {
//     FILE *file = fopen(filename, "w");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     fprintf(file, "P2\n");
//     fprintf(file, "%d %d\n", img->width, img->height);
//     fprintf(file, "%d\n", img->max_gray);

//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             fprintf(file, "%d ", img->pixels[i][j]);
//         }
//         fprintf(file, "\n");
//     }

//     fclose(file);
// }

// int piecewiseLinearTransformation(int pixel) {
//     // Exemple de transformation linéaire par morceau
//     if (pixel < 128) {
//         return pixel * 2;
//     } else {
//         return 255 - ((255 - pixel) * 2);
//     }
// }

// void applyTransformation(PGMImage *img) {
//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             img->pixels[i][j] = piecewiseLinearTransformation(img->pixels[i][j]);
//         }
//     }
// }

// void freePGM(PGMImage *img) {
//     for (int i = 0; i < img->height; i++) {
//         free(img->pixels[i]);
//     }
//     free(img->pixels);
// }

// int main() {
//     PGMImage img;

//     readPGM("ImageFold/1.pgm", &img);
//     applyTransformation(&img);
//     writePGM("SAVE/transformLinear.pgm", &img);

//     freePGM(&img);

//     return 0;
// }

//   Lineaire Simple

// #include <stdio.h>
// #include <stdlib.h>

// typedef struct {
//     int width;
//     int height;
//     int max_gray;
//     int **pixels;
// } PGMImage;

// void readPGM(const char *filename, PGMImage *img) {
//     FILE *file = fopen(filename, "r");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     char format[3];
//     fscanf(file, "%s", format);

//     fscanf(file, "%d %d", &img->width, &img->height);
//     fscanf(file, "%d", &img->max_gray);

//     img->pixels = (int **)malloc(img->height * sizeof(int *));
//     for (int i = 0; i < img->height; i++) {
//         img->pixels[i] = (int *)malloc(img->width * sizeof(int));
//         for (int j = 0; j < img->width; j++) {
//             fscanf(file, "%d", &img->pixels[i][j]);
//         }
//     }

//     fclose(file);
// }

// void writePGM(const char *filename, const PGMImage *img) {
//     FILE *file = fopen(filename, "w");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     fprintf(file, "P2\n");
//     fprintf(file, "%d %d\n", img->width, img->height);
//     fprintf(file, "%d\n", img->max_gray);

//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             fprintf(file, "%d ", img->pixels[i][j]);
//         }
//         fprintf(file, "\n");
//     }

//     fclose(file);
// }

// void linearTransformation(PGMImage *img, float a, float b) {
//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             int new_value = (int)(a * img->pixels[i][j] + b);
//             if (new_value < 0) new_value = 0;
//             if (new_value > img->max_gray) new_value = img->max_gray;
//             img->pixels[i][j] = new_value;
//         }
//     }
// }

// void freePGM(PGMImage *img) {
//     for (int i = 0; i < img->height; i++) {
//         free(img->pixels[i]);
//     }
//     free(img->pixels);
// }

// int main() {
//     PGMImage img;

//     readPGM("ImageFold/1.pgm", &img);
//     linearTransformation(&img, 1.2, 20);
//     writePGM("output_linear_1.pgm", &img);

//     freePGM(&img);

//     return 0;
// }



// Lineaire par saturation

// #include <stdio.h>
// #include <stdlib.h>

// typedef struct {
//     int width;
//     int height;
//     int max_gray;
//     int **pixels;
// } PGMImage;

// void readPGM(const char *filename, PGMImage *img) {
//     FILE *file = fopen(filename, "r");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     char format[3];
//     fscanf(file, "%s", format);

//     fscanf(file, "%d %d", &img->width, &img->height);
//     fscanf(file, "%d", &img->max_gray);

//     img->pixels = (int **)malloc(img->height * sizeof(int *));
//     for (int i = 0; i < img->height; i++) {
//         img->pixels[i] = (int *)malloc(img->width * sizeof(int));
//         for (int j = 0; j < img->width; j++) {
//             fscanf(file, "%d", &img->pixels[i][j]);
//         }
//     }

//     fclose(file);
// }

// void writePGM(const char *filename, const PGMImage *img) {
//     FILE *file = fopen(filename, "w");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     fprintf(file, "P2\n");
//     fprintf(file, "%d %d\n", img->width, img->height);
//     fprintf(file, "%d\n", img->max_gray);

//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             fprintf(file, "%d ", img->pixels[i][j]);
//         }
//         fprintf(file, "\n");
//     }

//     fclose(file);
// }

// void linearTransformationWithSaturation(PGMImage *img, float a, float b) {
//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             int new_value = (int)(a * img->pixels[i][j] + b);
//             if (new_value < 0) new_value = 0;
//             if (new_value > img->max_gray) new_value = img->max_gray;
//             img->pixels[i][j] = new_value;
//         }
//     }
// }

// void freePGM(PGMImage *img) {
//     for (int i = 0; i < img->height; i++) {
//         free(img->pixels[i]);
//     }
//     free(img->pixels);
// }

// int main() {
//     PGMImage img;

//     readPGM("ImageFold/1.pgm", &img);
//     linearTransformationWithSaturation(&img, 1.2, 20);
//     writePGM("output_linear_saturation.pgm", &img);

//     freePGM(&img);

//     return 0;
// }

// par MORCEAU

// #include <stdio.h>
// #include <stdlib.h>

// typedef struct {
//     int width;
//     int height;
//     int max_gray;
//     int **pixels;
// } PGMImage;

// void readPGM(const char *filename, PGMImage *img) {
//     FILE *file = fopen(filename, "r");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     char format[3];
//     fscanf(file, "%s", format);

//     fscanf(file, "%d %d", &img->width, &img->height);
//     fscanf(file, "%d", &img->max_gray);

//     img->pixels = (int **)malloc(img->height * sizeof(int *));
//     for (int i = 0; i < img->height; i++) {
//         img->pixels[i] = (int *)malloc(img->width * sizeof(int));
//         for (int j = 0; j < img->width; j++) {
//             fscanf(file, "%d", &img->pixels[i][j]);
//         }
//     }

//     fclose(file);
// }

// void writePGM(const char *filename, const PGMImage *img) {
//     FILE *file = fopen(filename, "w");
//     if (!file) {
//         fprintf(stderr, "Cannot open file %s\n", filename);
//         exit(1);
//     }

//     fprintf(file, "P2\n");
//     fprintf(file, "%d %d\n", img->width, img->height);
//     fprintf(file, "%d\n", img->max_gray);

//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             fprintf(file, "%d ", img->pixels[i][j]);
//         }
//         fprintf(file, "\n");
//     }

//     fclose(file);
// }

// int piecewiseLinearTransformation(int pixel) {
//     // Exemple de transformation linéaire par morceau
//     if (pixel < 128) {
//         return pixel * 2;
//     } else {
//         return 255 - ((255 - pixel) * 2);
//     }
// }

// void applyPiecewiseTransformation(PGMImage *img) {
//     for (int i = 0; i < img->height; i++) {
//         for (int j = 0; j < img->width; j++) {
//             img->pixels[i][j] = piecewiseLinearTransformation(img->pixels[i][j]);
//         }
//     }
// }

// void freePGM(PGMImage *img) {
//     for (int i = 0; i < img->height; i++) {
//         free(img->pixels[i]);
//     }
//     free(img->pixels);
// }

// int main() {
//     PGMImage img;

//     readPGM("ImageFold/1.pgm", &img);

//     applyPiecewiseTransformation(&img);
//     writePGM("output_piecewise.pgm", &img);

//     freePGM(&img);

//     return 0;
// }


///CONTRASTE



