#include <stdio.h>
#include <string.h>

#define WIDTH 160 // Double the width to make it larger

void printLine() {
    for (int i = 0; i < WIDTH; i++) {
        printf("*");
    }
    printf("\n");
}

void printEmptyLine() {
    printf("*");
    for (int i = 1; i < WIDTH - 1; i++) {
        printf(" ");
    }
    printf("*\n");
}

void centerText(const char* text) {
    int length = strlen(text);
    int padding = (WIDTH - length) / 2;
    printf("*");
    for (int i = 1; i < padding; i++) {
        printf(" ");
    }
    printf("%s", text);
    for (int i = padding + length; i < WIDTH - 1; i++) {
        printf(" ");
    }
    printf("*\n");
}

void afficherMenu() {
    printLine();
    for (int i = 0; i < 3; i++) printEmptyLine();
    centerText("Traitement d'image en C UE 3721");
    for (int i = 0; i < 3; i++) printEmptyLine();
    printLine();
    for (int i = 0; i < 2; i++) printEmptyLine();
    centerText("1  -  Afficher les images");
    for (int i = 0; i < 2; i++) printEmptyLine();
    centerText("2  -  Sélection d'image");
    for (int i = 0; i < 2; i++) printEmptyLine();
    centerText("3  -  Poursuivre en traitement d'image");
    for (int i = 0; i < 2; i++) printEmptyLine();
    centerText("4  -  Enregistrer le fichier");
    for (int i = 0; i < 2; i++) printEmptyLine();
    centerText("5  -  Quitter le programme");
    for (int i = 0; i < 2; i++) printEmptyLine();
    printLine();
}

void afficherImages() {
    // Placeholder for actual code to display images
    printf("Afficher les images...\n");
}

void selectionnerImage() {
    // Placeholder for actual code to select an image
    printf("Sélectionner une image...\n");
}

void traiterImage() {
    // Placeholder for actual code to process an image
    printf("Poursuivre en traitement d'image...\n");
}

void enregistrerFichier() {
    // Placeholder for actual code to save a file
    printf("Enregistrer le fichier...\n");
}

int main() {
    int choix;
    do {
        afficherMenu();
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherImages();
                break;
            case 2:
                selectionnerImage();
                break;
            case 3:
                traiterImage();
                break;
            case 4:
                enregistrerFichier();
                break;
            case 5:
                printf("Quitter le programme...\n");
                break;
            default:
                printf("Option invalide, veuillez réessayer.\n");
        }
    } while (choix != 5);

    return 0;
}
