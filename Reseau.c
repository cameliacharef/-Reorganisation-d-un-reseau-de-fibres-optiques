#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>

#include "SVGwriter.h"
#include "Chaine.h"
#include "Reseau.h"

/*Recherche un noeud de coordonnées (x,y) dans le réseau.
S'il n'existe pas, ce noeud est créé et ajouté dans le réseau.*/
Noeud * rechercheCreeNoeudListe(Reseau * R, double x, double y){
    CellNoeud * liste_Noeuds = R->noeuds;

    while(liste_Noeuds){
        Noeud * noeud = liste_Noeuds->nd;
        if(noeud->x == x && noeud->y == y ){
            return noeud; //retourne un seul noeud existant
        }
        liste_Noeuds = liste_Noeuds->suiv;
    }

    //Si on ne le trouve pas , on le crée et l'ajoute dans le réseau
    Noeud* noeud_recherche = (Noeud *)malloc(sizeof(Noeud));
    noeud_recherche->x = x; 
    noeud_recherche->y = y;
    noeud_recherche->num = R->nbNoeuds + 1;
    noeud_recherche->voisins = NULL;

    // Ajout d'un noeud en tête de la liste des noeuds du reseau et mise à jour de la liste 
    CellNoeud * ajout_noeud = (CellNoeud *)malloc(sizeof(CellNoeud));
    ajout_noeud->nd = noeud_recherche;
    ajout_noeud->suiv = NULL;
    ajout_noeud->suiv = R->noeuds;
    R->noeuds = ajout_noeud;

    R->nbNoeuds = R->nbNoeuds + 1; // ajout de 1 noeud au réseau 

    return noeud_recherche;
}

/*Insertion du noeud dans la liste de noeuds*/
CellNoeud * insererNoeud(CellNoeud * liste_noeuds, Noeud * nd_inserer){
    CellNoeud * tmp = liste_noeuds;

    // Parcourir la liste pour vérifier si le noeud à insérer existe déjà
    while(tmp && tmp->nd != nd_inserer){
        tmp = tmp->suiv;
    }

    // Si le noeud à insérer n'existe pas dans la liste
    if(tmp == NULL){
        CellNoeud * new = (CellNoeud *)malloc(sizeof(CellNoeud));
        new->nd = nd_inserer;
        new->suiv = liste_noeuds;
        liste_noeuds = new;   
    }

    return liste_noeuds;    
}

/*Reconstitue le réseau à partir des chaînes de C*/
Reseau * reconstitueReseauListe(Chaines * C){
    //Initialisation du réseau 
    Reseau *R = (Reseau *)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->commodites = NULL;
    R->noeuds = NULL;

    CellCommodite * liste_commodite = NULL ; //liste de commodite a inserer 
    CellChaine *chaine_courante = C->chaines;

    //Parcours des chaînes
    while(chaine_courante){
        //Initialisation des noeuds extrêmes de la commodité
        Noeud *debut = NULL;
        Noeud *fin = NULL;
        Noeud *precedant = NULL; //noeud précédant pour la gestion des voisins
        CellPoint *points = chaine_courante->points;

        //Parcours des points de chaque chaîne
        while(points){     
            // Si le noeud n'est pas dans V, on l'ajoute dans R 
            Noeud * nvNoeud = rechercheCreeNoeudListe(R, points->x, points->y);

            // Début de la chaîne 
            if(debut == NULL){
                debut = nvNoeud;
            }

            fin = nvNoeud;

            /*Si precedant n'est pas NULL, on insere le nouveau noeud dans la liste des voisins du précédant 
            et on insere le précédant dans la liste des voisins du nouveau noeud.*/
            if(precedant != NULL){
                precedant->voisins = insererNoeud(precedant->voisins, nvNoeud);
                nvNoeud->voisins = insererNoeud(nvNoeud->voisins, precedant);
            }

            precedant = nvNoeud;
            points = points->suiv;
        }

        //Conservation de la commodité
        CellCommodite * commodite = (CellCommodite *)malloc(sizeof(CellCommodite));
        commodite->extrA = debut;
        commodite->extrB = fin;
        commodite->suiv = liste_commodite;
        liste_commodite = commodite;

        chaine_courante=chaine_courante->suiv;
    }

    R->commodites = liste_commodite;

    return R;
}

/*Compte le nombre de liaisons présentes dans le réseau*/
int nbLiaisons(Reseau *R){
    int total = 0;
    CellNoeud * liste_noeud = R->noeuds;

    // Parcours de la liste des noeuds du réseau 
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

    /*Le nombre total des voisins de tous les noeuds du réseau étant égal au double du nombre de liaisons,
    nous divisons le résultat.*/
    return total / 2;
}

/*Compte le nombre de commodités total du réseau*/
int nbCommodites(Reseau *R){
    int total = 0;
    CellCommodite * liste_commodites = R->commodites;

    while(liste_commodites){
        total = total + 1;
        liste_commodites = liste_commodites->suiv;
    }

    return total;
}

/*Crée un fichier f à partir du réseau R*/
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

    fprintf(f,"\n");

    // Ecriture des liaisons l
    liste_noeud = R->noeuds;
    while(liste_noeud){

        Noeud * noeud = liste_noeud->nd;
        CellNoeud * voisins = noeud->voisins;

        while (voisins){
            // il ya des doublons, on ecrit la liaison
            if(noeud->num > voisins->nd->num){
                fprintf(f, "l %d %d\n", voisins->nd->num, noeud->num);
            }
            voisins = voisins->suiv;
        }

        liste_noeud = liste_noeud->suiv;
    }

    fprintf(f,"\n");

    // Ecrire les commodités k 
    CellCommodite * liste_commodites = R->commodites;
    for(int y = 0; y < nb_commodites; y++){

        fprintf(f, "k %d %d\n", liste_commodites->extrA->num, liste_commodites->extrB->num);

        liste_commodites = liste_commodites->suiv;
    }
    
}

/*Affichage du réseau dans un fichier au format SVG*/
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

/*Libération du noeud*/
void liberer_noeud(Noeud* nd){
    liberer_CellNoeud(nd->voisins); //ajout et libération de la liste des voisins
    free(nd);
}

/*Libération d'une liste de noeuds*/
void liberer_CellNoeud(CellNoeud* liste_noeuds){
    CellNoeud* tmp;
    while(liste_noeuds){
        tmp = liste_noeuds;
        liste_noeuds = liste_noeuds->suiv;
        free(tmp);
    }
}

/*Libération d'une liste de commodités*/
void liberer_commodites(CellCommodite* commodites){
    CellCommodite* tmp;
    while(commodites){
        tmp = commodites;
        commodites = commodites->suiv;
        free(tmp);
    }
    free(commodites);
}

/*Libération du réseau*/
void liberer_Reseau(Reseau* R){
    // On crée un tableau pour tester si un noeud est déjà libéré
    int tabN[R->nbNoeuds];
    for (int i = 0; i < R->nbNoeuds; i++) {
        tabN[i] = 1; // Initialisation du tableau avec des 1 pour indiquer que tous les noeuds sont à libérer
    }

    // On libère les noeuds
    CellNoeud *liste_noeuds = R->noeuds;
    CellNoeud* tmp;
    while (liste_noeuds){
        if (tabN[(liste_noeuds->nd->num) - 1] == 1){
            tabN[(liste_noeuds->nd->num) - 1] = 0;
            liberer_noeud(liste_noeuds->nd);
        }
        tmp = liste_noeuds;
        liste_noeuds = liste_noeuds->suiv;
        free(tmp);
    }

    // Libération des commodités
    liberer_commodites(R->commodites);
    free(R);
}
