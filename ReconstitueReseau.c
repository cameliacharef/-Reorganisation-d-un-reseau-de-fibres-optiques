#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"

int main(){
  if(argc != 3){
        printf("Il faut 2 arguments : le nom du fichier et le numéro de l'action.\n");
        return 0;
  }
  
  FILE * fichier = fopen("00014_burma.cha", "r"); 
  if (fichier == NULL) {
      printf("Error opening file\n");
      return 1;
  }
  Chaines *chaines = lectureChaines(fichier);

  if(argv[1]==1){
    Reseau* R = NULL;
    Noeud* N1 = rechercheCreeNoeudListe(R,0,3);
    Noeud* N2 = rechercheCreeNoeudListe(R,0,3);
    Noeud* N3 = rechercheCreeNoeudListe(R,10,3);
  }else if(argv[1]==2){    
    Reseau* R = reconstitueReseauListe(chaines);
  }else{
    printf("Saisissez un autre nombre.\n");
  }

  fclose(fichier);
  return 0;
}
