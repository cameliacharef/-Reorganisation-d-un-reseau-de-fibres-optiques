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

void libererGraphe(Graphe* G){
    for (int i = 1; i <= G->nbsom; i++){
        Sommet* s = G->T_som[i];
        Cellule_arete* voisins = s->L_voisin;
        while(voisins){
            if (voisins->a->u == s->num){
                free(voisins->a);
            }
            Cellule_arete* courant = voisins;
            voisins = voisins->suiv;
            free(courant);
        }
        free(s);
    }
    free(G->T_som);
    free(G->T_commod);
    free(G);
}


void libererListe(Cellule_file* liste) {
    Cellule_file* temp;
    while (liste != NULL) {
        temp = liste;
        liste = liste->suiv;
        free(temp);
    }
}

void ajoute_en_tete(Cellule_file* L, int val){
	Cellule_file * nouvsom = (Cellule_file *)malloc(sizeof(Cellule_file));
	nouvsom->val = val;
	nouvsom->suiv = L;
	L = nouvsom;
}


void liberer(Cellule_file *L){
	Cellule_file *cour,*prec;
	cour = L;
	while(cour != NULL){
		prec = cour;
		cour = cour->suiv;
		free(prec);
	}
	free(L);
}


/*parcours en largeur pour trouver un chemin entre u et v , stocke l'arborescence des chemins issus du sommet u*/
Cellule_file * chaine_arborescence(Graphe * G, int u , int  v){
	if (u == v){
        return NULL ; // le chemin nulle 
    }
	int distance[G->nbsom]; // stocker les distances de chaque sommet
    int visite[G->nbsom]; // sommets visités
    int sommet_precedant[G->nbsom]; //on creer un tableau pour avoir le sommet precedent de chaque sommet

    // Initialisation des tableaux de distances et de visite
    for (int i = 0; i < G->nbsom; i++) {
        distance[i] = -1; // Initialiser les distances à -1
        visite[i] = 0; // Aucun sommet n'a été visité
        sommet_precedant[i] = -1;
    }

	S_file * F  = (S_file*) malloc(sizeof(S_file));
    Init_file(F);
	visite[u - 1] = 1; // u deja visite
	distance[u - 1] = 0; // u a u = 0
    sommet_precedant[u-1] = -1; 
	enfile(F, u); // ajout de u dans file F

	while(!(estFileVide(F))){
		int sommet = defile(F); // Retirer le sommet en tête de file

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
                sommet_precedant[sommet_voisin - 1] = sommet; // Maj du predecesseur 
			}
			voisins = voisins->suiv;
		}
	}

    // Initialiser chaine d'entier 
    Cellule_file * chaine = NULL;

    // Construction de la chaîne de u à v en utilisant les prédécesseurs
    if(sommet_precedant[v - 1] == -1){
        return chaine; //non connexe , si v pas de predecesseur retourne liste vide 
    }

    int tmp_sommet = v; 

    while (tmp_sommet != u){
        ajoute_en_tete(chaine, tmp_sommet);
        tmp_sommet = sommet_precedant[tmp_sommet -1];
    }
    ajoute_en_tete(chaine, u);

    liberer_file(F);

    return chaine;
}


int reorganiseReseau(Reseau* r){
    /* on crée le graphe correspondant au réseau */
    Graphe* G = creerGraphe(r);

    /* on crée un matrice sommet-sommet pour compter le nombre de chaines passant par chaque arete */
    int mat_som_som[G->nbsom][G->nbsom];
	// Initialisation de la matrice à 0
	for (int i = 0; i < G->nbsom; i++) {
        for (int j = 0; j < G->nbsom; j++) {
            mat_som_som[i][j] = 0;
        }
	}
    /* on calcule la plus courte chaine pour chaque commodité */
    for (int i = 0; i < G->nbcommod; i++){
        Cellule_file * L = chaine_arborescence(G, G->T_commod[i].e1, G->T_commod[i].e2); // Plus courte chaine 
        int u, v;

        Cellule_file * Lcour = L;
        v = Lcour->val;
        Lcour = Lcour->suiv;
        while(Lcour){
            u = v;
            v = Lcour->val;
            mat_som_som[u][v]++; // On incrémente le nombre de chaînes passant par l'arête (u, v)
            Lcour = Lcour->suiv;
        }

        liberer(L);
    }
    
    /* on retourne le res: vrai si gamma est garantie, false sinon */
    for (int i = 0; i < G->nbcommod; i++){
        for (int j = 0; i < G->nbcommod; i++){
            if (mat_som_som[i][j] >= G->gamma){ // Si le nombre de chaines passant par l'arete depasse gamma
                libererGraphe(G);
                return 0; // retourne faux
            }
        }
    }

    libererGraphe(G); 
    return 1; // retourne vrai gamma est garanti pour toutes les arêtes
}