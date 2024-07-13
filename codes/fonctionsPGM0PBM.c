#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"fonctionsPGM0PBM.h"

// ============( fonction pour le chargement de l'image pbm et pgm
ImagePGM_PBM* loadImagePGM0PBM(char* fileName){
    FILE* image = fopen(fileName, "rb");
    ImagePGM_PBM *imagePGM_PBM = malloc(sizeof(ImagePGM_PBM));

    if(image == NULL){
        perror("Ce fichiern'exite pas veillez entrer un bon chemin");
        exit(EXIT_FAILURE);
    }

    //-------( recuperation du type )---------
    fscanf(image, "%s\n", imagePGM_PBM->type);

    fscanf(image, "%d %d\n", &imagePGM_PBM->largeur, &imagePGM_PBM->hauteur);

    // ------( recuperation de pixel maximale )---------
    fscanf(image, "%d\n", &imagePGM_PBM->pixel_max);

    //------( lecture des pixels de l'image )----------
    
    int **pixel = (int **)malloc(imagePGM_PBM->hauteur * sizeof(int *));
    for (int i = 0; i < imagePGM_PBM->hauteur; i++){
        pixel[i] = (int *)malloc(imagePGM_PBM->largeur * sizeof(int));
        for(int j = 0; j < imagePGM_PBM->largeur; j++){
            fscanf(image, "%lc", &pixel[i][j]);
            // printf("%c", pixel[i][j]);
        }
    }

    imagePGM_PBM->pixels = pixel;
    fclose(image);
    return imagePGM_PBM;

}

// ============( fonction pour le chargement de l'image pbm et pgm
ImagePGM_PBM* loadImagePBM(char* fileName){
    FILE* image = fopen(fileName, "rb");
    ImagePGM_PBM *imagePGM_PBM = malloc(sizeof(ImagePGM_PBM));

    if(image == NULL){
        perror("Error lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    //-------( recuperation du type )---------
    fscanf(image, "%s\n", imagePGM_PBM->type);
    printf("%s",imagePGM_PBM->type);
    fscanf(image, "%d %d\n", &imagePGM_PBM->largeur, &imagePGM_PBM->hauteur);

    // ------( recuperation de pixel maximale )---------
    // fscanf(image, "%d\n", &imagePGM_PBM->pixel_max);

    //------( lecture des pixels de l'image )----------
    
    int **pixel = (int **)malloc(imagePGM_PBM->hauteur * sizeof(int *));
    for (int i = 0; i < imagePGM_PBM->hauteur; i++){
        pixel[i] = (int *)malloc(imagePGM_PBM->largeur * sizeof(int));
        for(int j = 0; j < imagePGM_PBM->largeur; j++){
            fscanf(image, "%lc", &pixel[i][j]);
            // printf("%c", pixel[i][j]);
        }
    }

    imagePGM_PBM->pixels = pixel;
    fclose(image);
    return imagePGM_PBM;

}

// ===============( fonction permettant d'afficher les pixels de l'image pbm et pgm
void printImageContentPGM0PBM(ImagePGM_PBM image){
    printf("%s\n%d %d\n",image.type, image.largeur, image.hauteur);
    for(int i = 0; i < image.hauteur; i++){
        for(int j = 0; j < image.largeur; j++)
            printf("%d ", image.pixels[i][j]);
    }
}


// =============( fonction permettant de sauvegarder l'image dans un fichier )
void writePBMFile(ImagePGM_PBM *imagePGM0PBM, char *nomFichier){

    // char *name = "traite.";
    char * chemin = "../images/pbm0pgm_traite/";
    char file_name[100];
    


    strcpy(file_name, chemin);
    strcat(file_name, nomFichier);
    FILE * imageFile = fopen(file_name, "wb");


    
    fprintf(imageFile, "%s\n%d %d\n",imagePGM0PBM->type, imagePGM0PBM->largeur, imagePGM0PBM->hauteur);
    
    for(int i = 0; i < imagePGM0PBM->hauteur; i++){
        for(int j = 0; j < imagePGM0PBM->largeur; j++){
            // fprintf(imageFile,"%s ", imagePGM0PBM->pixels[i][j]);
            fwrite(&imagePGM0PBM->pixels[i][j], 1, 1, imageFile);

        }
        // fwrite(&imagePGM0PBM->pixels[i], 1, 1, imageFile);
    
    }
    

    fclose(imageFile);
}

// =============( fonction permettant de sauvegarder l'image dans un fichier )
void writePGM0PBMFile(ImagePGM_PBM *imagePGM0PBM, char *nomFichier){

    // char *name = "traite.";
    // char * chemin = "../images/pbm0pgm_traite/";
    // char file_name[100];
    


    // strcpy(file_name, chemin);
    // strcat(file_name, nomFichier);
    FILE * imageFile = fopen(nomFichier, "wb");


    
    fprintf(imageFile, "%s\n%d %d\n%d\n",imagePGM0PBM->type, imagePGM0PBM->largeur, imagePGM0PBM->hauteur, imagePGM0PBM->pixel_max);
    
    for(int i = 0; i < imagePGM0PBM->hauteur; i++){
        for(int j = 0; j < imagePGM0PBM->largeur; j++){
            // fprintf(imageFile,"%s ", imagePGM0PBM->pixels[i][j]);
            fwrite(&imagePGM0PBM->pixels[i][j], 1, 1, imageFile);

        }
        // fwrite(&imagePGM0PBM->pixels[i], 1, 1, imageFile);
    
    }
    

    fclose(imageFile);
}

// =======================================================================
// ===================== OPERATIONS SUR LES IMAGES =======================
// =======================================================================

// ==========( addition de deux images)
ImagePGM_PBM* additionsImagePGM_PBM(ImagePGM_PBM *image1, ImagePGM_PBM *image2){

    // Pixel *pixel = malloc(image1->hauteur*image1->largeur*sizeof(Pixel));
    int s = 0;
    for(int i = 0; i < image1->hauteur; i++){
        for(int j = 0; j < image1->largeur; j++){
            s = image1->pixels[i][j] + image2->pixels[i][j];
            if(s > 255)
                image1->pixels[i][j] = 255;
            else
                image1->pixels[i][j] = s;
        }
    }
    return image1;
}

// ==========(soustraction de deux images)
ImagePGM_PBM* soustractionImagePGM0PBM(ImagePGM_PBM *image1, ImagePGM_PBM *image2){

    int s = 0;
    for(int i = 0; i < image1->hauteur; i++){
        for(int j = 0; j < image1->largeur; j++){
            s = image1->pixels[i][j] - image2->pixels[i][j];
            if(s < 0)
                image1->pixels[i][j] = 0;
            else
                image1->pixels[i][j] = s;
        }
    }
    return image1;
}

// ==========(multiplication de deux images)
ImagePGM_PBM* produitImagePGM_PBM(ImagePGM_PBM *image1, ImagePGM_PBM *image2){

     int s = 0;
    for(int i = 0; i < image1->hauteur; i++){
        for(int j = 0; j < image1->largeur; j++){
            s = image1->pixels[i][j] * image2->pixels[i][j];
            if(s > 255)
                image1->pixels[i][j] = 255;
            else
                image1->pixels[i][j] = s;
        }
    }
    return image1;
}

// ==================================================================================
// ==================== IMAGE BINAIRE ET OPERATION MORPHOLOGIQUE ====================
// ==================================================================================

// ==========( fonction permettant d'effectuer l'errosion d'une image )
ImagePGM_PBM* erosionPBM(ImagePGM_PBM *image, int** filtre, int tailleMasque) { 
        int height = image->hauteur;
        int width = image->largeur;
        ImagePGM_PBM *imageErode = malloc(sizeof(ImagePGM_PBM));
        imageErode->hauteur = height;
        image->largeur = width;
        imageErode->pixel_max = image->pixel_max;
        strcpy(imageErode->type, image->type);

        int offset = tailleMasque / 2;
        for (int i = offset; i < height - offset; ++i) {
                for (int j = offset; j < width - offset; ++j) {
                    int min = 1;  // For PBM, the minimum value in the neighborhood
                    for (int k = -offset; k <= offset; ++k) {
                        for (int l = -offset; l <= offset; ++l) {
                            if (filtre[k + offset][l + offset] == 1) {
                                if (image->pixels[i + k][j + l] < min) {
                                    min = image->pixels[i + k][j + l];
                                }
                            }
                        }
                    }
                    imageErode->pixels[i][j] = min;
                }
        }
    return imageErode;
}

// =============(fonction permettant d'effectuer l'erosion )
ImagePGM_PBM* erosion(ImagePGM_PBM *image, int** filtre, int tailleMasque) {
    int height = image->hauteur;
    int width = image->largeur;
    ImagePGM_PBM *imageErode = malloc(sizeof(ImagePGM_PBM));
    imageErode->hauteur = height;
    image->largeur = width;
    imageErode->pixel_max = image->pixel_max;
    strcpy(imageErode->type, image->type);

    int offset = tailleMasque / 2;
    for (int i = offset; i < height - offset; ++i) {
        for (int j = offset; j < width - offset; ++j) {
            int min = 255;  // For PGM, the minimum value in the neighborhood
            for (int k = -offset; k <= offset; ++k) {
                for (int l = -offset; l <= offset; ++l) {
                    if (filtre[k + offset][l + offset] == 1) {
                        if (image->pixels[i + k][j + l] < min) {
                            min = image->pixels[i + k][j + l];
                        }
                    }
                }
            }
            imageErode->pixels[i][j] = min;
        }
    }
    return imageErode;
}


// =============(fonction permettant d'effectuer la dilataion )
ImagePGM_PBM* dilatation(ImagePGM_PBM *image, int** mask, int tailleMasque) {
int height = image->hauteur;
    int width = image->largeur;
    ImagePGM_PBM *imageDilater = malloc(sizeof(ImagePGM_PBM));
    imageDilater->hauteur = height;
    image->largeur = width;
    imageDilater->pixel_max = image->pixel_max;
    strcpy(imageDilater->type, image->type);

    int offset = tailleMasque / 2;
    for (int i = offset; i < height - offset; ++i) {
        for (int j = offset; j < width - offset; ++j) {
            unsigned char max = 0;
            for (int k = -offset; k <= offset; ++k) {
                for (int l = -offset; l <= offset; ++l) {
                    if (mask[k + offset][l + offset] && image->pixels[i + k][j + l] > max) {
                        max = image->pixels[i + k][j + l];
                    }
                }
            }
            imageDilater->pixels[i][j] = max;
        }
    }
    return imageDilater;
}

// ======================(Binarisation )
ImagePGM_PBM* binarizePGM(ImagePGM_PBM *image, int seuil) {
    for (int i = 0; i < image->hauteur; ++i) {
        for (int j = 0; j < image->largeur; ++j) {
            image->pixels[i][j] = (image->pixels[i][j] > seuil) ? 255 : 0;
        }
    }
    return image;
}

// =======( labelisation des segment)
ImagePGM_PBM* labelSegments(ImagePGM_PBM *image, int **labels) {
    int label = 1;
    for (int i = 0; i < image->hauteur; ++i) {
        for (int j = 0; j < image->largeur; ++j) {
            if (image->pixels[i][j] == 255 && labels[i][j] == 0) {
                labels = floodFill(image, labels, i, j, label);
                label++;
            }
        }
    }
    return image;
}

int ** floodFill(ImagePGM_PBM *image, int **labels, int x, int y, int label) {
    if (x < 0 || x >= image->hauteur || y < 0 || y >= image->largeur || image->pixels[x][y] == 0 || labels[x][y] != 0) {
        exit;
    }
    labels[x][y] = label;
    floodFill(image, labels, x + 1, y, label);
    floodFill(image, labels, x - 1, y, label);
    floodFill(image, labels, x, y + 1, label);
    floodFill(image, labels, x, y - 1, label);
    return labels;
    
}

// ========( fonction permettant d'effectuer l'ouverture )
ImagePGM_PBM* ouverture(ImagePGM_PBM* image, int** masque, int tailleMasque){
    image = erosion(image, masque, tailleMasque);
    image = dilatation(image, masque, tailleMasque);
    return image;
}

// ========( fonction permettant d'effectuer la fermeture )
ImagePGM_PBM* fermeture(ImagePGM_PBM* image, int** masque, int tailleMasque){
    image = dilatation(image, masque, tailleMasque);
    image = erosion(image, masque, tailleMasque);
    return image;
}
