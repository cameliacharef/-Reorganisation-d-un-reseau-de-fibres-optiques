#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "ArbreQuat.h"

/*Détermine les coordonnées minimales et maximales des points qui constituent les chaînes du réseau.*/
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if (C == NULL) {
        return;
    } 
    if (C->chaines == NULL) {
        return;
    }  
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

/*Crée une cellule d'un arbre quaternaire, de centre (xc,yc), de longeur coteX et de hauteur coteY*/
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

/*Insère un noeud dans un arbre quaternaire*/
void insererNoeudArbre(Noeud * n, ArbreQuat ** a, ArbreQuat * parent){
    //Il existe trois cas d'insertion du noeud n
    //Cas 1 : Si l'arbre est vide
    if(* a == NULL){
        //Conditions
        double nv_xc;
        double nv_yc;
        double nv_coteX = parent->coteX / 2;
        double nv_coteY = parent->coteY / 2;
        if(n->x < parent->xc){
            nv_xc = parent->xc / 2;
        }else{
            nv_xc = parent->xc / 2 + parent->xc;
        }

        if(n->y < parent->yc){
            nv_yc = parent->yc / 2;
        }else{
            nv_yc = parent->yc / 2 + parent->yc;
        }

        *a = creerArbreQuat(nv_xc, nv_yc, nv_coteX, nv_coteY);
        (*a)->noeud = n;
    }

    //Cas 2 : Si la racine de l'arbre a est une feuille
    else {

        if((*a)->noeud != NULL){
            Noeud * feuilleExistante = (*a)->noeud;
            (*a)->noeud = NULL; //ce n'est plus une feuille apres l'insertion 
            
            // A l'Ouest
            if(feuilleExistante->x < (*a)->xc){
                if(feuilleExistante->y < (*a)->yc){ //Si le noeud à insérer est au sud-ouest de la feuille existante
                    insererNoeudArbre(feuilleExistante, &((*a)->so), *a);
                    //insererNoeudArbre(n, &((*a)->so), *a);
                }
                else{ //Si le noeud à insérer est au nord-ouest de la feuille existante
                    insererNoeudArbre(feuilleExistante, &(*a)->no, *a);
                    //insererNoeudArbre(n, &((*a)->no), *a);
                }
            }
            // A l'Est
            else{
                //Si le noeud à insérer est au sud-est de la feuille existante
                if(feuilleExistante->y < (*a)->yc){
                    insererNoeudArbre(feuilleExistante, &((*a)->se), *a);
                    //insererNoeudArbre(n, &((*a)->se), *a);
                }
                //Si le noeud à insérer est au nord-est de la feuille existante
                else {
                    insererNoeudArbre(feuilleExistante, &((*a)->ne), *a);
                    //insererNoeudArbre(n, &((*a)->ne), *a);
                }
            }

        }

        //Cas 3 : Si la racine de a est une cellule interne
        // Dans tous les cas on insere n 

        
        if(n->x < (*a)->xc){
            if(n->y < (*a)->yc){//Si le noeud à insérer est au sud-ouest de la feuille existante{
                insererNoeudArbre(n, &((*a)->so), *a);
            }
            else{//Si le noeud à insérer est au nord-ouest de la feuille existante
                insererNoeudArbre(n, &((*a)->no), *a);
            }
        }
        else{
            if(n->y < (*a)->yc) {//Si le noeud à insérer est au sud-est de la feuille existante
                insererNoeudArbre(n, &((*a)->se), *a);
            }
            //Si le noeud à insérer est au nord-est de la feuille existante
            else{
                insererNoeudArbre(n, &((*a)->ne), *a);
            }
        }
    }
}

/*Recherche un noeud de coordonnées (x,y) dans l'arbre quaternaire.
S'il n'est pas trouvé, le noeud est créé et il est inséré dans l'arbre et dans le réseau.*/
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

        // Ajout en tete
        ajout_noeud->suiv = R->noeuds;
        R->noeuds = ajout_noeud;

        R->nbNoeuds = R->nbNoeuds + 1; // ajout de 1 noeud au reseau 
        // insertion dans l'arbre 
        insererNoeudArbre(noeud_recherche, a, parent);
        return noeud_recherche;
    }

    // cas 2 : feuille 
    else {
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
            Noeud * noeud_recherche = NULL ;
            
            if(x < (*a)->xc ){
                if(y < (*a)->yc){ // sud-ouest 
                    noeud_recherche = rechercheCreeNoeudArbre(R, &(*a)->so, *a, x, y);
                }
                else{ // nord-ouest 
                    noeud_recherche = rechercheCreeNoeudArbre(R, &(*a)->no, *a, x, y);
                }
            }
            
            else{
                if(y < (*a)->yc){ // sud-est 
                    noeud_recherche = rechercheCreeNoeudArbre(R, &(*a)->se, *a, x, y);
                }
                else{ // nord-est 
                    noeud_recherche = rechercheCreeNoeudArbre(R, &(*a)->ne, *a, x, y);
                }
                
            }
            return noeud_recherche;
        }
    }
}

/*Libère l'arbre quaternaire.*/
void libererArbreQuat(ArbreQuat * a){
    if(!a){
        return;
    }
    if(a->noeud != NULL){
        free(a);
        return;
    }
    if(a){
        libererArbreQuat(a->se);
        libererArbreQuat(a->so);
        libererArbreQuat(a->ne);
        libererArbreQuat(a->no);
        free(a);
    }
}

/*Reconstitue le réseau à partir des chaînes et en utilisant l'arbre quaternaire*/
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
