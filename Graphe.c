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
	new->a = a;
	new->suiv = c;
	c = new;
}

Graphe* creerGraphe(Reseau* r){
	//Allocation et initialisation d'un graphe
	Graphe* G = (Graphe *)malloc(sizeof(Graphe));
	G->nbsom = r->nbNoeuds;
	G->gamma = r->gamma;
	G->nbcommod = nbCommodites(r);
	G->T_som = (Sommet**)malloc(G->nbsom * sizeof(Sommet*));
	G->T_commod = (Commod*)malloc(G->nbcommod * sizeof(Commod));
	for(int i = 0; i < G->nbsom; i++){
		G->T_som[i] = (Sommet*)malloc(sizeof(Sommet));
	}
	
	//Remplissage du tableau des sommets
	CellNoeud* courant = r->noeuds;
	while(courant){
		//initialisation de tous les sommets avec leur coordonnes avec pour chaque sommet leur liste de voisin 
        G->T_som[courant->nd->num - 1]->num = courant->nd->num;
        G->T_som[courant->nd->num - 1]->x = courant->nd->x;
        G->T_som[courant->nd->num - 1]->y = courant->nd->y;
		G->T_som[courant->nd->num - 1]->L_voisin = NULL;

        //G->T_som[courant->nd->num - 1]->L_voisin = (Cellule_arete*)malloc(sizeof(Cellule_arete));


		//Création de la liste des voisins
		CellNoeud *voisins = courant->nd->voisins;
        while(voisins){
			/*Chaque arete {u,v} du graphe est alloue une seule fois, mais apparait dans la liste des voisins
			du sommet u et du sommet v dans le graphe.*/
			if(courant->nd->num < voisins->nd->num){ 
				Arete* a = creerArete(courant->nd->num, voisins->nd->num); //Les arêtes sont incidentes au sommet i
				insereArete(a,G->T_som[a->u - 1]->L_voisin); // Ajout dans u
				insereArete(a,G->T_som[a->v - 1]->L_voisin); // Ajout dans v
			}
            
            voisins = voisins->suiv;
        }
		
        courant = courant->suiv;
	}
	
	//Remplissage du tableau des commodités
	CellCommodite* courC = r->commodites;
	int cpt = 0;
	while(courC){
		(G->T_commod)[cpt].e1 = courC->extrA->num;
        (G->T_commod)[cpt].e2 = courC->extrB->num;
		courC = courC->suiv;
		cpt++;
	}
	
	return G;
}
int plus_petit_nbChaine(Graphe* g, int u , int v){
	if (u == v){
        return 0 ; // le chemin nulle 
    }
}
int reorganiseReseau(Reseau *r){

}