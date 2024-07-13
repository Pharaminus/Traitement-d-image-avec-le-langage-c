#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"fonctions.h"
#include <unistd.h>
#include"fonctionsPGM0PBM.h"
#include"detectionContour.h"
#include"pgm.h"
#include "header.h"

// typedef struct {
//     int width;
//     int height;
//     int max_value;
//     unsigned char *data;
// } PGMImage; 
// void transformation_lineaire(PGMImage *image, double alpha, double beta);
// PGMImage* readPGM(const char *filename);
// void writePGM(const char *filename, const PGMImage *image);
// void transformation_lineaire_par_morceau(PGMImage *image);

void chargement();

void affichageMenuGenerale();
void menuBale();
void menuAbakar();
int main(int argc, char const *argv[])
{
    int choix = -1;
    
do
{

    /* code */


    afficherImportation();
    affichageMenuGenerale();
    printf("======# Entrer une operation de base > ");
    scanf("%d", &choix);
    switch (choix)
    {
    case 1:
        // afficherImportation();

        menuAbakar();
        int choice = -1;
        PGMImage *image1 = NULL, *image2 = NULL, *resultImage = NULL;
        char filename[256];
        int delta, kernel_size;
        double sigma, constant;

        do {
            

            menuAbakar();
            printf("==============# choisir une option : ");
            scanf("%d", &choice);
            printf("Entrez le nom du fichier a lire \033[2m(ex: ../images/pgm/baboon.512.pgm )\033[0m: ");
            scanf("%s", filename);
            image1 = readPGM(filename);
            if (image1 == NULL) {
                printf("Erreur lors de la lecture de l'image\n");
            } else {
                printf("Image lue avec succes\n");
            }
            switch (choice) {
                
                case 1:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez le nom du deuxième fichier a lire: ");
                    scanf("%s", filename);
                    image2 = readPGM(filename);
                    if (image2 == NULL) {
                        printf("Erreur lors de la lecture de l'image\n");
                        break;
                    }
                    resultImage = addPGM(image1, image2);
                    if (resultImage != NULL) {
                        printf("Images ajoutees avec succes\n");
                        saveResultImage(resultImage);
                    }
                    freePGM(image2);
                    break;
                case 2:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez le nom du deuxieme fichier à lire: ");
                    scanf("%s", filename);
                    image2 = readPGM(filename);
                    if (image2 == NULL) {
                        printf("Erreur lors de la lecture de l'image\n");
                        break;
                    }
                    resultImage = subtractPGM(image1, image2);
                    if (resultImage != NULL) {
                        printf("Images soustraites avec succes\n");
                        saveResultImage(resultImage);
                    }
                    freePGM(image2);
                    break;
                case 3:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez la constante de multiplication: ");
                    scanf("%lf", &constant);
                    resultImage = multiplyPGM(image1, constant);
                    if (resultImage != NULL) {
                        printf("Multiplication effectuee avec succes\n");
                        saveResultImage(resultImage);
                    }
                    break;
                case 4:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez la valeur de delta pour augmenter la luminance: ");
                    scanf("%d", &delta);
                    resultImage = increaseLuminance(image1, delta);
                    if (resultImage != NULL) {
                        printf("Luminance augmentee avec succes\n");
                        saveResultImage(resultImage);
                    }
                    break;
                case 5:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez la valeur de delta pour diminuer la luminance: ");
                    scanf("%d", &delta);
                    resultImage = decreaseLuminance(image1, delta);
                    if (resultImage != NULL) {
                        printf("Luminance diminuee avec succes\n");
                        saveResultImage(resultImage);
                    }
                    break;
                case 6:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    computeAndDisplayHistogram(image1);
                    break;
                case 7:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    double contrast = computeContrast(image1);
                    printf("Le contraste de l'image est: %f\n", contrast);
                    break;
                case 8:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez la taille du noyau: ");
                    scanf("%d", &kernel_size);
                    int **kernel_mean = (int **)malloc(kernel_size * sizeof(int *));
                    for (int i = 0; i < kernel_size; i++) {
                        kernel_mean[i] = (int *)malloc(kernel_size * sizeof(int));
                        for (int j = 0; j < kernel_size; j++) {
                            printf("Entrez la valeur pour la position (%d, %d): ", i, j);
                            scanf("%d", &kernel_mean[i][j]);
                        }
                    }
                    resultImage = applyMeanFilter(image1, kernel_mean, kernel_size);
                    if (resultImage != NULL) {
                        printf("Filtre moyen applique avec succes\n");
                        saveResultImage(resultImage);
                    }
                    for (int i = 0; i < kernel_size; i++) {
                        free(kernel_mean[i]);
                    }
                    free(kernel_mean);
                    break;
                case 9:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez la taille du noyau: ");
                    scanf("%d", &kernel_size);
                    int **kernel_median = (int **)malloc(kernel_size * sizeof(int *));
                    for (int i = 0; i < kernel_size; i++) {
                        kernel_median[i] = (int *)malloc(kernel_size * sizeof(int));
                        for (int j = 0; j < kernel_size; j++) {
                            printf("Entrez la valeur pour la position (%d, %d): ", i, j);
                            scanf("%d", &kernel_median[i][j]);
                        }
                    }
                    resultImage = applyMedianFilter(image1, kernel_median, kernel_size);
                    if (resultImage != NULL) {
                        printf("Filtre median applique avec succes\n");
                        saveResultImage(resultImage);
                    }
                    for (int i = 0; i < kernel_size; i++) {
                        free(kernel_median[i]);
                    }
                    free(kernel_median);
                    break;
                case 10:
                    if (image1 == NULL) {
                        printf("Aucune image chargee\n");
                        break;
                    }
                    printf("Entrez la taille du noyau: ");
                    scanf("%d", &kernel_size);
                    printf("Entrez la valeur de sigma: ");
                    scanf("%lf", &sigma);
                    resultImage = applyGaussianFilter(image1, kernel_size, sigma);
                    if (resultImage != NULL) {
                        printf("Filtre gaussien applique avec succes\n");
                        saveResultImage(resultImage);
                    }
                    break;
                default:
                    printf("Option invalide. Veuillez reessayer.\n");
            }
            printf("Entrer 0 pour continuer dans l'option 1 : ");
            scanf("%d",&choice);
        }while (choice == 0);
        
        

    case 7:

        monimage *input = malloc(sizeof(monimage));
        monimage *output = malloc(sizeof(monimage));
        int choixG;
    do{
        menuGaetan();
        printf("\n======)> Choisir une option :> ");
        scanf("%d",&choixG);
         char *pathfile = malloc(100*sizeof(char));
        printf("==================# Entrer le chemin vers une image \033[2m(ex: ../images/pgm/baboon.512.pgm )\033[0m : ");
        scanf("%s",pathfile);
        switch (choixG)
        {
           
        case 1:

            input = read_pgm_p2(pathfile, input);
            
            // Initialiser l'image de sortie
            strncpy(output->mod, input->mod, 3);
            output->largeur = input->largeur;
            output->hauteur = input->hauteur;
            output->max = input->max;
            output->pixel = (int**)malloc(output->hauteur * sizeof(int*));
            for (int i = 0; i < output->hauteur; i++) {
                output->pixel[i] = (int*)malloc(output->largeur * sizeof(int));
            }

            // Appliquer la segmentation K-means avec k=1 clusters

            output = otsu_thresholding(input,output);

            write_pgm_p2("../images/pbm0pgm_traite/image_segmentee.pgm", output);
            system("open ../images/pbm0pgm_traite/image_segmentee.pgm");

            // Libérer la mémoire
            // for (int i = 0; i < input->hauteur; i++) {
            //     free(input->pixel[i]);
            // }
            // free(input->pixel);

            // for (int i = 0; i < output->hauteur; i++) {
            //     free(output->pixel[i]);
            // }
            // free(output->pixel);
            break;
        case 2:
            /* code */

            input = read_pgm_p2(pathfile, input);

            // Initialiser l'image de sortie
            strncpy(output->mod, input->mod, 3);
            output->largeur = input->largeur;
            output->hauteur = input->hauteur;
            output->max = input->max;
            output->pixel = (int**)malloc(output->hauteur * sizeof(int*));
            for (int i = 0; i < output->hauteur; i++) {
                output->pixel[i] = (int*)malloc(output->largeur * sizeof(int));
            }

            // Appliquer la segmentation K-means avec k=1 clusters
            int k;
            printf("\n============( Entrer une valeur de K : ");
            scanf("%d",&k);
            output = kmeans_segmentation(input,output, k);

            write_pgm_p2("../images/pbm0pgm_traite/image_segmentee.pgm", output);
            system("open ../images/pbm0pgm_traite/image_segmentee.pgm");

            // Libérer la mémoire
            // for (int i = 0; i < input->hauteur; i++) {
            //     free(input->pixel[i]);
            // }
            // free(input->pixel);

            // for (int i = 0; i < output->hauteur; i++) {
            //     free(output->pixel[i]);
            // }
            // free(output->pixel);
            break;
        case 3:

            input = split_and_merge_segmentation(pathfile, input);

            // Initialiser l'image de sortie
            strncpy(output->mod, input->mod, 3);
            output->largeur = input->largeur;
            output->hauteur = input->hauteur;
            output->max = input->max;
            output->pixel = (int**)malloc(output->hauteur * sizeof(int*));
            for (int i = 0; i < output->hauteur; i++) {
                output->pixel[i] = (int*)malloc(output->largeur * sizeof(int));
            }

            // Appliquer la segmentation K-means avec k=1 clusters
        
            output = split_and_merge_segmentation(input,output);

            write_pgm_p2("..images/pbm0pgm_traite/image_segmentee.pgm", output);
            system("open ..images/pbm0pgm_traite/image_segmentee.pgm");

            // Libérer la mémoire
            // for (int i = 0; i < input->hauteur; i++) {
            //     free(input->pixel[i]);
            // }
            // free(input->pixel);

            // for (int i = 0; i < output->hauteur; i++) {
            //     free(output->pixel[i]);
            // }
            // free(output->pixel);
            
            break;
        
        default:
            break;
        }
        printf("Entrer 0 pour continuer au menu 7 : ");
        scanf("%d",&choixG);

    }while (choixG  == 0);
    
    
        break;
    case 8:
        int choixB;

        int filtre[500][500] = {
        {1, 1, 0},
        {1, 1, 1},
        {1, 1, 0}
                 };
    do{
        menuBale();
        ImagePGM_PBM *imagePImagePGM_PBM1 = malloc(sizeof(ImagePGM_PBM));

        printf("===============( Choisir une option : ");
        scanf("%d",&choixB);
        char *cheminfile = malloc(100*sizeof(char));
        printf("==================# Entrer le chemin vers une image \033[2m(ex: ../images/pgm/baboon.512.pgm )\033[0m : ");
        scanf("%s",cheminfile);
        switch (choixB)
        {
        case 1:

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = robert_filter(imagePImagePGM_PBM1);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;

        case 2:

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = prewitt_filter(imagePImagePGM_PBM1);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;

        case 3:

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = sobel_filter(imagePImagePGM_PBM1);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;
        case 4:

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = laplacian_filter(imagePImagePGM_PBM1);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;

        case 5:
            

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = erosion(imagePImagePGM_PBM1, filtre, 3);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;
        case 6:
            

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = erosion(imagePImagePGM_PBM1, filtre, 3);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;
        case 7:
            

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = ouverture(imagePImagePGM_PBM1, filtre, 3);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;
        case 8:
            

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = fermeture(imagePImagePGM_PBM1, filtre, 3);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;
        case 0:
            
            int seuil;
            printf("==========( Entrer le seuil : ");
            scanf("%d",&seuil);

            imagePImagePGM_PBM1 = loadImagePGM0PBM(cheminfile);
            imagePImagePGM_PBM1 = binarizePGM(imagePImagePGM_PBM1, seuil);
            writePGM0PBMFile(imagePImagePGM_PBM1, "../images/pbm0pgm_traite/traitement.pgm");
            system("open ../images/pbm0pgm_traite/traitement.pgm");
            break;
        default:
            break;
        }
        printf("Entrer 0 pour continuer dans l'option 8 : ");
        scanf("%d",&choixB);
    }while (choixB == 0);
        break;
    
    default:
        break;
    }
    printf("Entrer 0 pour continuer au menu principale : ");
    scanf("%d",&choix);
}while (choix == 0);
    return 0;
}


void menuBale(){
    system("clear");
    printf("\t\t\t\t\t\t#==============================================#\n");
    printf("\t\t\t\t\t\t#    0-Binarisation                            #\n");
    printf("\t\t\t\t\t\t#    1-Detection contour(Robert)               #\n");
    printf("\t\t\t\t\t\t#    2-Detection contour(prewitt)              #\n");
    printf("\t\t\t\t\t\t#    3-Detection contour(sobel)                #\n");
    printf("\t\t\t\t\t\t#    4-Detection contour(laplacien)            #\n");
    printf("\t\t\t\t\t\t#    5-erosion                                 #\n");
    printf("\t\t\t\t\t\t#    6-dilatation                              #\n");
    printf("\t\t\t\t\t\t#    7-ouverture                               #\n");
    printf("\t\t\t\t\t\t#    8-fermeture                               #\n");
    printf("\t\t\t\t\t\t#==============================================#\n");
}


// =========( methode pour l'affichage de l'interface )==================
void afficherImportation(){
    chargement();
    system("clear");
    printf("\n");
    printf("\t\t\t\t\t\t===========================================================================\n");
    printf("\t\t\t\t\t\t===#                                                                 #=====\n");
    printf("\t\t\t\t\t\t===#                       \033[1m\033[32mTRAITEMENT D'IMAGE\033[0m                        #=====\n");
    printf("\t\t\t\t\t\t===#                                                                 #=====\n");
    printf("\t\t\t\t\t\t===========================================================================\n\n\n");
}

void chargement(){
    system("clear");
    char chargement[12];
	int retardateur = 0;
	while(retardateur < 3)
	{
		system("clear");
		strcat(chargement,"####");
		printf("\n\033[1m\t\tTRAITEMENT \033[34m[\033[0m\033[32m%s\033[0m",chargement);
		fflush(stdout);
		sleep(1);
		retardateur ++;
	}
}
void menuGaetan(){
    system("clear");
    printf("\t\t\t\t\t\t#==============================================#\n");
    printf("\t\t\t\t\t\t#    1-Approche par seuillage                  #\n");
    printf("\t\t\t\t\t\t#    2-K-moyenne                               #\n");
    printf("\t\t\t\t\t\t#    3-Division Fusion                         #\n");
    printf("\t\t\t\t\t\t#==============================================#\n");
}

void menuIdriss(){
    system("clear");
    printf("\t\t\t\t\t\t#==============================================#\n");
    printf("\t\t\t\t\t\t#    1-Transformation par morceau              #\n");
    printf("\t\t\t\t\t\t#    2-Transformation lineaire                 #\n");
    printf("\t\t\t\t\t\t#    3-Transformation par saturation           #\n");
    printf("\t\t\t\t\t\t#==============================================#\n");
}


void affichageMenuGenerale(){
    printf("\t\t\t\t\t\t#==============================================#\n");
    printf("\t\t\t\t\t\t#    1-Operation de base sur Image             #\n");
    // printf("\t\t\t\t\t\t#    2-Transformation d’une image              #\n");
    // printf("\t\t\t\t\t\t#    3-Operation arithmetique sur Image        #\n");
    // printf("\t\t\t\t\t\t#    4-CONVOLUTION                             #\n");
    // printf("\t\t\t\t\t\t#    5-Tranformer de fourier                   #\n");
    // printf("\t\t\t\t\t\t#    6-Detection de contour                    #\n");
    printf("\t\t\t\t\t\t#    7-SEGMENTATION IMAGE                      #\n");
    printf("\t\t\t\t\t\t#    8-IMAGE BINAIRE ET OPERATION MORPHOLOGIQUE#\n");
    printf("\t\t\t\t\t\t#==============================================#\n");

}


void menuAbakar(){
    system("clear");
    printf("\t\t\t\t\t\t#==============================================#\n");
    printf("\t\t\t\t\t\t#    1. Additionner deux images PGM            #\n");
    printf("\t\t\t\t\t\t#    2. Soustraire deux images PGM             #\n");
    printf("\t\t\t\t\t\t#    3. Multiplier une image avec une constante#\n");
    printf("\t\t\t\t\t\t#    4. Augmenter la luminance                 #\n");
    printf("\t\t\t\t\t\t#    5. Diminuer la luminance                  #\n");
    printf("\t\t\t\t\t\t#    6. Calculer et afficher l'histogramme     #\n");
    printf("\t\t\t\t\t\t#    7. Calculer et afficher le contraste      #\n");
    printf("\t\t\t\t\t\t#    8. Appliquer un filtre moyen              #\n");
    printf("\t\t\t\t\t\t#    9. Appliquer un filtre median            #\n");
    printf("\t\t\t\t\t\t#    10. Appliquer un filtre gaussien          #\n");
    printf("\t\t\t\t\t\t#==============================================#\n");

}

// =================( idriss )===============================

// PGMImage* readPGMI(const char *filename) {
//     FILE *file = fopen(filename, "rb");
//     if (!file) {
//         fprintf(stderr, "Could not open file %s\n", filename);
//         return NULL;
//     }

//     PGMImage *image = (PGMImage*)malloc(sizeof(PGMImage));
//     if (!image) {
//         fprintf(stderr, "Memory allocation error\n");
//         fclose(file);
//         return NULL;
//     }

//     fscanf(file, "P5\n%d %d\n%d\n", &image->width, &image->height, &image->max_value);

//     image->data = (unsigned char*)malloc(image->width * image->height * sizeof(unsigned char));
//     if (!image->data) {
//         fprintf(stderr, "Memory allocation error\n");
//         free(image);
//         fclose(file);
//         return NULL;
//     }

//     fread(image->data, sizeof(unsigned char), image->width * image->height, file);
//     fclose(file);
//     return image;
// }

// void writePGM(const char *filename, const PGMImage *image) {
//     FILE *file = fopen(filename, "wb");
//     if (!file) {
//         fprintf(stderr, "Could not open file %s\n", filename);
//         return;
//     }

//     fprintf(file, "P5\n%d %d\n%d\n", image->width, image->height, image->max_value);
//     fwrite(image->data, sizeof(unsigned char), image->width * image->height, file);
//     fclose(file);
// }


// void transformation_lineaire_par_morceau(PGMImage *image) {
//     int a1 = 0, b1 = 50;
//     int a2 = 50, b2 = 200;
//     int a3 = 200, b3 = 255;

//     for (int i = 0; i < image->width * image->height; i++) {
//         if (image->data[i] < a2) {
//             image->data[i] = (image->data[i] - a1) * (b2 - b1) / (a2 - a1) + b1;
//         } else if (image->data[i] < a3) {
//             image->data[i] = (image->data[i] - a2) * (b3 - b2) / (a3 - a2) + b2;
//         } else {
//             image->data[i] = b3;
//         }
//     }
// }

// void transformation_lineaire(PGMImage *image, double alpha, double beta) {
//     for (int i = 0; i < image->width * image->height; i++) {
//         image->data[i] = (int)(alpha * image->data[i] + beta);
//         if (image->data[i] > 255) image->data[i] = 255;
//         if (image->data[i] < 0) image->data[i] = 0;
//     }
// }

// void transformation_lineaire_saturation(PGMImage *image) {
//     int min_val = 0;   // Minimum possible value
//     int max_val = 255; // Maximum possible value

//     for (int i = 0; i < image->width * image->height; i++) {
//         image->data[i] = (image->data[i] < min_val) ? min_val : (image->data[i] > max_val) ? max_val : image->data[i];
//     }
// }