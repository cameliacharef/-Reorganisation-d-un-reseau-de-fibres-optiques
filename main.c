#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(){
	//Initialisation des variables de temps de calcul
	clock_t tpsInitLC;
	clock_t tpsFinLC;
	clock_t tpsInitH;
	clock_t tpsFinH;
	clock_t tpsInitAQ;
	clock_t tpsFinAQ;

	//Lancement des trois programmes et calcul des temps de calcul

	//1.Listes chaînées
	tpsInitLC = clock();
	/*...*/
	tpsFinLC = clock();

	//2.Tables de hachage
	tpsInitH = clock();
	/*...*/
	tpsFinH = clock();

	//3.Arbres quaternaires
	tpsInitAQ = clock();
	/*...*/
	tpsFinAQ = clock();

	//Comparaisons des temps de calcul pour les trois programmes
	double tpsCalculLC = ((double)(tpsFinLC - tpsInitLC)) / CLOCKS_PER_SEC;
	double tpsCalculH = ((double)(tpsFinH - tpsInitH)) / CLOCKS_PER_SEC;
	double tpsCalcuAQ = ((double)(tpsFinAQ - tpsInitAQ)) / CLOCKS_PER_SEC;

	/*...*/

	//Libération des structures de données
	
	/*...*/

	return 0;
}