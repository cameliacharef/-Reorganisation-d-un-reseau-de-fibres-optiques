#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include <time.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(){
	//Initialisation des variables et fermeture du fichier de lecture
	clock_t tpsInit;
	clock_t tpsFin;
	int taille_H[4] = {10, 100, 1000, 10000}; //Tableau contenant les tailles de différentes tables de hachage

	FILE * fe1 = fopen("tempsCalculGenAleatLC.txt", "w");
	int nbChaines = 50;
	int nbPoints = 100;

	//Création du graphique des listes chainees
	while(nbChaines <=500){
		fprintf(fe1,"%d ",nbPoints * nbChaines);
		Chaines* chaines = generationAleatoire(nbChaines,100,500,500);

		tpsInit = clock();
		Reseau* reseauLC = reconstitueReseauListe(chaines);
		tpsFin = clock();

		liberer_Reseau(reseauLC);
		double tpsCalculLC = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC; //en millisecondes
		fprintf(fe1,"%.4f\n",tpsCalculLC);

		//Libération des chaînes et incrémentation
		liberer_Chaines(chaines);
		nbChaines = nbChaines + 50;
	}

	fclose(fe1);

	//Création du graphique des tables de hachage et des arbres quaternaires
	FILE * fe2= fopen("tempsCalculGenAleatHAQ.txt", "w");
	nbChaines = 50;
	while(nbChaines <=500){
		fprintf(fe2,"%d ",nbPoints * nbChaines);
		Chaines* chaines = generationAleatoire(nbChaines,100,500,500);

		//Tables de hachage
		for(int i = 0; i < 4; i++){
			tpsInit = clock();
			Reseau* reseauH = reconstitueReseauHachage(chaines, taille_H[i]);
			tpsFin = clock();

			liberer_Reseau(reseauH);
			double tpsCalculH = 1000 * ((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
			fprintf(fe2, "%.4f ", tpsCalculH);
		}

		//Arbres quaternaires
		tpsInit = clock();
		Reseau* reseauAQ = reconstitueReseauArbre(chaines);
		tpsFin = clock();

		liberer_Reseau(reseauAQ);
		double tpsCalculAQ = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
		fprintf(fe2,"%.4f\n",tpsCalculAQ);

		//Libération des chaînes et incrémentation
		liberer_Chaines(chaines);
		nbChaines = nbChaines + 50;
	}

	fclose(fe2);

	return 0;
}