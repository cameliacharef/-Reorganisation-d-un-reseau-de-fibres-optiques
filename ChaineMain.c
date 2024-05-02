#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "SVGwriter.h"
#include "Chaine.h"


int main(int argc, char** argv) {
    if(argc != 2){
        printf("Il faut 1 argument : nom_du_fichier\n"); // 00014_burma.cha
        return 0; 
    }
    char *nomfic = argv[1]; //fichier en argument
    FILE * fl = fopen(nomfic, "r"); 
    if (fl == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    Chaines *chaines = lectureChaines(fl);
    fclose(fl);

    FILE * fichier = fopen("ecriture_Chaines.txt", "w"); 
    if (fichier == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    ecrireChaines(chaines, fichier);

    fclose(fichier);

    //Creation de la page html : affichage_chaine.html
    afficheChainesSVG(chaines, "affichage_chaine");

    /*Test Fonctions Q 1.4, Q 1.5*/
    printf("La longueure physique totale des chaines est de : %.2f\n", longueurTotale(chaines));
    printf("Le nombre total des points est de : %d points\n", comptePointsTotal(chaines));

    liberer_Chaines(chaines);

    return 0;
}
