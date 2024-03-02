#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

#include "Chaine.h"

int main (){

    FILE * fichier = fopen("00014_burma.cha", "r"); 

        Chaines *chaines = lectureChaines(fichier);
        fclose(fichier);

        // Affichez les chaines
        printf("Chaines lues du fichier :\n");
        afficher_Chaines(chaines);
        
    
    return 0;
}
