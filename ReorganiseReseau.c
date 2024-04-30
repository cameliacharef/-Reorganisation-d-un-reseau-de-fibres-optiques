#include <stdio.h>
#include <stdlib.h>
#include "Chaine.h"
#include "Reseau.h"
#include "ArbreQuat.h"
#include "Graphe.h"

int main(int argc, char** argv){
    if(argc != 2){
        printf("Il faut le nom du fichier .cha en paramètre.\n");
    }

    char *nomfic = argv[1]; //le nom du fichier en argument
    
    FILE *f = fopen(nomfic, "r");
    if(f == NULL) {
        printf("Le fichier n'as pas pu être ouvert.\n");
        exit(1);
    }

    Chaines* C = lectureChaines(f);
    fclose(f);
    Reseau* R = reconstitueReseauArbre(C); // Nous utilisons les arbres quaternaires car c'est la méthode la plus rapide d'après l'exercice 6

    //Ex 7.1-7.3 
    Graphe* G = creerGraphe(R);
    int u = 5, v = 4;

    printf("Le nombre d'arêtes du chemin le plus court de %d à %d est %d.\n", u, v, plus_petit_nbChaine(G, u, v));
    printf("Le chemin est :\n");
    /*Cellule_file* L = chaine_arborescence(G, u, v);
    Cellule_file *Lcour = L;
    while(Lcour){
        printf("%d ", Lcour->val);
        Lcour = Lcour->suiv;
    }
    printf("\n");
    
    liberer(L);
    
    // Ex 7.4 
    int test = reorganiseReseau(R); //reorganisation du reseau qui va renvoye un vrai (1) ou faux(0)

    if(test == 0){
        printf(" le reseau n 'est pas optimise \n");
    }else{
        printf(" le reseau  est  optimise \n");
    }*/

    afficher_graph(G);
    libererGraphe(G); 
    liberer_Reseau(R);
    liberer_Chaines(C);
    return 0;
}