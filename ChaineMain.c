#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

#include "Chaine.h"

int main() {
    FILE * fichier = fopen("00014_burma.cha", "r"); 
    if (fichier == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    Chaines *chaines = lectureChaines(fichier);
    
    fclose(fichier);

    if(argc != 2){
        printf("Il faut 1 argument : nom_du_fichier\n");
        return 0; 
    }

    fichier = fopen(argv[1], "w"); 
    if (fichier == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    ecrireChaines(chaines, fichier);

    fclose(fichier);
    liberer_Chaines(chaines);

    return 0;
}

