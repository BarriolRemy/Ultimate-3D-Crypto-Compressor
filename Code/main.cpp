#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "file3D.h"
#include "Compress.h"

using namespace std;


/*

Lien du code dans Replit :
https://replit.com/@theghoul/CryptoCompressor#main.cpp


Pour voir le modèle resultat, utilisé Blender et importer l'obj


Ligne 54 :
		LowBunny.obj : Pas beaucoup de vertex
			Utilisé la précision "0.05" pour avoir un beau modèle 
		GreatBunny.obj : Beaucoup de vertex
			Utilisé la précision "5" pour avoir un beau modèle 
		BigBunny.obj : Enormément de vertex
			Utilisé la précision "5" pour avoir un beau modèle 

Precision à la ligne 39




*/


int main()
{
	vector<unsigned int*> faceVertexIndices;
	vector<float*> vertices;
	float distance=0.05;				//Modifier la précision ici
	
	printf("Lecture \n");

	/*
		LowBunny.obj : Pas beaucoup de vertex
			Utilisé la précision "0.05" pour avoir un beau modèle 
		GreatBunny.obj : Beaucoup de vertex
			Utilisé la précision "5" pour avoir un beau modèle 
		BigBunny.obj : Enormément de vertex
			Utilisé la précision "5" pour avoir un beau modèle 
	*/


	readOBJ("LowBunny.obj",faceVertexIndices,vertices);



	/*
		Enlève les points qui ne font pas partie du maillage (utilisé pour traiter les modèles de Stanford)
	*/
	printf("Supression point solitaire \n");
	supprAlonePoint(&vertices,faceVertexIndices);


	printf("Facteur \n");

	/*
		Pour pouvoir modifier des modèles trés petits
	*/
	applyFactor(&vertices, 100);

	printf("Compression \n");
	
	fichierCompresse fC=compress(&faceVertexIndices,&vertices,distance,1);
	
	writeOBJ("HappyBunny.obj",faceVertexIndices,vertices,true);

	
	//decompress(fC,&faceVertexIndices,&vertices);


	
	printf("Anti Facteur \n");


	antiApplyFactor(&vertices, 100);
	
	printf("Ecriture \n");
	//writeOBJ("HappyBunny2.obj",faceVertexIndices,vertices,true);

	/*
		Donne la taille du fichier si nous aurions eu le temps d'écrire un vrai flux binaire dans un fichier
	*/
	printf("Taille fichier compressé : %d Ko \n",getSizeOfCompressed(fC));
	printf("Fin \n");
	return 0;
} 