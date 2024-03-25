#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "Reseau.h"
#include "ArbreQuat.h"
#include "Chaine.h"

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
    ArbreQuat * arbreQ = (ArbreQuat *)malloc(sizeof(ArbreQuat*));
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
    if(*a == NULL){
        //Conditions
        double xc,yc;
        if(n->x < parent->xc){
            xc=parent->xc/2;
        }else{
            xc=parent->xc/2+parent->xc;
        }

        if(n->y < parent->yc){
            yc=parent->yc/2;
        }else{
            yc=parent->yc/2+parent->yc;
        }

        *a = creerArbreQuat(xc,yc,parent->coteX/2,parent->coteY/2);
        (*a)->noeud=n;
    }

    //Cas 2 : Si la racine de l'arbre a est une feuille
    else if((*a)->noeud != NULL){
        Noeud * feuilleExistante = (*a)->noeud;
        (*a)->noeud=NULL;
        //Si le noeud à insérer est au sud-ouest de la feuille existante
        if(n->x < feuilleExistante->x && n->y < feuilleExistante->y){
            insererNoeudArbre(feuilleExistante,&((*a)->ne),parent);
            insererNoeudArbre(n,&((*a)->so),parent);
        }
        //Si le noeud à insérer est au nord-ouest de la feuille existante
        else if(n->x < feuilleExistante->x && n->y >= feuilleExistante->y){
            insererNoeudArbre(feuilleExistante,&(*a)->se,parent);
            insererNoeudArbre(n,&((*a)->no),parent);
        }//Si le noeud à insérer est au sud-est de la feuille existante
        else if(n->x >= feuilleExistante->x && n->y < feuilleExistante->y){
            insererNoeudArbre(feuilleExistante,&((*a)->no),parent);
            insererNoeudArbre(n,&((*a)->se),parent);
        //Si le noeud à insérer est au nord-est de la feuille existante
        }else{
            insererNoeudArbre(feuilleExistante,&((*a)->so),parent);
            insererNoeudArbre(n,&((*a)->ne),parent);
        }

    }

    //Cas 3 : Si la racine de a est une cellule interne
    else{
        if(n->x < (*a)->xc && n->y < (*a)->yc){
            insererNoeudArbre(n,&((*a)->so),parent);
        }
        //Si le noeud à insérer est au nord-ouest de la feuille existante
        else if(n->x < (*a)->xc && n->y >= (*a)->yc){
            insererNoeudArbre(n,&((*a)->no),parent);
        }//Si le noeud à insérer est au sud-est de la feuille existante
        else if(n->x >= (*a)->xc && n->y < (*a)->yc){
            insererNoeudArbre(n,&((*a)->se),parent);
        //Si le noeud à insérer est au nord-est de la feuille existante
        }else{
            insererNoeudArbre(n,&((*a)->ne),parent);
        }
    }
}