#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "SVGwriter.h"
#include "Chaine.h"
#include "Reseau.h"

Noeud * rechercheCreeNoeudListe(Reseau * R, double x, double y){
    CellNoeud * liste_Noeuds = R->noeuds;

    Noeud * noeud_recherche = NULL;
    while(liste_Noeuds){
        Noeud * noeud = liste_Noeuds->nd;
        if(noeud->x == x && noeud->y == y ){
            noeud_recherche = noeud;
        }
        liste_Noeuds = liste_Noeuds->suiv;
    }

    //Si on le trouve pas , on le crée et ajoute au reseau
    if(noeud_recherche == NULL){
        // creation du noeud 
        noeud_recherche = (Noeud *)malloc(sizeof(Noeud));
        noeud_recherche->x = x; 
        noeud_recherche->y = y;
        noeud_recherche->num = R->nbNoeuds + 1;
        noeud_recherche->voisins = NULL;

        // ajout en tete la liste de noeud du reseau et update la liste 
        // Creation cellnoeud
        CellNoeud * ajout_noeud = (CellNoeud *)malloc(sizeof(CellNoeud));
        ajout_noeud->nd = noeud_recherche;
        ajout_noeud->suiv = NULL;

        // Ajout en tete
        ajout_noeud->suiv = R->noeuds;
        R->noeuds = ajout_noeud;

        R->nbNoeuds = R->nbNoeuds + 1; // ajout de 1 noeud au reseau 

    }

    return noeud_recherche;
}







/*
Reseau* reconstitueReseauListe(Chaines *C){}
void ecrireReseau(Reseau *R, FILE *f){}
int nbLiaisons(Reseau *R){}
int nbCommodites(Reseau *R){}
*/

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}
