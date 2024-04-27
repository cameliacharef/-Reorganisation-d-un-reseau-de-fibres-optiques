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

void liberer_file(S_file* F){
	while(!estFileVide(F)){
		defile(F);
	}
	free(F);
}

// Parcours en largeur 
int plus_petit_nbChaine(Graphe* G, int u , int v){
	if (u == v){
        return 0 ; // le chemin nulle 
    }
	int distance[G->nbsom]; // stocker les distances de chaque sommet
    int visite[G->nbsom]; // sommets visités

    // Initialisation des tableaux de distances et de visite
    for (int i = 0; i < G->nbsom; i++) {
        distance[i] = -1; // Initialiser les distances à -1
        visite[i] = 0; // Aucun sommet n'a été visité
    }

	S_file * F  = (S_file*) malloc(sizeof(S_file));
    Init_file(F);
	visite[u - 1] = 1; // u deja visite
	distance[u - 1] = 0; // u a u = 0
	enfile(F, u); // ajout de u dans file F

	while(!(estFileVide(F))){
		int sommet = defile(F); // Retirer le sommet en tête de file

		if (sommet == v){
			liberer_file(F);
            return distance[v-1];
		}

		Cellule_arete * voisins = G->T_som[sommet - 1]->L_voisin;
        while (voisins != NULL){
			// le sommet voisin 
			int sommet_voisin;
			if(sommet == voisins->a->u){
				sommet_voisin = voisins->a->v;
			}
			else {
				sommet_voisin = voisins->a->u;
			}
			// si pas deja visite
			if(visite[sommet_voisin - 1] == 0){
				visite[sommet_voisin - 1] = 1; // On le visite;
				enfile(F, sommet_voisin);
				distance[sommet_voisin - 1] = distance[sommet - 1] + 1; // A chaque fois on rajoute 1 a distance 
			}
			voisins = voisins->suiv;
		}
	}

	liberer_file(F);

	return -1; // si on trouves pas de chaine de u a v 
}


int reorganiseReseau(Reseau *r){
	return 0;
}
int main(){
	FILE *f = fopen("00014_burma.cha","r");

    Chaines *c = lectureChaines(f); 
    Reseau *res = reconstitueReseauArbre(c); //ici nous utilisons reconstitueReseauArbre car nous avons vue avec l'exo6 que c'est la maniere de reconstituer le reseau la plus rapide
	Graphe * g = creerGraphe(res);
	int t = plus_petit_nbChaine(g, 3, 6);
	printf("%d\n", t);
    liberer_Chaines(c);
	liberer_Reseau(res);
	return 0;
}