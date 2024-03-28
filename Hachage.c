#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "Reseau.h"
#include "Hachage.h"


int fonction_clef(int x, int y){
    return  (int) ( y + (x + y)*(x + y + 1)/2 );
}

int fonction_hachage(int k, int taille){
    double A = (sqrt(5) - 1) / 2;
    return (int) taille * (k * A - (int) k * A) ; // arrondi au plus bas : (int)
}

void liberer_Hachage(TableHachage * H){
    int m = H->tailleMax;
    CellNoeud ** tab_listeH = H->T;
    for(int i = 0; i < m; i++){
        if(tab_listeH[i]){
            CellNoeud* ccour = tab_listeH[i];
            CellNoeud* csuiv = NULL;
            while(ccour){
                csuiv = ccour;
                ccour = ccour->suiv;
                free(csuiv);
            }
        }
    }
    free(tab_listeH);
    free(H);
}

Noeud * rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    // Recherche dans la table de hachage 
    CellNoeud ** tab_listeH = H->T;
    int indice_insertion = fonction_hachage(fonction_clef(x, y), H->tailleMax);

    CellNoeud * courant = tab_listeH[indice_insertion];
    while(courant){
        Noeud * noeud = courant->nd;
        if(noeud->x == x && noeud->y == y ){
            return noeud; //retourne un seul noeud existant
        }
        courant = courant->suiv;
    }


    // Si on le trouve pas , on le crée et ajoute au reseau et a la table de hachage 
    // creation du noeud 
    Noeud* noeud_recherche = (Noeud *)malloc(sizeof(Noeud));
    noeud_recherche->x = x; 
    noeud_recherche->y = y;
    noeud_recherche->num = R->nbNoeuds + 1;
    noeud_recherche->voisins = NULL;

    // ajout en tete la liste de noeud du reseau et update la liste 
    // Creation cellnoeud
    CellNoeud * ajout_noeud = (CellNoeud *)malloc(sizeof(CellNoeud));
    ajout_noeud->nd = noeud_recherche;
    ajout_noeud->suiv = NULL;

    // Ajout en tete
    ajout_noeud->suiv = R->noeuds;
    R->noeuds = ajout_noeud;

    R->nbNoeuds = R->nbNoeuds + 1; // ajout de 1 noeud au reseau 

    // Ajout dans la table de hachage 
    CellNoeud * ajout_noeudH = (CellNoeud *)malloc(sizeof(CellNoeud)); // creation du cellNoeud
    ajout_noeudH->nd = noeud_recherche;
    ajout_noeudH->suiv = NULL;

    // Ajout en tete de la liste chaine d'indice_insertion 
    ajout_noeudH->suiv = tab_listeH[indice_insertion]; // insertion en tete de la liste chainee a lindice indique par fct hachage
    tab_listeH[indice_insertion] = ajout_noeudH;
    H->nbElement = H->nbElement + 1; // ajout de l'element

    return noeud_recherche;

}


TableHachage * initialiser_tableH(int tailleMax){
    TableHachage *H = (TableHachage *)(malloc(sizeof(TableHachage)));
    CellNoeud ** T = (CellNoeud **)(malloc(sizeof(CellNoeud *) * tailleMax));
    for(int i = 0; i < tailleMax; i++){
        T[i] = NULL;
    }
    H->T = T;
    H->nbElement = 0;
    H->tailleMax = tailleMax;
    return H;
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
    // Creation de la table de hachage de taille M 
    TableHachage * t_hachage = initialiser_tableH(M);

    // initialisation du RESEAU 
    Reseau *R = (Reseau *)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->commodites = NULL;
    R->noeuds = NULL;

    CellCommodite * liste_commodite = NULL ; //liste de commodite a inserer 

    CellChaine *chaine_courante = C->chaines;

    //Parcours des chaînes
    while(chaine_courante){
        //Initialisation des noeuds extrêmes de la commodité
        Noeud *debut = NULL;
        Noeud *fin = NULL;

        //noeud precedant pour gestion des voisins
        Noeud *precedant = NULL;

        //Nombre de noeuds de la chaîne
        //int nbNoeuds = 0;

        CellPoint *points = chaine_courante->points;
        //Parcours des points de chaque chaîne
        while(points){
            
            // Si le noeud n'est pas dans V, on l'ajoute dans R 
            Noeud * nvNoeud = rechercheCreeNoeudHachage(R, t_hachage, points->x, points->y);

            // debut de la chaine 
            if(debut == NULL){
                debut = nvNoeud;
            }

            fin = nvNoeud;

            //Si precedant on insere le nouveau noeud dans la liste des voisins du precedant 
            //Et on insere le precedant dans la liste des voisins du nouveau
            if(precedant != NULL){
                precedant->voisins = insererNoeud(precedant->voisins, nvNoeud);
                nvNoeud->voisins = insererNoeud(nvNoeud->voisins, precedant);
            }
            
            //stocker le precedant noeud
            precedant = nvNoeud;

            points = points->suiv;
        }


        //Conservation de la commodité
        CellCommodite * commodite = (CellCommodite *)malloc(sizeof(CellCommodite));
        commodite->extrA = debut;
        commodite->extrB = fin;
        commodite->suiv = liste_commodite;
        liste_commodite = commodite;


        chaine_courante=chaine_courante->suiv;
    }

    R->commodites = liste_commodite; // liste commodite avec toutes commodites
    liberer_Hachage(t_hachage);
    return R;
}

