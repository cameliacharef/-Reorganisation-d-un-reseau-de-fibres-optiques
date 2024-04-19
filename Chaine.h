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

// Les fonctions rajoutées
CellPoint * creer_cellPoint(double x, double y);
CellChaine * creer_cellChaine(int numero);
int nombre_points(CellPoint * points);
Chaines * creer_Chaines();
void inserer_point(CellChaine * chaine, double x, double y);
void inserer_cellChaine(Chaines * liste_chaines, CellChaine * chaine);
void liberer_cellPoint(CellPoint *point);
void liberer_cellChaine(CellChaine *chaine);
void liberer_Chaines(Chaines *chaines);
Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax,int ymax);

#endif	
