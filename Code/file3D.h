#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>

using namespace std;

bool readOBJ(string path,vector<unsigned int*> &faceVertexIndices,vector<float*> &temp_vertices)
{
	char char_array[path.length()+1];
	strcpy(char_array, path.c_str());
	FILE* file = fopen(char_array, "r");


	/////////////////////////////////////////
	/////////////////////////////////////////
	///////////////




	//vector<unsigned int*> faceVertexIndices;
	//vector<float*> temp_vertices;


	if(file== NULL)
	{
		printf("Impossible d'ouvrir le fichier\n");
		return 0;
	}
	while(true)
	{
		char lineHeader[30];
		int res = fscanf(file, "%s", lineHeader);

		//printf("%s\n",lineHeader);

		if(res == EOF)
			break;

		if(strcmp(lineHeader, "v")==0)
		{
			float* vertex=new float[3]();
			fscanf(file,"%f %f %f\n", &vertex[0],&vertex[1],&vertex[2]);
			temp_vertices.push_back(vertex);
		}
		else if(strcmp(lineHeader,"f")==0)
		{
			unsigned int* vertexIndex = new unsigned int[3];
			unsigned int fraude;
			int matches= fscanf(file, "%u %u %u\n",&vertexIndex[0],&vertexIndex[1],&vertexIndex[2]);
			//printf("%u %u %u\n",vertexIndex[0],vertexIndex[1],vertexIndex[2]);
			faceVertexIndices.push_back(vertexIndex);
		}
		else if(strcmp(lineHeader,"l")==0)
		{
			unsigned int* vertexIndex = new unsigned int[3];
			int matches= fscanf(file, "%d %d\n",&vertexIndex[0],&vertexIndex[1]);
			vertexIndex[2]=-1;

			//printf("%u %u\n",vertexIndex[0],vertexIndex[1]);
			faceVertexIndices.push_back(vertexIndex);
		}
	}
	/*for(int i=0;i<temp_vertices.size();i++)
	{
		printf("Vertex %d : %f %f %f\n",i,temp_vertices.at(i)[0],temp_vertices.at(i)[1],temp_vertices.at(i)[2]);
	}*/
	printf("Number of vertex : %zu\n",temp_vertices.size());
	printf("Number of face : %zu\n",faceVertexIndices.size());
	
	return true;
}

bool writeOBJ(string path,vector<unsigned int*> &faceVertexIndices,vector<float*> &temp_vertices,bool writeFace)
{
	char char_array[path.length()+1];
	strcpy(char_array, path.c_str());
	/*FILE* file = fopen(char_array, "w");*/
	ofstream myfile;
	myfile.open(char_array);


	/////////////////////////////////////////
	/////////////////////////////////////////
	///////////////




	//vector<unsigned int*> faceVertexIndices;
	//vector<float*> temp_vertices;

	printf("Nombre de vertex dans le fichier de base : %zu\n",temp_vertices.size());
	for(int i=0;i<temp_vertices.size();i++)
	{

		myfile<<"v "<<temp_vertices.at(i)[0]<<" "<<temp_vertices.at(i)[1]<<" "<<temp_vertices.at(i)[2]<<endl;
		/*float* vertex=new float[3]();
		fscanf(file,"%f %f %f\n", &vertex[0],&vertex[1],&vertex[2]);
		temp_vertices.push_back(vertex);*/
	}

	if(writeFace)
	{
		int indiceMax=0;
		for(int i=0;i<faceVertexIndices.size();i++)
		{
			
			if(faceVertexIndices.at(i)[2]!=-1)
				myfile<<"f "<<faceVertexIndices.at(i)[0]+1<<" "<<faceVertexIndices.at(i)[1]+1<<" "<<faceVertexIndices.at(i)[2]+1;
			else
				myfile<<"l"<<faceVertexIndices.at(i)[0]+1<<" "<<faceVertexIndices.at(i)[1]+1;
			myfile<<endl;



			/*float* vertex=new float[3]();
			fscanf(file,"%f %f %f\n", &vertex[0],&vertex[1],&vertex[2]);
			temp_vertices.push_back(vertex);*/
		}
	}
	myfile.close();
	return true;
}