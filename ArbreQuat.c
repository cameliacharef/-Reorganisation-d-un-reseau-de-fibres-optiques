#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    CellChaine * chaines = C->chaines;
    // On initialise coordonne max et min aux coordonne du premier point
    CellPoint * premier = chaines->points;
    *xmin = premier->x;
    *ymin = premier->y;
    *xmax = premier->x;
    *ymax = premier->y;
    while(chaines){
        CellPoint * points = chaines->points;
        while(points){
            if(points->x < *xmin){
                *xmin = points->x;
            }
            if(points->y < *ymin){
                *ymin = points->y;
            }
            if(points->x > *xmax){
                *xmax = points->x;
            }
            if(points->y > *ymax){
                *ymax = points->y;
            }
            points = points->suiv;
        }
        chaines = chaines->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat * arbreQ = (ArbreQuat *)malloc(sizeof(ArbreQuat));
    arbreQ->xc = xc;
    arbreQ->yc = yc;
    arbreQ->coteX = coteX; 
    arbreQ->coteY = coteY; 
    arbreQ->noeud = NULL;
    arbreQ->so = NULL;
    arbreQ->se = NULL;
    arbreQ->no = NULL;
    arbreQ->ne = NULL;

    return arbreQ;
}

void insererNoeudArbre(Noeud * n, ArbreQuat ** a, ArbreQuat * parent){
    //Il existe trois cas d'insertion du noeud n

    //Cas 1 : Si l'arbre est vide
    if(* a == NULL){
        //Conditions
        double xc;
        double yc;
        if(n->x < parent->xc){
            xc = parent->xc / 2;
        }else{
            xc = parent->xc / 2 + parent->xc;
        }

        if(n->y < parent->yc){
            yc = parent->yc / 2;
        }else{
            yc = parent->yc / 2 + parent->yc;
        }

        *a = creerArbreQuat(xc, yc, parent->coteX / 2, parent->coteY / 2);
        (*a)->noeud = n;
    }

    //Cas 2 : Si la racine de l'arbre a est une feuille
    else if((*a)->noeud != NULL){
        Noeud * feuilleExistante = (*a)->noeud;
        (*a)->noeud = NULL;
        //Si le noeud à insérer est au sud-ouest de la feuille existante
        if(n->x < feuilleExistante->x && n->y < feuilleExistante->y){
            insererNoeudArbre(feuilleExistante, &((*a)->ne), *a);
            insererNoeudArbre(n, &((*a)->so), *a);
        }
        //Si le noeud à insérer est au nord-ouest de la feuille existante
        if(n->x < feuilleExistante->x && n->y >= feuilleExistante->y){
            insererNoeudArbre(feuilleExistante, &(*a)->se, *a);
            insererNoeudArbre(n, &((*a)->no), *a);
        }
        //Si le noeud à insérer est au sud-est de la feuille existante
        if(n->x >= feuilleExistante->x && n->y < feuilleExistante->y){
            insererNoeudArbre(feuilleExistante, &((*a)->no), *a);
            insererNoeudArbre(n, &((*a)->se), *a);
        }
        //Si le noeud à insérer est au nord-est de la feuille existante
        else {
            insererNoeudArbre(feuilleExistante, &((*a)->so), *a);
            insererNoeudArbre(n, &((*a)->ne), *a);
        }

    }

    //Cas 3 : Si la racine de a est une cellule interne
    else{
        //Si le noeud à insérer est au sud-ouest de la feuille existante
        if(n->x < (*a)->xc && n->y < (*a)->yc){
            insererNoeudArbre(n, &((*a)->so), *a);
        }
        //Si le noeud à insérer est au nord-ouest de la feuille existante
        if(n->x < (*a)->xc && n->y >= (*a)->yc){
            insererNoeudArbre(n, &((*a)->no), *a);
        }
        //Si le noeud à insérer est au sud-est de la feuille existante
        if(n->x >= (*a)->xc && n->y < (*a)->yc){
            insererNoeudArbre(n, &((*a)->se), *a);
        }
        //Si le noeud à insérer est au nord-est de la feuille existante
        else{
            insererNoeudArbre(n, &((*a)->ne), *a);
        }
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    // cas 1 : arbre vide 
    if(*a == NULL ){
        // creation du noeud 
        Noeud* noeud_recherche = (Noeud *)malloc(sizeof(Noeud));
        noeud_recherche->x = x; 
        noeud_recherche->y = y;
        noeud_recherche->num = R->nbNoeuds + 1;
        noeud_recherche->voisins = NULL;

        // insertion dans le reseau 
        // ajout en tete la liste de noeud du reseau et update la liste 
        // Creation cellnoeud
        CellNoeud * ajout_noeud = (CellNoeud *)malloc(sizeof(CellNoeud));
        ajout_noeud->nd = noeud_recherche;
        ajout_noeud->suiv = NULL;

        // Ajout en tete
        ajout_noeud->suiv = R->noeuds;
        R->noeuds = ajout_noeud;

        R->nbNoeuds = R->nbNoeuds + 1; // ajout de 1 noeud au reseau 
        // insertion dans l'arbre 
        insererNoeudArbre(noeud_recherche, a, parent);
        return noeud_recherche;
    }

    // cas 2 : feuille 
    if ((*a)->noeud != NULL){
        // si le noeud recherche correspond a la feuille retourne le neoud
        if ((*a)->noeud->x == x && (*a)->noeud->y == y ){
            return (*a)->noeud;
        }
        // sinon on le cree , insere dans le reseau et dans l'arbre 
        else{
            // creation du noeud 
            Noeud* noeud_recherche = (Noeud *)malloc(sizeof(Noeud));
            noeud_recherche->x = x; 
            noeud_recherche->y = y;
            noeud_recherche->num = R->nbNoeuds + 1;
            noeud_recherche->voisins = NULL;

            // insertion dans le reseau 
            // ajout en tete la liste de noeud du reseau et update la liste 
            // Creation cellnoeud
            CellNoeud * ajout_noeud = (CellNoeud *)malloc(sizeof(CellNoeud));
            ajout_noeud->nd = noeud_recherche;
            ajout_noeud->suiv = NULL;

            // Ajout en tete
            ajout_noeud->suiv = R->noeuds;
            R->noeuds = ajout_noeud;

            R->nbNoeuds = R->nbNoeuds + 1; // ajout de 1 noeud au reseau 
            // insertion dans l'arbre 
            insererNoeudArbre(noeud_recherche, a, parent);
            return noeud_recherche;
        }
    }

    // cas 3 : cellule interne 
    else {
        Noeud * noeud_recherche ;
        //Si le noeud à insérer est au sud-ouest de la feuille existante
        if(x < (*a)->xc && y < (*a)->yc){
            noeud_recherche = rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
        }
        //Si le noeud à insérer est au nord-ouest de la feuille existante
        if(x < (*a)->xc && y >= (*a)->yc){
            noeud_recherche = rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
        }
        //Si le noeud à insérer est au sud-est de la feuille existante
        if(x >= (*a)->xc && y < (*a)->yc){
            noeud_recherche = rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
        }
        //Si le noeud à insérer est au nord-est de la feuille existante
        else{
            noeud_recherche = rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
        }
        return noeud_recherche;
    }
}

void libererArbreQuat(ArbreQuat * a){
    if(a){
        if(a->se){
            libererArbreQuat(a->se);
        }
        if(a->so){
            libererArbreQuat(a->so);
        }
        if(a->ne){
            libererArbreQuat(a->ne);
        }
        if(a->no){
            libererArbreQuat(a->no);
        }
    }
    free(a);
    
}

Reseau* reconstitueReseauArbre(Chaines* C){
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);

    ArbreQuat *arbreQ = creerArbreQuat(xmin + ((xmax - xmin) / 2.0), ymin + ((ymax - ymin) / 2.0) , xmax - xmin , ymax - ymin);

    //initialisation du RESEAU 
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

        CellPoint *points = chaine_courante->points;
        //Parcours des points de chaque chaîne
        while(points){
            
            // Si le noeud n'est pas dans V, on l'ajoute dans R 
            Noeud * nvNoeud = rechercheCreeNoeudArbre(R, &arbreQ, NULL, points->x, points->y);

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
    libererArbreQuat(arbreQ);
    return R;
}




