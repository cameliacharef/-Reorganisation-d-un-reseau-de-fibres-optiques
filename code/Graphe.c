#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <limits.h>
#include "Graphe.h"

/*Alloue et crée une arête*/
Arete* creerArete(int u,int v){
	Arete* a= (Arete*)malloc(sizeof(Arete));
	a->u = u;
	a->v = v;
	return a;
}

void insereArete(Arete* a,Cellule_arete** c){
	if(a){
        Cellule_arete* new = (Cellule_arete*)malloc(sizeof(Cellule_arete));
        new->a = a;
        new->suiv = *c;
        *c = new;
    }
}

Sommet * creerSommet(int num, double x, double y){
	Sommet* s =  (Sommet *)malloc(sizeof(Sommet));
    s->num = num;
    s->x = x;
    s->y = y;
    s->L_voisin = NULL;
    return s;
}

Graphe* creerGraphe(Reseau* r){
	//Allocation et initialisation d'un graphe
	Graphe* G = (Graphe *)malloc(sizeof(Graphe));
	G->nbsom = r->nbNoeuds;
	G->gamma = r->gamma;
	G->nbcommod = nbCommodites(r);
	G->T_som = (Sommet**)malloc(G->nbsom * sizeof(Sommet*));
	G->T_commod = (Commod*)malloc(G->nbcommod * sizeof(Commod));

	//Initialiser sommets avec coordonnes
	CellNoeud* courant = r->noeuds;
	while(courant){
		//initialisation de tous les sommets avec leur coordonnes avec pour chaque sommet leur liste de voisin 
		(G->T_som)[courant->nd->num - 1] = creerSommet(courant->nd->num, courant->nd->x, courant->nd->y);
		courant = courant->suiv;
	}
	//Remplissage du tableau des sommets
	// Refaire le parcours pour inserer les voisins  
	courant = r->noeuds;
	while(courant){
		//Création de la liste des voisins
		CellNoeud *voisins = courant->nd->voisins;
        while(voisins){
			/*Chaque arete {u,v} du graphe est alloue une seule fois, mais apparait dans la liste des voisins
			du sommet u et du sommet v dans le graphe.*/
			if(courant->nd->num < voisins->nd->num){
                Arete* a = creerArete(courant->nd->num, voisins->nd->num);
                insereArete(a,&(G->T_som[a->u - 1]->L_voisin)); // Ajout dans u
                insereArete(a,&(G->T_som[a->v - 1]->L_voisin)); // Ajout dans v
				 //Les arêtes sont incidentes au sommet i
				
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

/* Inserer en tete de cellule_file */
void insererEnTeteFile(Cellule_file **f, int val){
  Cellule_file *nouv=(Cellule_file *) malloc(sizeof(Cellule_file));
  nouv->val=val;

  nouv->suiv=*f;
  *f=nouv;
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


/*ALGO DE DJIKSTRA*/
/*parcours en largeur pour trouver un chemin entre u et v , stocke l'arborescence des chemins issus du sommet u*/
Cellule_file * chaine_arborescence(Graphe * g, int u , int  v){
// verifier si u et v sont identique
    if (u == v)
        return NULL;

    int tab_dist[g->nbsom];
    int tab_visite[g->nbsom];
    int tab_precedent[g->nbsom];

    for (int i = 0; i < g->nbsom; i++) {
        tab_dist[i] = INT_MAX;// Initialisation à l'infinit
        tab_visite[i] = 0;// Aucun sommet n'a été visité
        tab_precedent[i] = -1; //Aucun sommet n'a de prédécesseur
    }

    tab_dist[u-1] = 0; // Distance du nœud de départ est de 0
    tab_precedent[u-1] = u;
    while (1) {
        int min_dist = INT_MAX;
        int min_index = -1;

        // Recherche du nœud non visité avec la plus petite distance
        for (int i = 0; i < g->nbsom; i++) {
            if (!tab_visite[i] && tab_dist[i] < min_dist) {
                min_dist = tab_dist[i];
                min_index = i;
            }
        }

        if (min_index == -1 || min_index == v-1) {
            break; // Si tous les nœuds ont été visités ou si on a atteint le nœud d'arrivée, sortir de la boucle
        }

        tab_visite[min_index] = 1; // Marquer le nœud comme visité

        // Mettre à jour les distances des nœuds voisins non visités
        Cellule_arete* voisins = g->T_som[min_index]->L_voisin;
        while (voisins != NULL) {
            if (!tab_visite[voisins->a->v-1]) {
                int temp_dist = tab_dist[min_index] + 1; 
                if (temp_dist < tab_dist[voisins->a->v - 1]) {
                    tab_dist[voisins->a->v - 1] = temp_dist;
                    tab_precedent[voisins->a->v - 1] = min_index+1;
                }
            }
            voisins = voisins->suiv;
        }
    }
    // Chemin non trouve
    if (tab_precedent[v - 1] == -1) {
        return NULL;
    }

    // Reconstruction du chemin
    Cellule_file* chemin = NULL;
    //insererEnTeteFile(&chemin, v);
    int sommet = v;
    while (sommet != u) {
        insererEnTeteFile(&chemin, sommet);
        sommet = tab_precedent[sommet - 1];
    }
    insererEnTeteFile(&chemin, u);
    return chemin;
}




/* Reorganiser le reseau à l'aide d'un graphe */
int reorganiseReseau(Reseau* r){
    /* on crée le graphe correspondant au réseau */
    Graphe* G = creerGraphe(r);

	if (G == NULL) {
        printf("Erreur : Graphe non créé.\n");
        return 0; // retourne faux
    }

    /* on crée une matrice pour compter le nombre de chaines passant par chaque arete */
    int** mat_compte_chaines = (int**)malloc(sizeof(int*) * G->nbsom);
	// Initialisation de la matrice à 0
	for (int i = 0; i < G->nbsom; i++) { //alloue et initialise a 0
        mat_compte_chaines[i] = (int*)malloc(G->nbsom * sizeof(int)); 
        for(int j = 0; j < G->nbsom; j++){
            mat_compte_chaines[i][j] = 0;
        }
    }


    /* on calcule la plus courte chaine pour chaque commodité , remplir matrice*/
    Cellule_file * L = NULL;
    for (int i = 0; i < G->nbcommod; i++){
        L = chaine_arborescence(G, G->T_commod[i].e1, G->T_commod[i].e2); // Plus courte chaine 
        int u, v;

        Cellule_file * tmp = L;
        // Incrémenter pour chaque arête dans le chemin
        while(tmp && tmp->suiv){
            u = tmp->val;
            v = tmp->suiv->val;
            mat_compte_chaines[u-1][v-1]++; // On incrémente le nombre de chaînes passant par l'arête (u, v)
            mat_compte_chaines[v-1][u-1]++; // Les arêtes sont non orientées
            tmp = tmp->suiv;
        }
        
        supprimerChemin(L);
    }

    /* on retourne le res: vrai si gamma est garantie, false sinon */
    for (int i = 0; i < G->nbcommod; i++){
        for (int j = 0; j < G->nbcommod; j++){
            if (mat_compte_chaines[i][j] >= G->gamma){ // Si le nombre de chaines passant par l'arete depasse gamma
                // Libérer la mémoire allouée à la matrice 
                for (int k = 0; k < G->nbsom; k++) { 
                    free(mat_compte_chaines[k]);
                }
                free(mat_compte_chaines);
                libererGraphe(G);
                return 0; // retourne faux
            }
        }
    }

    // Libérer la mémoire allouée à la matrice compteur_chaines
    for (int i = 0; i < G->nbsom; i++) {
        free(mat_compte_chaines[i]);
    }
    free(mat_compte_chaines);

    // Libérer la mémoire allouée au graphe
    libererGraphe(G); 
    return 1; // retourne vrai gamma est garanti pour toutes les arêtes
}



void afficher_graph(Graphe* g){ //affichage du graph pour dubuggage 
    int i ;
    for(i =  0  ; i < g->nbsom ; i++){
        Sommet * som_present = g->T_som[i]; 
        printf("Le sommet : %d  de liste voisin  : [" , som_present->num);
        Cellule_arete * liste_voisin =  som_present->L_voisin;
        while(liste_voisin){
            printf("(%d , %d) " , liste_voisin->a->u , liste_voisin->a->v);
            liste_voisin = liste_voisin->suiv;
        }
        printf("] \n");
    }

    Commod* tab_commod  =  g->T_commod; 
    for(int j = 0 ; j < g->nbcommod ; j++){
        printf("Commodite %d du graph :(%d, %d) \n", j+1, tab_commod[j].e1, tab_commod[j].e2);
    }
}

/* Afficher le chemin le plus court de u à v*/
void afficherChemin(Cellule_file* L, int u, int v){
    Cellule_file* tmp = L;
    printf("Chemin de %d à %d:\n", u, v);
    if (tmp == NULL){
        printf("Pas de chemin trouvé\n");
        return;
    }
    while (tmp){
        printf("Sommet: %d \n", tmp->val);
        tmp = tmp->suiv;
    }
    printf("\n\n");
}

/*Liberation Sommet*/
void libererSommet(Sommet *s){
    if(s==NULL){ // deja liberé
        return;
    }

    s->L_voisin = NULL;
    free(s);
}

/*Liberation Cellule_arete*/
void libererCellule_Arete(Cellule_arete *c){
    if(c==NULL){ // deja liberé
        return;
    }
    c->a = NULL;
    c->suiv = NULL;

    free(c);
}

/*liberation du graphe*/
void libererGraphe(Graphe* G){
    if(G){
        Sommet* s;
        for (int i = 0; i < G->nbsom; i++){
            s = G->T_som[i];
            Cellule_arete* voisins = s->L_voisin;
            Cellule_arete* tmp;
            while(voisins){
                /*Pour ne pas liberer 2 fois la meme arete, on verifie si nous allons la rencontrer plus tard ou pas*/
                if ((voisins->a->u <= i && voisins->a->v <= s->num)){
                    free(voisins->a);
                }
                tmp = voisins;
                voisins = voisins->suiv;
                libererCellule_Arete(tmp);
            }
            libererSommet(s);
        }
        free(G->T_som);
        free(G->T_commod);
        free(G); 
    }
    
}

/* Suppression du chemin */
void supprimerChemin(Cellule_file* chemin){
    Cellule_file* tmp;
    while (chemin != NULL) {
        tmp = chemin;
        chemin = chemin->suiv;
        free(tmp);
    }
    free(chemin);
}

/*Libere une file */
void liberer_file(S_file* F){
	while(!estFileVide(F)){
		defile(F);
	}
	free(F);
}