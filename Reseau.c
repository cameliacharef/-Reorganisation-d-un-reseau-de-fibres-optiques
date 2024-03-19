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
    while(V){
        if(V->nd->x == x && V->nd->y == y){
            return 1; // si trouve
        }
        V = V->suiv;
    }
    return 0; // pas trouve
}

void inserer_noeud(CellNoeud * liste_noeuds, Noeud * nd_inserer){
    CellNoeud * tmp = liste_noeuds;
    while(tmp && tmp->nd != nd_inserer){
        tmp = tmp->suiv;
    }
    if(tmp == NULL){
        CellNoeud* new=(CellNoeud*)malloc(sizeof(CellNoeud));
        new->nd=nd_inserer;
        new->suiv=liste_noeuds;
        liste_noeuds=new;   
    }
     
}

/*Reseau * reconstitueReseauListe(Chaines * C){
    //initialiser reseau 
    Reseau *R = (Reseau *)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = 0;
    R->commodites = NULL;

    CellNoeud * V = NULL;
    CellChaine * liste_chaine = C->chaines;

    while(liste_chaine){

        CellPoint * liste_points = liste_chaine->points;
        
        while(liste_points) {
            
            int trouve = recherche_noeud(V, liste_points->x, liste_points->y);
            // si pas trouve
            if(!trouve){
                Noeud* nd = rechercheCreeNoeudListe(R,liste_points->x, liste_points->y);
                Noeud* tmp = V->nd;
                inserer_noeud(tmp->voisins, nd); //ajout d'un nouveau voisin au noeud tmp
                inserer_noeud(V, nd);
                inserer_noeud(V->nd->voisins, tmp); //ajout d'un nouveau voisin au noeud créé
            }
            //mettre à jour les voisins de p et de ceux de ses voisins

            //conserver la commodité de la chaîne
            liste_points = liste_points->suiv;
        }

        liste_chaine = liste_chaine->suiv;
    }
    return R;
}*/

Reseau * reconstitueReseauListe(Chaines * C){
    //initialisation du RESEAU 
    Reseau *R = (Reseau *)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->commodites = NULL;
    R->noeuds = NULL;

    CellCommodite * liste_commodite = NULL ; //liste de commodite a inserer 

    CellChaine *chaine_courante= C->chaines;

    

    //Parcours des chaînes
    while(chaine_courante){
        //Initialisation des noeuds extrêmes de la commodité
        Noeud *debut = NULL;
        Noeud *fin = NULL;

        //noeud precedant pour gestion des voisins
        Noeud *precedant = NULL;

        //Nombre de noeuds de la chaîne
        //int nbNoeuds = 0;

        CellPoint *points = chaine_courante->points;
        //Parcours des points de chaque chaîne
        while(points){
            
            // Si le noeud n'est pas dans V, on l'ajoute dans R 
            Noeud * nvNoeud = rechercheCreeNoeudListe(R, points->x, points->y);
            /*if(nbNoeuds == 0){
                debut=nvNoeud;
            }*/

            // debut de la chaine 
            if(debut == NULL){
                debut = nvNoeud;
            }

            fin = nvNoeud;

            //Si precedant on insere le nouveau noeud dans la liste des voisins du precedant 
            //Et on insere le precedant dans la liste des voisins du nouveau
            //Sans meler noeud suivant comme on avait fait 
            if(precedant != NULL){
                inserer_noeud(precedant->voisins, nvNoeud);
                inserer_noeud(nvNoeud->voisins, precedant);
            }
            
            //Mise à jour des voisins de p et de ceux de CellChaineses voisins : à revoir
            //Noeud *suivant = rechercheCreeNoeudListe(R,points->suiv->x,points->suiv->y);
 
            /*if(suivant){
                inserer_noeud(nvNoeud->voisins,suivant);
            }*/

            //stocker le precedant noeud
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

    R->commodites = liste_commodite; // liste commodite avec toutes commodites

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
    fprintf(f,"\n");
    // Ecriture des liaisons l
    liste_noeud = R->noeuds;
    while(liste_noeud){

        Noeud * noeud = liste_noeud->nd;
        CellNoeud * voisins = noeud->voisins;

        while (voisins){
            // il ya des doublons , on ecrit la liaison que celui 
            if(noeud->num > voisins->nd->num){
                fprintf(f, "l %d %d\n", voisins->nd->num, noeud->num);
            }
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

void liberer_noeud(Noeud* nd){
    liberer_CellNoeud(nd->voisins);//ajout
}

void liberer_CellNoeud(CellNoeud* liste_noeuds){
    while(liste_noeuds){
        CellNoeud* tmp=liste_noeuds;
        liberer_noeud(tmp->nd);//ajout
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
    liberer_commodites(R->commodites);
    free(R);
}


