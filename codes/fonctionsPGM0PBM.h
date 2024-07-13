#ifndef __FONCTIONS_PGM0PBM__
#define __FONCTIONS_PGM0PBM__


// -------( structure pour stocker les caracteristiques de l'image PGM et PBM )-----
typedef struct ImagePGM_PBM
{
    char type[3];
    int hauteur;
    int largeur;
    int pixel_max;
    char *commentaire;
    int **pixels;
}ImagePGM_PBM;

// ================( prototype des fonctions )==============

ImagePGM_PBM* loadImagePGM0PBM(char* fileName);
ImagePGM_PBM* loadImagePBM(char* fileName);
void writePGM0PBMFile(ImagePGM_PBM *imagePGM0PBM, char *nomFichier);
void writePBMFile(ImagePGM_PBM *imagePGM0PBM, char *nomFichier);
void printImageContentPGM0PBM(ImagePGM_PBM image);

// ===================================================================
// ===================================================================
// ===================================================================

// ==========( prototype des fonctions pour les operations sur les images )===========
ImagePGM_PBM* additionsImagePGM_PBM(ImagePGM_PBM *image1, ImagePGM_PBM *image2);
ImagePGM_PBM* soustractionImagePGM0PBM(ImagePGM_PBM *image1, ImagePGM_PBM *image2);
ImagePGM_PBM* produitImagePGM_PBM(ImagePGM_PBM *image1, ImagePGM_PBM *image2);
ImagePGM_PBM* erosionPBM(ImagePGM_PBM *image, int** filtre, int tailleMasque);

ImagePGM_PBM* erosion(ImagePGM_PBM *image, int** filtre, int tailleMasque);
ImagePGM_PBM* dilatation(ImagePGM_PBM *image, int** mask, int tailleMasque);
ImagePGM_PBM* ouverture(ImagePGM_PBM* image, int** masque, int tailleMasque);
ImagePGM_PBM* fermeture(ImagePGM_PBM* image, int** masque, int tailleMasque);
ImagePGM_PBM* binarizePGM(ImagePGM_PBM *image, int seuil);
ImagePGM_PBM* labelSegments(ImagePGM_PBM *image, int **labels);
int ** floodFill(ImagePGM_PBM *image, int **labels, int x, int y, int label);






#endif

