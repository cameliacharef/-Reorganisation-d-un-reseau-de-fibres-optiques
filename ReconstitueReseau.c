#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include "Hachage.h"

// Menu pour choisir la methode 
void menu(){
    printf("1 - Reconstitution du reseau par methode : LISTE\n");
    printf("2 - Reconstitution du reseau par methode : TABLE DE HACHAGE \n");
    printf("3 - Reconstitution du reseau par methode : ARBRE \n");
}

/*int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Il faut 2 arguments : le nom du fichier .cha et le numéro de la methode.\n"); // test_exo3.
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

    Erreur dans le test de la question 2 de l'exercice 2 :
    nbNoeuds et gamma sont incroyablement très grands
    printf("Nombre de noeuds = %d Gamma = %d\n",R->nbNoeuds,R->gamma);

    Test de la question 1 de l'exercice 2
    Noeud* N1 = rechercheCreeNoeudListe(R,10.00,31.23);
    printf("x = %.2f y = %.2f\n",N1->x,N1->y);
    Noeud* N2 = rechercheCreeNoeudListe(R,10.00,31.23);
    printf("x = %.2f y = %.2f\n",N2->x,N2->y);*/

    /*Test de la question 1 de l'exercice 3
    int nbC = nbCommodites(R);
    int nbL = nbLiaisons(R);
    printf("Nombre de commodités : %d\nNombre de liaisons : %d\n",nbC,nbL);

    FILE* fe = fopen("test_exo3.res","w");
    ecrireReseau(R,fe);
    //afficheReseauSVG(R,???);
    fclose(fe);
    liberer_Reseau(R);
  }else if(atoi(argv[2])==2){    
    printf("Méthode non encore mise en place\n");
  }else if(atoi(argv[2])==3){    
    printf("Méthode non encore mise en place\n");
  }else{
    printf("Saisissez un autre nombre.\n");
  }

  
  liberer_Chaines(chaines);
  fclose(fl);
  return 0;
}*/

int main(int argc , char *argv[]){
    if(argc != 3){
        printf("Il faut 2 arguments : le nom du fichier .cha et le numéro de la methode.\n"); // "00014_burma.cha"
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

    //int reponse = atoi(argv[2]);
    int stop = 1;
	char buffer[256];
    menu();
	printf("saisir l'action a effectuer.\n");
	fgets(buffer, sizeof(int), stdin);
	stop = atoi(buffer);

    switch (stop){
    // cas des listes chaine
    case 1: ;
        Reseau * R = reconstitueReseauListe(chaines);
        printf("Nombre de noeuds = %d Gamma = %d\n", R->nbNoeuds, R->gamma);

        //Test de la question 1 de l'exercice 2
        //Noeud * N1 = rechercheCreeNoeudListe(R, 10.00, 31.23);
        //printf("x = %.2f y = %.2f\n", N1->x, N1->y);
        //Noeud * N2 = rechercheCreeNoeudListe(R, 10.00, 31.23);
        //printf("x = %.2f y = %.2f\n", N2->x, N2->y);

        //Test de la question 1 de l'exercice 3
        int nbC = nbCommodites(R);
        int nbL = nbLiaisons(R);
        printf("Nombre de commodités : %d\nNombre de liaisons : %d\n", nbC, nbL);

        FILE * fe = fopen("test_exo3.res", "w");
        ecrireReseau(R, fe);
        afficheReseauSVG(R, "affichagereseauListe");
        fclose(fe);

        liberer_Reseau(R);

        printf("Affichage du reseau dans le fichier html affchagereseauListe\n");
        break;
    case 2 : ;
        R = reconstitueReseauHachage(chaines, 1000);
        printf("Nombre de noeuds = %d Gamma = %d\n", R->nbNoeuds, R->gamma);

        //Test de la question 1 de l'exercice 2
        //Noeud * N1 = rechercheCreeNoeudListe(R, 10.00, 31.23);
        //printf("x = %.2f y = %.2f\n", N1->x, N1->y);
        //Noeud * N2 = rechercheCreeNoeudListe(R, 10.00, 31.23);
        //printf("x = %.2f y = %.2f\n", N2->x, N2->y);

        //Test de la question 1 de l'exercice 3 
        nbC = nbCommodites(R);
        nbL = nbLiaisons(R);
        printf("Nombre de commodités : %d\nNombre de liaisons : %d\n", nbC, nbL);

        fe = fopen("test_exo4.res", "w");
        ecrireReseau(R, fe);
        afficheReseauSVG(R, "affchagereseauHachage");
        fclose(fe);

        liberer_Reseau(R);
        printf("Affichage du reseau dans le fichier html affchagereseauHachage\n");
        break ;
    case 3 : ;
        printf("Méthode non encore mise en place\n");
        break; 
    
    default: ;
        printf("veuillez suivre l'indication du menu");
        break;
    }

    liberer_Chaines(chaines);


    return 0;


}
