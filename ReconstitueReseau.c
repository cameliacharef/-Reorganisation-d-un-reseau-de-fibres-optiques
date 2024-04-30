#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include <string.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include "Hachage.h"
#include "ArbreQuat.h"

// Menu pour choisir la méthode 
void menu(){
    printf("1 - Reconstitution du réseau par la méthode : LISTE CHAINEE\n");
    printf("2 - Reconstitution du réseau par la méthode : TABLE DE HACHAGE \n");
    printf("3 - Reconstitution du réseau par la méthode : ARBRE QUATERNAIRE\n");
    printf("\n");
}

int main(int argc , char *argv[]){
    if(argc != 2){
        printf("Il faut 1 argument : le nom du fichier d'extension .cha.\n");
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

    int stop = 1;
	char buffer[256];
    menu();
	printf("\nSaisissez l'action (nombre entier) à effectuer.\n");
	fgets(buffer, sizeof(int), stdin);
	stop = atoi(buffer);
    printf("\n");
    char* c = strchr(nomfic,'.'); //séparation de l'extension du nom du fichier
    *c = '\0';
    char name[100] = "";

    switch (stop){
    // cas des listes chaînées
    case 1: ;
        Reseau * R = reconstitueReseauListe(chaines);
        printf("Nombre de noeuds = %d\nGamma = %d\n", R->nbNoeuds, R->gamma);

        //Test de la question 1 de l'exercice 3
        int nbC = nbCommodites(R);
        int nbL = nbLiaisons(R);
        printf("Nombre de commodités : %d\nNombre de liaisons : %d\n", nbC, nbL);

        FILE * fe = fopen("test_exo3.res", "w");
        ecrireReseau(R, fe);
        strcat(name,"affichageReseauListe");
        strcat(name,nomfic);
        afficheReseauSVG(R, name);
        fclose(fe);

        liberer_Reseau(R);
        printf("Affichage du réseau dans le fichier html : %s\n",name);
        break;
    case 2 : ;
        R = reconstitueReseauHachage(chaines, 1000);
        printf("Nombre de noeuds = %d\nGamma = %d\n", R->nbNoeuds, R->gamma);

        //Test de la question 1 de l'exercice 3 
        nbC = nbCommodites(R);
        nbL = nbLiaisons(R);
        printf("Nombre de commodités : %d\nNombre de liaisons : %d\n", nbC, nbL);

        fe = fopen("test_exo4.res", "w");
        ecrireReseau(R, fe);
        strcat(name,"affichageReseauHachage");
        strcat(name,nomfic);
        afficheReseauSVG(R, name);
        fclose(fe);

        liberer_Reseau(R);
        printf("Affichage du réseau dans le fichier html : %s\n",name);
        break ;
    case 3 : ;
        R = reconstitueReseauArbre(chaines);
        printf("Nombre de noeuds = %d\nGamma = %d\n", R->nbNoeuds, R->gamma);

        //Test de la question 1 de l'exercice 3 
        nbC = nbCommodites(R);
        nbL = nbLiaisons(R);
        printf("Nombre de commodités : %d\nNombre de liaisons : %d\n", nbC, nbL);

        fe = fopen("test_exo5.res", "w");
        ecrireReseau(R, fe);
        strcat(name,"affichageReseauArbre");
        strcat(name,nomfic);
        afficheReseauSVG(R, name);
        fclose(fe);

        liberer_Reseau(R);
        printf("Affichage du réseau dans le fichier html : %s\n",name);
        break ;
    
    default: ;
        printf("Veuillez suivre l'indication du menu.\n");
        break;
    }

    liberer_Chaines(chaines);

    return 0;
}
