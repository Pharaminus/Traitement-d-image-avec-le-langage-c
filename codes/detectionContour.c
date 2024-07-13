#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include"fonctionsPGM0PBM.h"
#include"detectionContour.h"

#define MASK_SIZE 20


//================( Filtre de Robert
ImagePGM_PBM* robert_filter(ImagePGM_PBM* image) {
    printf("===>oktest");

    int x, y;
    int width = image->largeur, height = image->hauteur;
    
    ImagePGM_PBM *image_filtre = malloc(sizeof(ImagePGM_PBM));
    image_filtre->hauteur = height;
    int **pixels = malloc(height*sizeof(int*));
    for(int i = 0; i < width; i++)
        pixels[i] = malloc(width*sizeof(int));
    image->largeur = width;
    image_filtre->pixel_max = image->pixel_max;
    image_filtre->hauteur = image->hauteur;
    image_filtre->largeur = image->largeur;
    strcpy(image_filtre->type, image->type);
    for (y = 0; y < height - 1; y++) {
        for (x = 0; x < width - 1; x++) {
            int gx = abs(image->pixels[y][x] - image->pixels[y + 1][x + 1]);
            int gy = abs(image->pixels[y + 1][x] - image->pixels[y][x + 1]);
            int val = gx * gx + gy * gy;
            pixels[y][x] = (int)sqrt(val);
        }
    }
    image_filtre->pixels = pixels;
    return image_filtre;
}
 
//==============( Filtre de Prewitt
ImagePGM_PBM* prewitt_filter(ImagePGM_PBM* image) {
    int width = image->largeur, height = image->hauteur;

    ImagePGM_PBM *image_filtre = malloc(sizeof(ImagePGM_PBM));
    image_filtre->hauteur = height;
    int **pixels = malloc(height*sizeof(int*));
    for(int i = 0; i < width; i++)
        pixels[i] = malloc(width*sizeof(int));
    image->largeur = width;
    image_filtre->pixel_max = image->pixel_max;
    image_filtre->hauteur = image->hauteur;
    image_filtre->largeur = image->largeur;
    strcpy(image_filtre->type, image->type);

    int x, y, i, j;
    for (y = 0; y < height - 1; y++) {
        for (x = 0; x < width - 1; x++) {
            int gx = abs(image->pixels[y][x] - image->pixels[y + 1][x + 1]);
            int gy = abs(image->pixels[y + 1][x] - image->pixels[y][x + 1]);
            pixels[y][x] = (int)sqrt(gx * gx + gy * gy);
        }
    }

    image_filtre->pixels = pixels;
    return image_filtre;
}

//============( Filtre de Sobel
ImagePGM_PBM* sobel_filter(ImagePGM_PBM *image) {
    int width = image->largeur, height = image->hauteur;
    
    ImagePGM_PBM *image_filtre = malloc(sizeof(ImagePGM_PBM));
    image_filtre->hauteur = height;
    int **pixels = malloc(height*sizeof(int*));
    for(int i = 0; i < width; i++)
        pixels[i] = malloc(width*sizeof(int));
    image->largeur = width;
    image_filtre->pixel_max = image->pixel_max;
    image_filtre->hauteur = image->hauteur;
    image_filtre->largeur = image->largeur;
    strcpy(image_filtre->type, image->type);


    int x, y, i, j;
    int gx, gy;
    for (y = 1; y < height - 1; y++) {
        for (x = 1; x < width - 1; x++) {
            gx = 0;
            gy = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    gx += (i == 0 ? 0 : 2 * i) * image->pixels[y + i][x + j];
                    gy += (j == 0 ? 0 : 2 * j) * image->pixels[y + i][x + j];
                }
            }
            pixels[x][y] = (int)sqrt(gx * gx + gy * gy);
        }
    }
    image_filtre->pixels = pixels;
    return image_filtre;
}

//===============( Filtre Laplacien
ImagePGM_PBM* laplacian_filter(ImagePGM_PBM *image) {
    int width = image->largeur, height = image->hauteur;
    
    ImagePGM_PBM *image_filtre = malloc(sizeof(ImagePGM_PBM));
    image_filtre->hauteur = height;
    int **pixels = malloc(height*sizeof(int*));
    for(int i = 0; i < width; i++)
        pixels[i] = malloc(width*sizeof(int));
    image->largeur = width;
    image_filtre->pixel_max = image->pixel_max;
    image_filtre->hauteur = image->hauteur;
    image_filtre->largeur = image->largeur;
    strcpy(image_filtre->type, image->type);

    int x, y;
    for (y = 1; y < height - 1; y++) {
        for (x = 1; x < width - 1; x++) {
            int value = 4 * image->pixels[y][x]
                      - image->pixels[y - 1][x]
                      - image->pixels[y + 1][x]
                      - image->pixels[y][x - 1]
                      - image->pixels[y][x + 1];
            pixels[x][y] = (int)abs(value);
        }
    }
    image_filtre->pixels = pixels;
    return image_filtre;
}

