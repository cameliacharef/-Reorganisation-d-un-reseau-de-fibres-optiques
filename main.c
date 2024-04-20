#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include <time.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(int argc , char *argv[]){
	//Ouverture du fichier de lecture
	if(argc != 2){
        printf("Il faut 1 argument : le nom du fichier .cha.\n"); // "00014_burma.cha"
        return 0;
    }
    char *nomfic = argv[1];

    FILE * fl = fopen(nomfic, "r");
    if (fl == NULL) {
        printf("Error opening file\n");
        return 1;
    }

	//Ouverture du fichier d'écriture
	FILE * fe = fopen("comparaisonTpsCalcul", "w");

	//Initialisation des variables et fermeture du fichier de lecture
	clock_t tpsInit;
	clock_t tpsFin;
	double tpsCalcul;
	Chaines *chaines = lectureChaines(fl);
	fclose(fl);

	//Lancement des trois programmes et calcul des temps de calcul
	fprintf(fe,"Comparaisons de calcul :\n");

	//1.Listes chaînées
	tpsInit = clock();
	Reseau* reseauLC = reconstitueReseauListe(chaines);
	tpsFin = clock();

	liberer_Reseau(reseauLC);

	tpsCalcul = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
	fprintf(fe,"Temps de calcul pour les listes chaînées : %.4f millisecondes\n",tpsCalcul);

	//2.Tables de hachage :
	//- de taille 10 :
	tpsInit = clock();
	Reseau* reseauH10 = reconstitueReseauHachage(chaines,10);
	tpsFin = clock();

	liberer_Reseau(reseauH10);

	tpsCalcul = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
	fprintf(fe,"Temps de calcul pour les tables de hachage de taille 10 : %.4f millisecondes\n",tpsCalcul);

	//- de taille 100 :
	tpsInit = clock();
	Reseau* reseauH100 = reconstitueReseauHachage(chaines,100);
	tpsFin = clock();

	liberer_Reseau(reseauH100);

	tpsCalcul = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
	fprintf(fe,"Temps de calcul pour les tables de hachage de taille 100 : %.4f millisecondes\n",tpsCalcul);

	//- de taille 1000 :
	tpsInit = clock();
	Reseau* reseauH1000 = reconstitueReseauHachage(chaines,1000);
	tpsFin = clock();

	liberer_Reseau(reseauH1000);

	tpsCalcul = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
	fprintf(fe,"Temps de calcul pour les tables de hachage de taille 1000 : %.4f millisecondes\n",tpsCalcul);

	//3.Arbres quaternaires
	tpsInit = clock();
	Reseau* reseauAQ = reconstitueReseauArbre(chaines);
	tpsFin = clock();
	
	liberer_Reseau(reseauAQ);

	tpsCalcul = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
	fprintf(fe,"Temps de calcul pour les arbres quaternaires : %.4f millisecondes\n",tpsCalcul);

	//Fermeture du fichier d'écriture et libération des chaines
	fclose(fe);
	liberer_Chaines(chaines);

	return 0;
}