#include "Chaine.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

CellPoint * creer_cellPoint(double x, double y){
    CellPoint * point = (CellPoint *)malloc(sizeof(CellPoint));
    point->x = x;
    point->y = y; 
    point->suiv = NULL;

    return point;
}

CellChaine * creer_cellChaine(int numero, CellPoint * points){
    CellChaine * chaine_points = (CellChaine *)malloc(sizeof(CellChaine));
    chaine_points->numero = numero;
    chaine_points->points = points; 
    chaine_points->suiv = NULL;

    return chaine_points;
}

Chaines * creer_Chaines(int gamma, int nbChaines, CellChaine * chaines){
    Chaines * chaines = (Chaines *)malloc(sizeof(Chaines));
    chaines->gamma = gamma;
    chaines->nbChaines = nbChaines; 
    chaines->chaines = chaines;

    return chaines;
}

void inserer_point(CellPoint * point, double x, double y){
    CellPoint * new_point = creer_cellPoint(x, y);
    new_point->suiv = point;
    point = new_point;
}

void inserer_cellChaine(CellChaine * chaine, int numero, CellPoint * point){
    CellChaine * new_chaine = creer_cellChaine(numero, point);
    new_chaine->suiv = chaine;
    chaine = new_chaine;
}

Chaines * lectureChaines(FILE *f){
    assert(f != NULL);
    char buffer[256];
    int NbChain;
    int Gamma;
    int nbPoints;
    double x;
    double y;


    int cpt = 0;
    fgets(buffer, 256, f);
    sscanf(buffer, "NbChain: %d\nGamma: %d", &NbChain, &Gamma);
    
    CellChaine* chaine=malloc(sizeof(CellChaine));
    chaine=NULL;
    while(fgets(buffer, 256, f)){
        if (cpt >= NbChain) 
            break;
        sscanf(buffer, "%d %d", &cpt, &nbPoints);

        CellPoint* p=malloc(sizeof(CellPoint));
        p=NULL;

        for(int i = 0; i <= nbPoints; i++){
            sscanf(buffer, "%.2f %.2f", &x, &y);
            inserer_point(p, x, y);          
        }

        CellChaine* chaine=malloc(sizeof(CellChaine));
        chaine=NULL;

        inserer_cellChaine(chaine,cpt,p);
        cpt++;
    }


    Chaines * chaines = creer_Chaines(Gamma, NbChain, chaine);

    return chaines;     

}

