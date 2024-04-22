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

	FILE * fe = fopen("tempsCalculGenAleatLC.txt", "w");
	int nbChaines = 500;

	//Creation du graphique des listes chainees
	while(nbChaines <=1500){
		fprintf(fe,"%d ",nbChaines);
		Chaines* chaines = generationAleatoire(nbChaines,100,500,500);
		tpsInit = clock();
		Reseau* reseauLC = reconstitueReseauListe(chaines);
		tpsFin = clock();
		liberer_Reseau(reseauLC);
		double tpsCalculLC = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
		fprintf(fe,"%.4f\n",tpsCalculLC);
		liberer_Chaines(chaines);
		nbChaines = nbChaines + 500;
	}

	fclose(fe);

	/*FILE * fe2= fopen("tempsCalculGenAleatHAQ.txt", "w");
	nbChaines = 500;
	while(nbChaines <=5000){
		fprintf(fe2,"%d ",nbChaines);
		Chaines* chaines = generationAleatoire(nbChaines,100,500,500);
		//Tables de hachage taille 10
		tpsInit = clock();
		Reseau* reseauH10 = reconstitueReseauHachage(chaines,10);
		tpsFin = clock();

		tpsInit = clock();
		Reseau* reseauH100 = reconstitueReseauHachage(chaines,100);
		tpsFin = clock();

		tpsInit = clock();
		Reseau* reseauH1000 = reconstitueReseauHachage(chaines,1000);
		tpsFin = clock();

		liberer_Reseau(reseauH10);
		double tpsCalculH10 = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
		fprintf(fe2,"%.4f ",tpsCalculH10);

		liberer_Reseau(reseauH100);
		double tpsCalculH100 = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
		fprintf(fe2,"%.4f ",tpsCalculH100);

		liberer_Reseau(reseauH1000);
		double tpsCalculH1000 = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
		fprintf(fe2,"%.4f ",tpsCalculH1000);


		//Arbres quaternaires
		tpsInit = clock();
		Reseau* reseauAQ = reconstitueReseauArbre(chaines);
		tpsFin = clock();

		liberer_Reseau(reseauAQ);
		double tpsCalculAQ = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;
		fprintf(fe2,"%.4f\n",tpsCalculLC);
		liberer_Chaines(chaines);
		nbChaines = nbChaines + 500;
	}

	fclose(fe2);*/

	

	return 0;
}