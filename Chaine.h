#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include <stdio.h>

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

Chaines* lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurChaine(CellChaine *c);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);

//rajoutée
CellPoint * creer_cellPoint(double x, double y);
void afficher_cellPoint(CellPoint* point);
CellChaine * creer_cellChaine(int numero, CellPoint * points);
int nombre_points(CellPoint * points);
void afficher_cellChaine(CellChaine * chaine);
Chaines * creer_Chaines(int gamma, int nbChaines, CellChaine * chaines);
void afficher_Chaines(Chaines * chaines);
void inserer_point(CellPoint * point, double x, double y);
void inserer_cellChaine(CellChaine * chaine, int numero, CellPoint * point);

#endif	
