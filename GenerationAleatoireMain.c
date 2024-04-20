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
    Chaines* chaines = generationAleatoire(500,100,500,500);//faire varier le nombre de chaînes jusqu'à 5000 par pas de 500

	//1.Listes chaînées
	tpsInit = clock();
	Reseau* reseauLC = reconstitueReseauListe(chaines);
	tpsFin = clock();

	liberer_Reseau(reseauLC);
	double tpsCalculLC = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;

	//2.Tables de hachage :
	//- de taille 10 :
	tpsInit = clock();
	Reseau* reseauH10 = reconstitueReseauHachage(chaines,10);
	tpsFin = clock();

	liberer_Reseau(reseauH10);
	double tpsCalculH10 = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;

	//- de taille 100 :
	tpsInit = clock();
	Reseau* reseauH100 = reconstitueReseauHachage(chaines,100);
	tpsFin = clock();

	liberer_Reseau(reseauH100);
	double tpsCalculH100 = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;

	//- de taille 1000 :
	tpsInit = clock();
	Reseau* reseauH1000 = reconstitueReseauHachage(chaines,1000);
	tpsFin = clock();

	liberer_Reseau(reseauH1000);
	double tpsCalculH1000 = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;

	//3.Arbres quaternaires
	tpsInit = clock();
	Reseau* reseauAQ = reconstitueReseauArbre(chaines);
	tpsFin = clock();

	liberer_Reseau(reseauAQ);
	double tpsCalculAQ = 1000*((double)(tpsFin - tpsInit)) / CLOCKS_PER_SEC;

	//Libération des chaines
	liberer_Chaines(chaines);

	return 0;
}