#include "Chaine.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "SVGwriter.h"

CellPoint * creer_cellPoint(double x, double y){
    CellPoint * point = (CellPoint *)malloc(sizeof(CellPoint));
    point->x = x;
    point->y = y; 
    point->suiv = NULL;

    return point;
}

void afficher_cellPoint(CellPoint* point){
    if(point != NULL){
        printf("%.2f %.2f ", point->x, point->y);
    }
}

CellChaine * creer_cellChaine(int numero, CellPoint * points){
    CellChaine * chaine_points = (CellChaine *)malloc(sizeof(CellChaine));
    chaine_points->numero = numero;
    chaine_points->points = points; 
    chaine_points->suiv = NULL;

    return chaine_points;
}

int nombre_points(CellPoint * points){
    int nb = 0;
    while(points){
        nb = nb + 1;
    }
    return nb;
}

void afficher_cellChaine(CellChaine * chaine){
    if(chaine != NULL){
        printf("%d %d ", chaine->numero, nombre_points(chaine->points));
        afficher_cellPoint(chaine->points);
    }
}


Chaines * creer_Chaines(int gamma, int nbChaines, CellChaine * cellchaines){
    Chaines * chainesf = (Chaines *)malloc(sizeof(Chaines));
    chainesf->gamma = gamma;
    chainesf->nbChaines = nbChaines; 
    chainesf->chaines = cellchaines;

    return chainesf;
}

void afficher_Chaines(Chaines * chaines){
    if(chaines != NULL){
        printf("NbChain: %d\nGamma: %d\n", chaines->nbChaines, chaines->gamma);
        afficher_cellChaine(chaines->chaines);
    }
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

/*void inserer_point(CellPoint * point, double x, double y){
    CellPoint * new_point = creer_cellPoint(x, y);
    new_point->suiv = point;
    point = new_point;
}*/
void inserer_point(CellPoint * point, double x, double y) {
    if (point == NULL) {
        printf("Erreur : la liste de points est vide.\n");
        return;
    }
    while (point->suiv != NULL) {
        point = point->suiv;
    }
    CellPoint * nouveau_point = creer_cellPoint(x, y);
    if (nouveau_point == NULL) {
        printf("Erreur lors de la création du nouveau point.\n");
        return;
    }
    point->suiv = nouveau_point;
}
void inserer_cellChaine(CellChaine *chaine, int numero, CellPoint *point) {
    if (chaine == NULL) {
        printf("Erreur : la liste de chaînes est vide.\n");
        return;
    }
    while (chaine->suiv != NULL) {
        chaine = chaine->suiv;
    }
    CellChaine *nouvelle_chaine = creer_cellChaine(numero, point);
    if (nouvelle_chaine == NULL) {
        printf("Erreur lors de la création de la nouvelle chaîne.\n");
        return;
    }
    chaine->suiv = nouvelle_chaine;
}


/*void inserer_cellChaine(CellChaine * chaine, int numero, CellPoint * point){
    CellChaine * new_chaine = creer_cellChaine(numero, point);
    new_chaine->suiv = chaine;
    chaine = new_chaine;
}*/

Chaines * lectureChaines(FILE *f){
    assert(f != NULL);
    char buffer[256];
    int cpt = 0;

    int NbChain ;
    int Gamma;

    fgets(buffer, 256, f);
    sscanf(buffer, "NbChain: %d", &NbChain);

    fgets(buffer, 256, f);
    sscanf(buffer, "Gamma: %d", &Gamma);

    CellChaine * nouvel_chaine = NULL;
    while (fgets(buffer, 256, f)){
        if (cpt > NbChain) // Arrêt de la lecture lorsque n lignes ont été lues
            break;
         
        int numero_chaine;
        int nb_points;

        sscanf(buffer, "%d %d ", &numero_chaine, &nb_points);// lecture numero et nb points

        
        //creation de la liste de points 
        CellPoint * nouveau_point = NULL;
        for(int i = 0; i<nb_points; i++){
            double x,y;
            if (i == 0) { // premier point
                sscanf(buffer, "%lf %lf ", &x, &y);
                nouveau_point = creer_cellPoint(x, y);
            } else {
                sscanf(buffer, "%lf %lf ", &x, &y);
                inserer_point(nouveau_point, x, y);
            }
        }
        
        //creation de la chaine avec numero 
        if (cpt == 0) { // premiere chaine 
            nouvel_chaine = creer_cellChaine(numero_chaine, nouveau_point);
        } else {
            inserer_cellChaine(nouvel_chaine, numero_chaine, nouveau_point);
        }

        
        cpt++;
    }

    //creation de la structure Chaines

    Chaines * chaines = creer_Chaines(Gamma, NbChain, nouvel_chaine);
    return chaines;
}

void liberer_cellPoint(CellPoint *point) {
    while (point != NULL) {
        CellPoint * temp = point; 
        point = point->suiv; 
        free(temp); 
    }
}

void liberer_cellChaine(CellChaine *chaine) {
    while (chaine != NULL) {
        liberer_cellPoint(chaine->points);
        CellChaine * temp = chaine; 
        chaine = chaine->suiv; 
        free(temp); 
    }
}

void liberer_Chaines(Chaines *chaines) {
    if (chaines != NULL) {
        liberer_cellChaine(chaines->chaines); 
        free(chaines); 
    }
}


double longueurPoint(CellPoint* pt){
    if(nombre_points(c->points)<2){
        return 0.0;
    }
    double s=0.0;
    CellPoint *cour=pt;
    while(cour->suiv){
        s+=sqrt(pow(cour->suiv->x-cour->x,2)+pow(cour->suiv->y-cour->y,2));
        cour=cour->suiv;
    }
    return s;
}

double longueurChaine(CellChaine *c){
    if(!c){
        return 0.0;
    }
    double s=0.0;
    CellChaine *cour=c;
    while(cour){
        s+=longueurPoint(c->points);
        cour=cour->suiv;
    }
    return s;
}

double longueurTotale(Chaines *C){
    if(C->nbChaines==0){
        return 0.0;
    }
    double s=0.0;
    for(int i=0;i<C->nbChaines;i++){
        s+=longueurChaine(C->chaines[i]);
    }
    return s;
}

int comptePointsTotal(Chaines *C){
    int s=0;
    for(int i =0; i<C->nbChaines;i++){
        CellChaine *tmp=C->chaines[i];
        while(tmp){
            s+=nombre_points(tmp->points);
            tmp=tmp->suiv;
        }
    }
    return s;
}
