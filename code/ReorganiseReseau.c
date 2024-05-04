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
    Reseau* R = reconstitueReseauArbre(C); // Nous utilisons les arbres quaternaires car c'est la méthode la plus rapide d'après l'exercice 6.

    // Ex 7.1 
    /* Affichage du graphe */
    printf("\nAFFICHAGE DU GRAPHE G:\n");
    Graphe* G = creerGraphe(R);
    afficher_graph(G);

    printf("\nJEUX DE TESTS Q7.2 , Q7.3\n");
    //Ex 7.2 , 7.3
    /*Test des fonctions de parcours en largeurs*/
    int u =  1, v = 12;
    printf("Le nombre d'arêtes du chemin le plus court de %d à %d est %d.\n\n", u, v, plus_petit_nbChaine(G, u, v));
    Cellule_file* L = chaine_arborescence(G, u, v);
    afficherChemin(L, u, v);
    supprimerChemin(L);

    u =  5, v = 11;
    printf("Le nombre d'arêtes du chemin le plus court de %d à %d est %d.\n\n", u, v, plus_petit_nbChaine(G, u, v));
    Cellule_file* L2 = chaine_arborescence(G, u, v);
    afficherChemin(L2, u, v);
    supprimerChemin(L2);

    u =  0, v = 0;
    printf("Le nombre d'arêtes du chemin le plus court de %d à %d est %d.\n\n", u, v, plus_petit_nbChaine(G, u, v));
    Cellule_file* L3 = chaine_arborescence(G, u, v);
    afficherChemin(L3, u, v);
    supprimerChemin(L3);

    
    printf("\nReorganisation du reseau :\n");
    // Ex 7.4 
    int test = reorganiseReseau(R); //Réorganisation du reseau qui va renvoyer un vrai (1) ou faux (0)

    if(test == 0){
        printf("Le reseau n 'est pas optimise \n");
    }
    else{
        printf("Le reseau  est  optimise \n");
    }

    
    /*Libération des structures*/
    libererGraphe(G); 
    liberer_Reseau(R);
    liberer_Chaines(C);
    return 0;
}
