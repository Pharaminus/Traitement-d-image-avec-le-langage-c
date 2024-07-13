#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include "entete.h"
#include "fonction.c"

char* identify_file_type(char* nom_du_fichier) {
    char *recherche_extension = strrchr(nom_du_fichier, '.');
    if (!recherche_extension || recherche_extension == nom_du_fichier) return NULL;
    if (strcmp(recherche_extension, ".pbm") == 0) return "PBM";
    if (strcmp(recherche_extension, ".pgm") == 0) return "PGM";
    if (strcmp(recherche_extension, ".ppm") == 0) return "PPM";
    return "UNKNOWN";
}

void list_files_in_folder(const char *folder_path) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                printf("[  %s  ]", ent->d_name);
            }
        }
        closedir(dir);
    } else {
        printf("Erreur d'ouverture du dossier\n");
    }
}

char* selection_de_Image(const char *dossier_image) {
    char *nom_image = (char *)malloc(sizeof(char) * 256);
    if (!nom_image) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return NULL;
    }
    printf("Entrez le nom de l'image: ");
    scanf("%255s", nom_image);
    printf("Nom de l'image : {%s}\n", nom_image);

    char imagePath[512];
    snprintf(imagePath, sizeof(imagePath), "%s/%s", dossier_image, nom_image);

    if (access(imagePath, F_OK) == -1) {
        DIR *dir = opendir(dossier_image);
        if (dir) {
            struct dirent *ent;
            while ((ent = readdir(dir)) != NULL) {
                if (strcmp(ent->d_name, nom_image) == 0) {
                    printf("Le nom saisi correspond à un dossier, pas à un fichier image.\n");
                    closedir(dir);
                    free(nom_image);
                    return NULL;
                }
            }
            closedir(dir);
        }
        printf("Image absente du répertoire\n");
        free(nom_image);
        return NULL;
    } else {
        printf("\nChargement de l'image en cours : [");
        for (int i = 0; i < 20; i++) {
            printf("#");
            fflush(stdout);
            struct timespec ts = {0, 100000000};
            nanosleep(&ts, NULL);
        }
        printf("] 100%%\n");
    }

    return nom_image;
}

void transformation_image(char *filename) {
    printf("========= Transformeur =========\n");
    printf("1 - Transformation linéaire\n");
    printf("2 - Transformation linéaire avec saturation\n");
    printf("3 - Transformation linéaire par morceau\n");
    printf("4 - Transformation non linéaire\n");
    printf("5 - Egalisation d'Histogramme\n");
    printf("6 - Dynamique de l'image\n");
    printf("7 - LUT (Look Up Table) de l'image\n");

    int choix;
    printf("\nEntrez l'opération : ");
    scanf("%d", &choix);

    // Afficher le chemin du fichier pour débogage
    printf("Chemin du fichier à lire : %s\n", filename);

    // Vérifier si le fichier existe avant de le lire
    if (access(filename, F_OK) == -1) {
        printf("Erreur : le fichier n'existe pas.\n");
        return;
    }

    PGMImage *image = readPGM(filename);
    if (!image) {
        printf("Erreur lors de la lecture de l'image\n");
        return;
    }

    switch (choix) {
    case 1:
        transformation_lineaire(image, 1.0, 0.0); // Example values
            writePGM("SAVE/output_linear.pgm", image);
        break;
    case 2:
        transformation_lineaire_saturation(image);
        writePGM("SAVE/output_saturation.pgm", image);
        break;
    case 3:
        transformation_lineaire_par_morceau(image);
        writePGM("SAVE/output_piecewise.pgm", image);
        break;
    case 5:
        egalisation_histogramme(image);
        writePGM("SAVE/output_equalized.pgm", image);
        break;
    default:
        printf("Opération non reconnue\n");
        break;
    }

    freePGM(image);
}

void detecter_contours(char *filename) {
    printf("Détecteur de contours...\n");
    printf("• 1 - Filtre de Robert\n");
    printf("• 2 - Filtre de Prewitt\n");
    printf("• 3 - Filtre de Sobel\n");
    printf("• 4 - Filtre de Laplacien\n");
    printf("• 5 - Transformée de Hough\n");

    int choix;
    printf("\nEntrez l'opération : ");
    scanf("%d", &choix);

    // Afficher le chemin du fichier pour débogage
    printf("Chemin du fichier à lire : %s\n", filename);

    // Vérifier si le fichier existe avant de le lire
    if (!access(filename, F_OK) == -1) {
        printf("succes.\n");
        return;
    }

    PGMImage *image = readPGM(filename);
    if (!image) {
        printf("Erreur lors de la lecture de l'image\n");
        return;
    }

    switch (choix) {
    case 5: {
        int *accumulator;
        int acc_width, acc_height;
        hough_transform(image, &accumulator, &acc_width, &acc_height);
        draw_hough_lines(image, accumulator, acc_width, acc_height, 150); // Example threshold
        writePGM("output_hough.pgm", image);
        free(accumulator);
        break;
    }
    default:
        printf("Opération non reconnue\n");
        break;
    }

    freePGM(image);
}

void operation(char *filename) {
    int choix;
    printf("Choisissez une opération :\n");
    printf("1 - Transformation d'image\n");
    printf("2 - Détection de contours\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    switch (choix) {
    case 1:
        transformation_image(filename);
        break;
    case 2:
        detecter_contours(filename);
        break;
    default:
        printf("Choix invalide\n");
        break;
    }
}

int main() {
    int saisie;
    char *stock_image = NULL;
    const char folder_path[] = "ImageFold/";

    while (1) {
        printf("\nEntrez le traitement (1: Lister fichiers,\t2: Sélectionner image,\t3: Opération,\t0: Quitter) : ");
        scanf("%d", &saisie);

        switch (saisie) {
        case 1:
            list_files_in_folder(folder_path);
            break;
        case 2:
            if (stock_image) {
                free(stock_image);
            }
            stock_image = selection_de_Image(folder_path);
            break;
        case 3:
            if (stock_image) {
                operation(stock_image);
            } else {
                printf("Aucune image sélectionnée\n");
            }
            break;
        case 0:
            if (stock_image) {
                free(stock_image);
            }
            return 0;
        default:
            printf("Choix invalide\n");
            break;
        }
    }

    return 0;
}
