#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define MAX_ITERATIONS 100
#define INT_MAX 2147483647

#define SPLIT_THRESHOLD 150  // Seuil pour la division
#define MERGE_THRESHOLD 100 // Seuil pour la fusion
#define MIN_REGION_SIZE 4    // Taille minimale d'une région

#define MIN_REGION_SIZE 4
#define HOMOGENEITY_THRESHOLD 10

#define PI 3.14159265358979323846
typedef struct {
    int x, y, width, height;
    long long sum;
    long long sum_sq;
    int count;
} Region;

typedef struct {
    double real;
    double imag;
} Complex;

typedef struct {
    char mod[3];
    int largeur;
    int hauteur;
    int max;
    int **pixel;
} monimage;

monimage* read_pgm_p2(char* filename, monimage* img);
void write_pgm_p2(char* filename, monimage* img) ;
void calculate_histogram(monimage* img, int* histogram) ; //calcule histograme sur le terminal

int otsu_threshold(int* histogram, int total_pixels) ;

monimage* otsu_thresholding(monimage* input, monimage* output) ; //fonction qui aplique un certain suiellage par apport au seuil otsu
int abs_custom(int x);
monimage* kmeans_segmentation(monimage* input, monimage* output, int k) ;
// Fonction pour vérifier l'homogénéité d'une région
// Fonction pour calculer la variance d'une région
long long calculate_variance(Region* region) ;
// Fonction pour vérifier si une région doit être divisée
int should_split(Region* region) ;
// Fonction pour vérifier si deux régions peuvent être fusionnées
int can_merge(Region* r1, Region* r2) ;

// Fonction pour diviser une région
void split_region(monimage* img, Region* region, Region* queue, int* queue_size);


// Fonction principale de segmentation par division-fusion
monimage* split_and_merge_segmentation(monimage* input, monimage* output);






#endif // HEADER_H_INCLUDED
