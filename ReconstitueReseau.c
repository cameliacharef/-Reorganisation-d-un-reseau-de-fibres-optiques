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

    /*Erreur dans le test de la question 2 de l'exercice 2 :
    nbNoeuds et gamma sont incroyablement très grands*/
    printf("Nombre de noeuds = %d Gamma = %d\n",R->nbNoeuds,R->gamma);

    /*Test de la question 1 de l'exercice 2*/
    Noeud* N1 = rechercheCreeNoeudListe(R,10.00,31.23);
    printf("x = %.2f y = %.2f\n",N1->x,N1->y);
    Noeud* N2 = rechercheCreeNoeudListe(R,10.00,31.23);
    printf("x = %.2f y = %.2f\n",N2->x,N2->y);

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
