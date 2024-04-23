#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "Graphe.h"

Arete* creerArete(int u,int v){
	Arete* a= (Arete*)malloc(sizeof(Arete));
	a->u = u;
	a->v = v;
	return a;
}

void insereArete(Arete* a,Cellule_arete* c){
	Cellule_arete* new = (Cellule_arete*)malloc(sizeof(Cellule_arete));
	new->suiv = c;
	c=new;
}

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

		//Création de la liste des voisins
        while(courant->voisins){
            Arête* a = creerArete(G->T_som[i]->num,courant->voisins->num); //Les arêtes sont incidentes au sommet i
			insereArete(a,G->T_som[i]->L_voisin);
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