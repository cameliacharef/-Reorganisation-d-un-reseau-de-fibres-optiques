#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include "Reseau.h"


typedef struct{
  int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud ** T;
} TableHachage ;


Noeud * rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);

int fonction_clef(int x, int y);
int fonction_hachage(int k, int taille);
void liberer_Hachage(TableHachage * H);
TableHachage * initialiser_tableH(int tailleMax);

#endif