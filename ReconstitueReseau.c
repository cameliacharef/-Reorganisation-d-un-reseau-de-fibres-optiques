#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"

int main(int argc, char* argv[]){
  if(argc != 3){
        printf("Il faut 2 arguments : le nom du fichier et le numéro de l'action.\n");
        return 0;
  }
  
  FILE * fl = fopen("00014_burma.cha", "r"); 
  if (fl == NULL) {
      printf("Error opening file\n");
      return 1;
  }
  Chaines *chaines = lectureChaines(fl);

  if(atoi(argv[2])==1){
    Reseau* R = reconstitueReseauListe(chaines);
    Noeud* N1 = rechercheCreeNoeudListe(R,10.00,31.23);
    Noeud* N2 = rechercheCreeNoeudListe(R,10.00,31.23);
    Noeud* N3 = rechercheCreeNoeudListe(R,20.09,92.54);

    int nbC = nbCommodites(R);
    int nbL = nbLiaisons(R);
    printf("Nombre de commodités : %d\nNombre de liaisons : %d\n",nbC,nbL);

    FILE* fe = fopen("test_exo3.res","w");
    ecrireReseau(R,fe);
    //afficheReseauSVG(R,???);
    fclose(fe);
  }else if(atoi(argv[2])==2){    
    printf("Méthode non encore mise en place\n");
  }else if(atoi(argv[2])==3){    
    printf("Méthode non encore mise en place\n");
  }else{
    printf("Saisissez un autre nombre.\n");
  }

  fclose(fl);
  return 0;
}
