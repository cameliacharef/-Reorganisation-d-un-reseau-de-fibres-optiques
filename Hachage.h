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

#endif