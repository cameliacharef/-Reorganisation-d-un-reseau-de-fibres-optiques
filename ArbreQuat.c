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
    if(*a == NULL){
        ArbreQuat * nouvel_arbreQ = creerArbreQuat;
    }
}