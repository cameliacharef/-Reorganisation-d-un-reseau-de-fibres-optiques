#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include <stdlib.h>
#include <stdio.h>
#include "Struct_File.h"
#include "Reseau.h"

typedef struct {
    int u , v; /* Numeros des sommets extremite */
} Arete ;

typedef struct cellule_arete {
    Arete * a ; /* pointeur sur l’arete */
    struct cellule_arete * suiv ;
} Cellule_arete ;

typedef struct {
    int num ; /* Numero du sommet (le meme que dans T_som) */
    double x , y ;
    Cellule_arete * L_voisin ; /* Liste chainee des voisins */
} Sommet ;

typedef struct {
    int e1 , e2 ; /* Les deux extremites de la commodite */
} Commod ;

typedef struct {
    int nbsom ; /* Nombre de sommets */
    Sommet ** T_som ; /* Tableau de pointeurs sur sommets */
    int gamma ;
    int nbcommod ; /* Nombre de commodites */
    Commod * T_commod ; /* Tableau des commodites */
} Graphe ;

int plus_petit_nbChaine(Graphe* G, int u , int v);
Cellule_file * chaine_arborescence(Graphe * G, int u , int  v);
int reorganiseReseau(Reseau *r);

/*Creation*/
Sommet * creerSommet(int num, double x, double y);
Arete* creerArete(int u,int v);
Graphe* creerGraphe(Reseau* r);

/*Libération*/
void libererSommet(Sommet *s);
void libererCellule_Arete(Cellule_arete *c);
void libererGraphe(Graphe* G);
void supprimerChemin(Cellule_file* chemin);
void liberer_file(S_file* F);

/*Insertion*/
void insereArete(Arete* a,Cellule_arete** c);
void insererEnTeteFile(Cellule_file **f, int val);

/*Affichage*/
void afficher_graph(Graphe* g);
void afficherChemin(Cellule_file* L, int u, int v);









# endif