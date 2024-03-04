#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

#include "SVGwriter.h"
#include "Chaine.h"


int main(int argc, char** argv) {
    FILE * fichier = fopen("00014_burma.cha", "r"); 
    if (fichier == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    Chaines *chaines = lectureChaines(fichier);
    
    fclose(fichier);

    if(argc != 2){
        printf("Il faut 1 argument : nom_du_fichier\n"); // fichier ecriture_chaine.txt
        return 0; 
    }

    fichier = fopen(argv[1], "w"); 
    if (fichier == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    ecrireChaines(chaines, fichier);

    fclose(fichier);

    //Creation de la page html : affichage_chaine.html
    afficheChainesSVG(chaines, "affichage_chaine");

    printf("La longueure physique totale des chaines est de : %.2f\n", longueurTotale(chaines));
    printf("Le nombre total des points est de : %d points\n", comptePointsTotal(chaines));

    liberer_Chaines(chaines);

    return 0;
}
