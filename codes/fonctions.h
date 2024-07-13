#ifndef __FONCTIONS__
#define __FONCTIONS__


// =============( struture de donne pour l'image)============

// ----( structure permettant de stocker les pixels de l'image )------
typedef struct Pixel{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}Pixel;

// -------( structure pour stocker les caracteristiques de l'image PPM )-----
typedef struct ImagePPM
{
    char type[3];
    int hauteur;
    int largeur;
    int pixel_max;
    char *commentaire;
    struct Pixel *pixel;
}ImagePPM;




// ================( prototype des fonctions )==============
ImagePPM* loadImagePPM(char* fileName);
void printImageContent(ImagePPM image);
void writePPMFile(ImagePPM *imagePPM, char *fileName);


// ===================================================================
// ===================================================================
// ===================================================================

// ==========( prototype des fonctions pour les operations sur les images )===========
ImagePPM* additionsImagePPM(ImagePPM *image1, ImagePPM *image2);
ImagePPM* soustractionImagePPM(ImagePPM *image1, ImagePPM *image2);
ImagePPM* produitImagePPM(ImagePPM *image1, ImagePPM *image2);





#endif