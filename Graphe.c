#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "Graphe.h"



Graphe* creerGraphe(Reseau* r){
	//Allocation et initialisation d'un graphe
	Graphe* G = (Graphe *)malloc(sizeof(Graphe));
	G->nbsom= r->nbNoeuds;
	G->gamma= r->gamma;
	G->nbcommod = nbCommodites(r);
	G->T_som = (Sommet**)malloc(G->nbsom*sizeof(Sommet*));
	G->T_commod = (Commod*)malloc(G->nbcommod*sizeof(Commod));
	for(int i=0;i<G->nbsom;i++){
		G->T_som[i] = (Sommet*)malloc(sizeof(Sommet));
	}
	
	//Remplissage du tableau des sommets
	CellNoeud* courant = r->noeuds;
    int i = 0;
	while(courant && i<G->nbsom){
        G->T_som[i]->num =i+1;
        G->T_som[i]->x =courant->nd->x;
        G->T_som[i]->y =courant->nd->y;
        G->T_som[i]->L_voisin=(Cellule_arete*)malloc(sizeof(Cellule_arete));
        while(courant->voisins){
            G->T_som[i]->L_voisin->a->u=G->T_som[i]->num;
            G->T_som[i]->L_voisin->a->v=courant->voisins->num;
            courant->voisins = courant->voisins->suiv;
        }
        courant=courant->suiv;
        i++;
	}
	
	//Remplissage du tableau des commodités
	CellCommodite* courC = r->commodites;
	while(courC){
		G->T_commod->e1 = courC->extraA->num;
        G->T_commod->e2 = courC->extraB->num;
		courC=courC->suiv;
	}
	
	return G;
}

int reorganiseReseau(Reseau *r){

}