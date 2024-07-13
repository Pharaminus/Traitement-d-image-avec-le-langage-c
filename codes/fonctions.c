#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"fonctions.h"

// ============( fonction pour le chargement de l'image ppm
ImagePPM* loadImagePPM(char* fileName){
    FILE* image = fopen(fileName, "rb");

    ImagePPM* dataImage = malloc(sizeof(ImagePPM));

    if(image == NULL){
        perror("Error lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    //-------( recuperation du type )---------
    fscanf(image, "%s\n", dataImage->type);

    fscanf(image, "%d %d\n", &dataImage->largeur, &dataImage->hauteur);

    // ------( recuperation de pixel maximale )---------
    fscanf(image, "%d\n", &dataImage->pixel_max);


    //------( lecture des pixels de l'image )----------

    Pixel *pixel = malloc(dataImage->hauteur * dataImage->largeur * sizeof(Pixel));
    fread(pixel, sizeof(Pixel), dataImage->hauteur*dataImage->largeur, image);

    dataImage->pixel = pixel;
    
    fclose(image);
    return dataImage;
}



// ===============( fonction permettant d'afficher les pixels de l'image ppm
void printImageContent(ImagePPM image){
    printf("%s\n%d %d\n%d\n",image.type, image.largeur, image.hauteur, image.pixel_max);
    for(int i = 0; i < image.hauteur * image.largeur; i++){
        
            printf("\033[91m%d\033[92m%d\033[94m%d\033[0m ", image.pixel[i].red, image.pixel[i].green, image.pixel[i].blue);
    }
}



// =============( fonction permettant de sauvegarder l'image dans un fichier )
void writePPMFile(ImagePPM *imagePPM, char *nomFichier){

    // char *name = "traite.";
    char * chemin = "../images/ppm_traite/";
    char file_name[100];
    


    strcpy(file_name, chemin);
    strcat(file_name, nomFichier);
    FILE * imageFile = fopen(file_name, "wb");


    
    fprintf(imageFile, "%s\n%d %d\n%d\n",imagePPM->type, imagePPM->largeur, imagePPM->hauteur, imagePPM->pixel_max);
    
    for(int i = 0; i < imagePPM->largeur * imagePPM->hauteur; i++){
        fwrite(&imagePPM->pixel[i].red, 1, 1, imageFile);
        fwrite(&imagePPM->pixel[i].green, 1, 1, imageFile);
        fwrite(&imagePPM->pixel[i].blue, 1, 1, imageFile);
    }

    fclose(imageFile);
}

// =======================================================================
// ===================== OPERATIONS SUR LES IMAGES =======================
// =======================================================================

// ==========( addition de deux images)
ImagePPM* additionsImagePPM(ImagePPM *image1, ImagePPM *image2){

    Pixel *pixel = malloc(image1->hauteur*image1->largeur*sizeof(Pixel));
    for(int i = 0; i < image1->hauteur * image1->largeur; i++){
        if(image1->pixel[i].blue + image2->pixel[i].blue > 255)
            pixel[i].blue = 255;
        else
            pixel[i].blue = (image1->pixel[i].blue + image2->pixel[i].blue);

        if(image1->pixel[i].green + image2->pixel[i].green > 255)
            pixel[i].green = 255;
        else
            pixel[i].green = (image1->pixel[i].green + image2->pixel[i].green);
        
        if(image1->pixel[i].red + image2->pixel[i].red > 255)
            pixel[i].red = 255;
        else
            pixel[i].red = (image1->pixel[i].red + image2->pixel[i].red);
    }
    image1->pixel = pixel;
    return image1;
}

// ==========(soustraction de deux images)
ImagePPM* soustractionImagePPM(ImagePPM *image1, ImagePPM *image2){

    Pixel *pixel = malloc(image1->hauteur*image1->largeur*sizeof(Pixel));
    for(int i = 0; i < image1->hauteur * image1->largeur; i++){
        // if((image1->pixel[i].blue - image2->pixel[i].blue) <= 0)
        //     pixel[i].blue = 0;
        // else
            pixel[i].blue = image1->pixel[i].blue - image2->pixel[i].blue;

        // if((image1->pixel[i].green - image2->pixel[i].green) <= 0)
        //     pixel[i].green = 0;
        // else
            pixel[i].green = image1->pixel[i].green - image2->pixel[i].green;
        
        // if((image1->pixel[i].red - image2->pixel[i].red) <= 0)
        //     pixel[i].red = 0;
        // else
            pixel[i].red = image1->pixel[i].red - image2->pixel[i].red;
    }
    image1->pixel = pixel;
    return image1;
}

// ==========(multiplication de deux images)
ImagePPM* produitImagePPM(ImagePPM *image1, ImagePPM *image2){

    Pixel *pixel = malloc(image1->hauteur*image1->largeur*sizeof(Pixel));
    for(int i = 0; i < image1->hauteur * image1->largeur; i++){
        if(image1->pixel[i].blue * image2->pixel[i].blue > 255)
            pixel[i].blue = 255;
        else
            pixel[i].blue = (image1->pixel[i].blue * image2->pixel[i].blue);

        if(image1->pixel[i].green * image2->pixel[i].green > 255)
            pixel[i].green = 255;
        else
            pixel[i].green = (image1->pixel[i].green * image2->pixel[i].green);
        
        if(image1->pixel[i].red * image2->pixel[i].red > 255)
            pixel[i].red = 255;
        else
            pixel[i].red = (image1->pixel[i].red * image2->pixel[i].red);
    }
    image1->pixel = pixel;
    return image1;
}


// ==================================================================================
// ==================== IMAGE BINAIRE ET OPERATION MORPHOLOGIQUE ====================
// ==================================================================================

// ==========( fonction permettant d'effectuer la dilatation d'une image )






