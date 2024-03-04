#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

#include "SVGwriter.h"
#include "Chaine.h"
#include "Reseau.h"

Noeud * rechercheCreeNoeudListe(Reseau * R, double x, double y){
    CellNoeud * liste_Noeuds = R->noeuds;

    while(liste_Noeuds){
        Noeud * noeud = liste_Noeuds->nd;
        if(noeud->x == x && noeud->y == y ){
            return noeud; //retourne un seul noeud existant
        }
        liste_Noeuds = liste_Noeuds->suiv;
    }

    //Si on le trouve pas , on le crée et ajoute au reseau
    // creation du noeud 
    Noeud* noeud_recherche = (Noeud *)malloc(sizeof(Noeud));
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

    return noeud_recherche;
}




int recherche_noeud(CellNoeud * V, double x, double y){
    if (V == NULL){
        return 0;
    }
    else{
        while(V){
            if(V->nd->x == x && V->nd->y == y){
                return 1; // si trouve
            }
            V = V->suiv;
        }
    }
    return 0; // pas trouve
}

void inserer_noeud(CellNoeud * liste_noeuds, Noeud * nd_inserer){
    CellNoeud* new=(CellNoeud*)malloc(sizeof(CellNoeud));
    new->nd=nd_inserer;
    new->suiv=liste_noeuds;
    liste_noeuds=new;    
}

Reseau * reconstitueReseauListe(Chaines * C){
    //initialiser reseau 
    Reseau *R=(Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds=0;
    R->gamma=0;
    R->commodites=NULL;

    CellNoeud * V = NULL;
    R->noeuds=V;
    CellChaine * liste_chaine = C->chaines;
    int num=0;
    while(liste_chaine){

        CellPoint * liste_points = liste_chaine->points;
        
        while(liste_points) {
            
            int trouve = recherche_noeud(V, liste_points->x, liste_points->y);
            // si pas trouve
            if(!trouve){
                Noeud* nd =rechercheCreeNoeudListe(R,liste_points->x,liste_points->y);
                inserer_noeud(V,nd);
            }
            

            liste_points = liste_points->suiv;
        }
        V=R->noeuds;

        liste_chaine = liste_chaine->suiv;
    }
    return R;
}



int nbLiaisons(Reseau *R){
    int total = 0;
    CellNoeud * liste_noeud = R->noeuds;

    // parcours liste noeud reseau 
    while (liste_noeud){
        Noeud * noeud = liste_noeud->nd;
        CellNoeud * liste_voisins = noeud->voisins;

        // parcours des voisins de chaque noeud 
        while (liste_voisins){
            total = total + 1 ; 
            liste_voisins = liste_voisins->suiv;
        }

        liste_noeud = liste_noeud->suiv;
    }

    return total / 2; // 2 liaisons pour 2 meme noeud A-B et B-A
}


int nbCommodites(Reseau *R){
    int total = 0;
    CellCommodite * liste_commodites = R->commodites;

    while(liste_commodites){
        total = total + 1;
        liste_commodites = liste_commodites->suiv;
    }

    return total;
}


// REVOIR POUR ELIMINER LES DOUBLONS 
void ecrireReseau(Reseau *R, FILE *f){
    assert(f != NULL);

    int nbNoeuds = R->nbNoeuds;
    int gamma = R->gamma;
    int nb_liaisons = nbLiaisons(R);
    int nb_commodites = nbCommodites(R);

    // Ecriture 4 premieres lignes 

    fprintf(f, "NbNoeuds: %d\n", nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nb_liaisons);
    fprintf(f, "NbCommodites: %d\n", nb_commodites);
    fprintf(f, "Gamma: %d\n\n", gamma);

    // Ecriture des noeuds v
    CellNoeud * liste_noeud = R->noeuds;
    for(int i = 0; i < nbNoeuds; i++){

        Noeud * noeud = liste_noeud->nd;
        fprintf(f, "v %d %.6f %.6f\n", noeud->num, noeud->x, noeud->y);

        liste_noeud = liste_noeud->suiv;
    }

    // Ecriture des liaisons l
    liste_noeud = R->noeuds;
    for(int j = 0; j < nb_liaisons; j++){

        Noeud * noeud = liste_noeud->nd;
        CellNoeud * voisins = noeud->voisins;
        while (voisins){
            fprintf(f, "l %.2f %d\n", voisins->nd->x, noeud->num); // COMMENT ELIMINER LES DOUBLONS 
            voisins = voisins->suiv;
        }

        liste_noeud = liste_noeud->suiv;
    }

    // Ecrire les commodités k 
    CellCommodite * liste_commodites = R->commodites;
    for(int y = 0; y < nb_commodites; y++){

        fprintf(f, "k %d %d\n", liste_commodites->extrA->num, liste_commodites->extrB->num);

        liste_commodites = liste_commodites->suiv;
    }
    
}




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

void liberer_CellNoeud(CellNoeud* liste_noeuds){
    while(liste_noeuds){
        CellNoeud* tmp=liste_noeuds;
        liste_noeuds=liste_noeuds->suiv;
        free(tmp);
    }
    free(liste_noeuds);
}

void liberer_commodites(CellCommodite* commodites){
    while(commodites){
        CellCommodite* tmp=commodites;
        commodites=commodites->suiv;
        free(tmp);
    }
    free(commodites);
}

void liberer_Reseau(Reseau* R){
    liberer_CellNoeud(R->noeuds);
    liberer_CellCommodites(R->commodites);
    free(R);
}