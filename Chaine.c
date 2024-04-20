#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

#include "SVGwriter.h"
#include "Chaine.h"


CellPoint * creer_cellPoint(double x, double y){
    CellPoint * point = (CellPoint *)malloc(sizeof(CellPoint));
    point->x = x;
    point->y = y; 
    point->suiv = NULL;

    return point;
}

CellChaine * creer_cellChaine(int numero){
    CellChaine * chaine_points = (CellChaine *)malloc(sizeof(CellChaine));
    chaine_points->numero = numero;
    chaine_points->points = NULL; //Liste des points initialise a NULL  
    chaine_points->suiv = NULL;

    return chaine_points;
}

int nombre_points(CellPoint * points){
    int nb = 0;
    while(points){
        nb = nb + 1;
        points = points->suiv;
    }
    return nb;
}

Chaines * creer_Chaines(){
    Chaines * chainesf = (Chaines *)malloc(sizeof(Chaines));
    chainesf->gamma = 0; // nbgamma initialise a 0
    chainesf->nbChaines = 0; //nbchaine initialise a 0
    chainesf->chaines = NULL;

    return chainesf;
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

// inserer en tete de la liste de points de chaine 
void inserer_point(CellChaine * chaine, double x, double y){
    CellPoint * new_point = creer_cellPoint(x, y);
    new_point->suiv = chaine->points;
    chaine->points = new_point;
}

// inserer en tete de la liste de chaines de CHAINE 
void inserer_cellChaine(Chaines * liste_chaines, CellChaine * chaine){
    chaine->suiv = liste_chaines->chaines;
    liste_chaines->chaines = chaine;
}

Chaines *lectureChaines(FILE *f) {

    assert(f != NULL);
    char buffer[256];
    
    // Lecture des deux premières lignes
    int NbChain;
    int Gamma;
    
    fgets(buffer, 256, f);
    sscanf(buffer, "NbChain: %d", &NbChain);

    fgets(buffer, 256, f);
    sscanf(buffer, "Gamma: %d", &Gamma);

    // Creation de la structure de l'ensemble des chaines a retourner 
    Chaines* chaines = creer_Chaines();
    chaines->nbChaines = NbChain;
    chaines->gamma = Gamma;

    // Lecture des chaines

    for (int j =  0 ;  j < NbChain ; j++) {

        int numero_chaine, nb_points;

        fscanf(f, "%d %d ", &numero_chaine, &nb_points);

        //Creation de la chaine
        CellChaine *nouvel_chaine = creer_cellChaine(numero_chaine);

        // Création de la liste de points

        for (int i = 0; i < nb_points; i++) {
            double x, y;
            fscanf(f, "%lf %lf ", &x, &y);
        
            inserer_point(nouvel_chaine, x, y);
        }
        //inserer chaque chaine dans notre ensemble de chaine 
        inserer_cellChaine(chaines, nouvel_chaine);

    }

    return chaines;
}

void ecrireChaines(Chaines *C, FILE *f){
    assert(f != NULL);

    int gamma = C->gamma;
    int nbChaines = C->nbChaines;
    CellChaine * liste_chaine = C->chaines;

    fprintf(f, "NbChain: %d\n", nbChaines);
    fprintf(f, "Gamma: %d\n", gamma);

    for(int i = 0; i < nbChaines; i++){
        CellPoint * liste_points = liste_chaine->points;

        fprintf(f, "%d %d ", liste_chaine->numero, nombre_points(liste_points));
        

        while(liste_points){
            fprintf(f, "%.2f %.2f ", liste_points->x, liste_points->y);
            liste_points = liste_points->suiv;
        }
        fprintf(f, "\n");
        liste_chaine = liste_chaine->suiv;
    }
} 

void afficheChainesSVG(Chaines *C, char* nomInstance){
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

double longueurChaine(CellChaine *c){
    if(nombre_points(c->points) < 2){
        return 0.0;
    }
    double distance = 0.0;
    CellPoint *courant = c->points;
    while(courant->suiv){
        distance += sqrt(pow(courant->suiv->x - courant->x, 2) + pow(courant->suiv->y - courant->y, 2));
        courant = courant->suiv;
    }
    return distance;
}

double longueurTotale(Chaines *C){
    if(!C){
        return 0.0;
    }
    if(C->nbChaines == 0){
        return 0.0;
    }

    double longueur = 0.0;
    CellChaine *courant = C->chaines;

    while(courant){
        longueur += longueurChaine(C->chaines);
        courant = courant->suiv;
    }
    return longueur;
}

int comptePointsTotal(Chaines *C){
    int nb_points = 0;
    CellChaine * courant = C->chaines;
    while(courant){
        nb_points += nombre_points(courant->points);
        courant = courant->suiv;
    }
    return nb_points;
}

Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax,int ymax){
    Chaines* liste_chaines = creer_Chaines();
    liste_chaines->gamma = 3;

    //Création d'un ensemble de chaînes
    for(int i=0; i<nbChaines;i++){
        //Création d'une nouvelle chaîne
        CellChaine* chaine = creer_cellChaine(i+1);

        //Génération aléatoire de points dans la chaîne créée
        for(int j=0;j<nbPointsChaine;j++){
            inserer_point(chaine,(double)(rand() % xmax),(double)(rand() % ymax));
        }

        //Insertion d'une chaîne dans la liste de chaînes
        inserer_cellChaine(liste_chaines,chaine);
        liste_chaines->nbChaines = liste_chaines->nbChaines +1 ;
    }
    return liste_chaines;
}