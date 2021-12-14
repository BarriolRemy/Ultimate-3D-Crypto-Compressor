#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <math.h>

using namespace std;


struct fichierCompresse
{
	float limitMinX,limitMinY,limitMinZ,limitMaxX,limitMaxY,limitMaxZ;
	int NbProfondeur, NbFaces;
	vector<vector<int>> arbreBinaire;
	vector<unsigned int*> faceVertexIndices;
};


struct Groupe
{
	float limitMinX,limitMinY,limitMinZ,limitMaxX,limitMaxY,limitMaxZ;
	int nextTurn=0;
	vector<int> indexAdj;
	int numberVertex=0;
	int indicePoint;


	int profPère=-1;   //?
	int indexPère=-1;
	vector<int> Fils;
	bool Vertex=false;

	Groupe* Frère;
	int profondeur;
	bool Droite=false;

	int indexAffich=0;
};

int decToBinary(int n) {
    int binaryNum[32]; 
  
    int i = 0; 
    while (n > 0) {
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    }

    return i;
}




int getSizeOfCompressed(fichierCompresse sC)
{
	int size=0;
	size+=sizeof(float)*6;
	size+=sizeof(int)*2;


	for(int x=0;x<sC.arbreBinaire.size();x++)
	{
		int maxmax=0;
		for(int y=0;y<sC.arbreBinaire.at(x).size();y++)
		{
			if(maxmax<sC.arbreBinaire.at(x).at(y))
				maxmax=sC.arbreBinaire.at(x).at(y);
		}
		size+=sizeof(int)*2;
		for(int y=0;y<sC.arbreBinaire.at(x).size();y++)
		{
			size+=decToBinary(maxmax);
		}
	}
	int maxmax1=0;
	for(int y=0;y<sC.faceVertexIndices.size();y++)
		{
			for(int lll=0;lll<3;lll++)
				{
				if(maxmax1<sC.faceVertexIndices.at(y)[lll])
					maxmax1=sC.faceVertexIndices.at(y)[lll];
			}
		}
	size+=sC.faceVertexIndices.size()*decToBinary(maxmax1);
	size/=8;
	size/=1000;
	return size;
}




void applyFactor(vector<float*>* vertices, float factor)
{
	for(int i=0;i<vertices->size();i++)
	{
		vertices->at(i)[0]*=factor;
		vertices->at(i)[1]*=factor;
		vertices->at(i)[2]*=factor;
	}
}

void antiApplyFactor(vector<float*>* vertices, float factor)
{
	for(int i=0;i<vertices->size();i++)
	{
		vertices->at(i)[0]/=factor;
		vertices->at(i)[1]/=factor;
		vertices->at(i)[2]/=factor;
	}
}

void replaceFace(vector<unsigned int*>* faceVertexIndices,vector<Groupe> ensembleGroupe)
{
	int affichValue=0;
	for(int i=0;i<faceVertexIndices->size();i++)
	{
		for(int indVert=0;indVert<3;indVert++)
		{
			if(faceVertexIndices->at(i)[indVert]!=-1)
			{
				for(int indGroupe=0;indGroupe<ensembleGroupe.size();indGroupe++)
				{
					if(faceVertexIndices->at(i)[indVert]==(ensembleGroupe.at(indGroupe).indicePoint)+1)
					{
						faceVertexIndices->at(i)[indVert]=indGroupe;
						indGroupe=ensembleGroupe.size();
					}
				}
			}
		}

		int percent=(int)round(float(i)/faceVertexIndices->size()*100);
		if(( percent%10==0)&&(affichValue!= percent))
		{
			printf("--%d%% \n", percent);
			affichValue=percent;
		}
	}
}




void recLimite2(int Zindex, int prof, int index, vector<vector<Groupe>>* ultimateResult)
{
	
    float interLimit=0;
	int realZindex=Zindex;
	if(Zindex==3)
		realZindex=0;

	if((!ultimateResult->at(prof).at(index).Vertex)&&(ultimateResult->at(prof).at(index).Fils.size()==2))
	{

		int fils0=ultimateResult->at(prof).at(index).Fils.at(0);
		int fils1=ultimateResult->at(prof).at(index).Fils.at(1);
		
		ultimateResult->at(prof+1).at(fils0).limitMinX=0;
		ultimateResult->at(prof+1).at(fils0).limitMinY=0;
		ultimateResult->at(prof+1).at(fils0).limitMinZ=0;
		ultimateResult->at(prof+1).at(fils0).limitMaxX=0;
		ultimateResult->at(prof+1).at(fils0).limitMaxY=0;
		ultimateResult->at(prof+1).at(fils0).limitMaxZ=0;
		
		ultimateResult->at(prof+1).at(fils1).limitMinX=0;
		ultimateResult->at(prof+1).at(fils1).limitMinY=0;
		ultimateResult->at(prof+1).at(fils1).limitMinZ=0;
		ultimateResult->at(prof+1).at(fils1).limitMaxX=0;
		ultimateResult->at(prof+1).at(fils1).limitMaxY=0;
		ultimateResult->at(prof+1).at(fils1).limitMaxZ=0;

		recLimite2(realZindex+1, prof+1, fils0, ultimateResult);
		recLimite2(realZindex+1, prof+1, fils1, ultimateResult);
	}
	else if(ultimateResult->at(prof).at(index).Fils.size()==1)
	{
		int fils0=ultimateResult->at(prof).at(index).Fils.at(0);
			if(!ultimateResult->at(prof+1).at(fils0).Droite)
			{
				ultimateResult->at(prof+1).at(fils0).limitMinX=0;
				ultimateResult->at(prof+1).at(fils0).limitMinY=0;
				ultimateResult->at(prof+1).at(fils0).limitMinZ=0;
				ultimateResult->at(prof+1).at(fils0).limitMaxX=0;
				ultimateResult->at(prof+1).at(fils0).limitMaxY=0;
				ultimateResult->at(prof+1).at(fils0).limitMaxZ=0;
				recLimite2(realZindex+1, prof+1, fils0, ultimateResult);
			}
			else
			{
				ultimateResult->at(prof+1).at(fils0).limitMinX=0;
				ultimateResult->at(prof+1).at(fils0).limitMinY=0;
				ultimateResult->at(prof+1).at(fils0).limitMinZ=0;
				ultimateResult->at(prof+1).at(fils0).limitMaxX=0;
				ultimateResult->at(prof+1).at(fils0).limitMaxY=0;
				ultimateResult->at(prof+1).at(fils0).limitMaxZ=0;
				recLimite2(realZindex+1, prof+1, fils0, ultimateResult);
			}
	}
}

void recLimite(int Zindex, int prof, int index, vector<vector<Groupe>>* ultimateResult)
{
	
    float interLimit=0;
	int realZindex=Zindex;
	if(Zindex==3)
		realZindex=0;


	if((!ultimateResult->at(prof).at(index).Vertex)&&(ultimateResult->at(prof).at(index).Fils.size()==2))
	{

		int fils0=ultimateResult->at(prof).at(index).Fils.at(0);
		int fils1=ultimateResult->at(prof).at(index).Fils.at(1);
		

		ultimateResult->at(prof+1).at(fils0).limitMinX=ultimateResult->at(prof).at(index).limitMinX;
		ultimateResult->at(prof+1).at(fils0).limitMinY=ultimateResult->at(prof).at(index).limitMinY;
		ultimateResult->at(prof+1).at(fils0).limitMinZ=ultimateResult->at(prof).at(index).limitMinZ;
		ultimateResult->at(prof+1).at(fils0).limitMaxX=ultimateResult->at(prof).at(index).limitMaxX;
		ultimateResult->at(prof+1).at(fils0).limitMaxY=ultimateResult->at(prof).at(index).limitMaxY;
		ultimateResult->at(prof+1).at(fils0).limitMaxZ=ultimateResult->at(prof).at(index).limitMaxZ;
		
		ultimateResult->at(prof+1).at(fils1).limitMinX=ultimateResult->at(prof).at(index).limitMinX;
		ultimateResult->at(prof+1).at(fils1).limitMinY=ultimateResult->at(prof).at(index).limitMinY;
		ultimateResult->at(prof+1).at(fils1).limitMinZ=ultimateResult->at(prof).at(index).limitMinZ;
		ultimateResult->at(prof+1).at(fils1).limitMaxX=ultimateResult->at(prof).at(index).limitMaxX;
		ultimateResult->at(prof+1).at(fils1).limitMaxY=ultimateResult->at(prof).at(index).limitMaxY;
		ultimateResult->at(prof+1).at(fils1).limitMaxZ=ultimateResult->at(prof).at(index).limitMaxZ;

		if(realZindex==0) 		//X
		{
			interLimit=(ultimateResult->at(prof).at(index).limitMaxX-ultimateResult->at(prof).at(index).limitMinX)/2.0;
			ultimateResult->at(prof+1).at(fils1).limitMaxX=ultimateResult->at(prof).at(index).limitMaxX-interLimit;
			ultimateResult->at(prof+1).at(fils0).limitMinX=ultimateResult->at(prof).at(index).limitMinX+interLimit;
		}
		else if(realZindex==1)	//Y
		{
			interLimit=(ultimateResult->at(prof).at(index).limitMaxY-ultimateResult->at(prof).at(index).limitMinY)/2.0;
			ultimateResult->at(prof+1).at(fils1).limitMaxY=ultimateResult->at(prof).at(index).limitMaxY-interLimit;
			ultimateResult->at(prof+1).at(fils0).limitMinY=ultimateResult->at(prof).at(index).limitMinY+interLimit;
		}
		else if(realZindex==2)	//Z
		{
			interLimit=(ultimateResult->at(prof).at(index).limitMaxZ-ultimateResult->at(prof).at(index).limitMinZ)/2.0;
			ultimateResult->at(prof+1).at(fils1).limitMaxZ=ultimateResult->at(prof).at(index).limitMaxZ-interLimit;
			ultimateResult->at(prof+1).at(fils0).limitMinZ=ultimateResult->at(prof).at(index).limitMinZ+interLimit;
		}
		recLimite(realZindex+1, prof+1, fils0, ultimateResult);
		recLimite(realZindex+1, prof+1, fils1, ultimateResult);
	}
	else if(ultimateResult->at(prof).at(index).Fils.size()==1)
	{
		int fils0=ultimateResult->at(prof).at(index).Fils.at(0);
			if(!ultimateResult->at(prof+1).at(fils0).Droite)
			{
				ultimateResult->at(prof+1).at(fils0).limitMinX=ultimateResult->at(prof).at(index).limitMinX;
				ultimateResult->at(prof+1).at(fils0).limitMinY=ultimateResult->at(prof).at(index).limitMinY;
				ultimateResult->at(prof+1).at(fils0).limitMinZ=ultimateResult->at(prof).at(index).limitMinZ;
				ultimateResult->at(prof+1).at(fils0).limitMaxX=ultimateResult->at(prof).at(index).limitMaxX;
				ultimateResult->at(prof+1).at(fils0).limitMaxY=ultimateResult->at(prof).at(index).limitMaxY;
				ultimateResult->at(prof+1).at(fils0).limitMaxZ=ultimateResult->at(prof).at(index).limitMaxZ;

				if(realZindex==0) 		//X
				{
					interLimit=(ultimateResult->at(prof).at(index).limitMaxX-ultimateResult->at(prof).at(index).limitMinX)/2.0;
					ultimateResult->at(prof+1).at(fils0).limitMaxX=ultimateResult->at(prof).at(index).limitMaxX-interLimit;
				}
				else if(realZindex==1)	//Y
				{
					interLimit=(ultimateResult->at(prof).at(index).limitMaxY-ultimateResult->at(prof).at(index).limitMinY)/2.0;
					ultimateResult->at(prof+1).at(fils0).limitMaxY=ultimateResult->at(prof).at(index).limitMaxY-interLimit;
				}
				else if(realZindex==2)	//Z
				{
					interLimit=(ultimateResult->at(prof).at(index).limitMaxZ-ultimateResult->at(prof).at(index).limitMinZ)/2.0;
					ultimateResult->at(prof+1).at(fils0).limitMaxZ=ultimateResult->at(prof).at(index).limitMaxZ-interLimit;
				}
				recLimite(realZindex+1, prof+1, fils0, ultimateResult);
			}
			else
			{
				ultimateResult->at(prof+1).at(fils0).limitMinX=ultimateResult->at(prof).at(index).limitMinX;
				ultimateResult->at(prof+1).at(fils0).limitMinY=ultimateResult->at(prof).at(index).limitMinY;
				ultimateResult->at(prof+1).at(fils0).limitMinZ=ultimateResult->at(prof).at(index).limitMinZ;
				ultimateResult->at(prof+1).at(fils0).limitMaxX=ultimateResult->at(prof).at(index).limitMaxX;
				ultimateResult->at(prof+1).at(fils0).limitMaxY=ultimateResult->at(prof).at(index).limitMaxY;
				ultimateResult->at(prof+1).at(fils0).limitMaxZ=ultimateResult->at(prof).at(index).limitMaxZ;

				if(realZindex==0) 		//X
				{
					interLimit=(ultimateResult->at(prof).at(index).limitMaxX-ultimateResult->at(prof).at(index).limitMinX)/2.0;
					ultimateResult->at(prof+1).at(fils0).limitMinX=ultimateResult->at(prof).at(index).limitMinX+interLimit;
				}
				else if(realZindex==1)	//Y
				{
					interLimit=(ultimateResult->at(prof).at(index).limitMaxY-ultimateResult->at(prof).at(index).limitMinY)/2.0;
					ultimateResult->at(prof+1).at(fils0).limitMinY=ultimateResult->at(prof).at(index).limitMinY+interLimit;
				}
				else if(realZindex==2)	//Z
				{
					interLimit=(ultimateResult->at(prof).at(index).limitMaxZ-ultimateResult->at(prof).at(index).limitMinZ)/2.0;
					ultimateResult->at(prof+1).at(fils0).limitMinZ=ultimateResult->at(prof).at(index).limitMinZ+interLimit;
				}
				recLimite(realZindex+1, prof+1, fils0, ultimateResult);
			}
	}
}

vector<vector<string>> setAffich(vector<vector<Groupe>>* ultimateResult)
{
	vector<vector<string>> output;
	int profondeur=0;

	while(profondeur!=ultimateResult->size()-2)
	{
		if(profondeur!=output.size()+1)
		{
			vector<string> newChallenger;
			output.push_back(newChallenger);
		}
		int comptIn=-1;
		for(int profIndex=0;profIndex<ultimateResult->at(profondeur).size();profIndex++)
		{
			
			if(profIndex%2==0)
				comptIn++;

			if(ultimateResult->at(profondeur).at(profIndex).Fils.size()==2)
			{

				int fils0=ultimateResult->at(profondeur).at(profIndex).Fils.at(0);
				int fils1=ultimateResult->at(profondeur).at(profIndex).Fils.at(1);

				bool droite=ultimateResult->at(profondeur+1).at(fils0).Vertex;
				bool gauche=ultimateResult->at(profondeur+1).at(fils1).Vertex;

				int action=0;
				if(ultimateResult->at(profondeur).at(profIndex).Droite)
				{
					action=1;
					if((gauche)&&(!droite))
						action=4;
					else if((!gauche)&&(droite))
						action=5;
					else if((gauche)&&(droite))
						action=7;
				}
				else
				{
					if((gauche)&&(!droite))
						action=2;
					else if((!gauche)&&(droite))
						action=4;
					else if((gauche)&&(droite))
						action=6;
				}
				string result=std::to_string(comptIn)+std::to_string(action);
				output.at(profondeur).push_back(result);

			}
			else if(ultimateResult->at(profondeur).at(profIndex).Fils.size()==1)
			{
				int fils0=ultimateResult->at(profondeur).at(profIndex).Fils.at(0);

				bool gauche=!ultimateResult->at(profondeur+1).at(fils0).Droite;

				int action=0;
				if(ultimateResult->at(profondeur).at(profIndex).Droite)
				{
					action=1;
					if(gauche)
						action=4;
					else
						action=5;
				}
				else
				{
					if(gauche)
						action=2;
					else
						action=3;
				}
				string result=std::to_string(comptIn)+std::to_string(action);
				output.at(profondeur).push_back(result);
			}
		
		}

		profondeur++;
	}
	return output;
}


vector<vector<int>> setAffich1(vector<vector<Groupe>>* ultimateResult)
{
	vector<vector<int>> output;
	int profondeur=0;

	while(profondeur!=ultimateResult->size()-1)///Alerte : Problème ici ? J'ai -2 normalement
	{
		if(profondeur!=output.size()+1)
		{
			vector<int> newChallenger;
			output.push_back(newChallenger);
		}
		int comptIn=-1;
		for(int profIndex=0;profIndex<ultimateResult->at(profondeur).size();profIndex++)
		{
			
			if(profIndex%2==0)
				comptIn++;

			if(ultimateResult->at(profondeur).at(profIndex).Fils.size()==2)
			{

				int fils0=ultimateResult->at(profondeur).at(profIndex).Fils.at(0);
				int fils1=ultimateResult->at(profondeur).at(profIndex).Fils.at(1);

				bool droite=ultimateResult->at(profondeur+1).at(fils0).Vertex;
				bool gauche=ultimateResult->at(profondeur+1).at(fils1).Vertex;

				int action=0;
				if(ultimateResult->at(profondeur).at(profIndex).Droite)
				{
					action=1;
					if((gauche)&&(!droite))
						action=4;
					else if((!gauche)&&(droite))
						action=5;
					else if((gauche)&&(droite))
						action=7;
				}
				else
				{
					if((gauche)&&(!droite))
						action=2;
					else if((!gauche)&&(droite))
						action=4;
					else if((gauche)&&(droite))
						action=6;
				}
				int result=(comptIn+1)*10+action;

				output.at(profondeur).push_back(result);

			}
			else if(ultimateResult->at(profondeur).at(profIndex).Fils.size()==1)
			{
				int fils0=ultimateResult->at(profondeur).at(profIndex).Fils.at(0);

				bool gauche=!ultimateResult->at(profondeur+1).at(fils0).Droite;

				int action=0;
				if(ultimateResult->at(profondeur).at(profIndex).Droite)
				{
					action=1;
					if(gauche)
						action=4;
					else
						action=5;
				}
				else
				{
					if(gauche)
						action=2;
					else
						action=3;
				}
				
				int result=(comptIn+1)*10+action;
				output.at(profondeur).push_back(result);
			}
		
		}

		profondeur++;
	}
	return output;
}

bool contains(vector<int> g, int c)
{
	for(int i=0;i<g.size();i++)
	{
		if(g.at(i)==c)
			return true;
	}
	return false;
}
bool containsTriangle(vector<unsigned int*> g,unsigned int* c)
{
	for(int i=0;i<g.size();i++)
	{
		if(((g.at(i)[0]==c[0])&&(g.at(i)[1]==c[1])&&(g.at(i)[2]==c[2]))||	
		((g.at(i)[0]==c[0])&&(g.at(i)[1]==c[2])&&(g.at(i)[2]==c[1]))||
		((g.at(i)[0]==c[1])&&(g.at(i)[1]==c[0])&&(g.at(i)[2]==c[2]))||	
		((g.at(i)[0]==c[1])&&(g.at(i)[1]==c[2])&&(g.at(i)[2]==c[0]))||
		((g.at(i)[0]==c[2])&&(g.at(i)[1]==c[0])&&(g.at(i)[2]==c[1]))||	
		((g.at(i)[0]==c[2])&&(g.at(i)[1]==c[1])&&(g.at(i)[2]==c[0])))
			return true;
	}
	return false;
}
bool contains(vector<int*> g,int* c)
{
	for(int i=0;i<g.size();i++)
	{
		if(((g.at(i)[0]==c[0])&&(g.at(i)[1]==c[1]))||((g.at(i)[0]==c[1])&&(g.at(i)[1]==c[0])))
			return true;
	}
	return false;
}

vector<int> getAdjacent(vector<float*>* vertices,vector<unsigned int*> faceVertexIndices,int indicePoint)
{
	vector<int> result;
	for(int i=0;i<faceVertexIndices.size();i++)
	{
		if(faceVertexIndices.at(i)[0]-1==indicePoint)
		{
			if(!contains(result,faceVertexIndices.at(i)[1]-1))
			{
				
				result.push_back(faceVertexIndices.at(i)[1]-1);
			}
			if(faceVertexIndices.at(i)[2]!=-1)
			{
				if(!contains(result,faceVertexIndices.at(i)[2]-1))
					result.push_back(faceVertexIndices.at(i)[2]-1);
			}
		}
		else if(faceVertexIndices.at(i)[1]-1==indicePoint)
		{
			if(!contains(result,faceVertexIndices.at(i)[0]-1))
				result.push_back(faceVertexIndices.at(i)[0]-1);
			if(faceVertexIndices.at(i)[2]!=-1)
			{
				if(!contains(result,faceVertexIndices.at(i)[2]-1))
					result.push_back(faceVertexIndices.at(i)[2]-1);
			}
		}
		else if(faceVertexIndices.at(i)[2]-1==indicePoint)
		{
			if(!contains(result,faceVertexIndices.at(i)[0]-1))
				result.push_back(faceVertexIndices.at(i)[0]-1);
			if(!contains(result,faceVertexIndices.at(i)[1]-1))
				result.push_back(faceVertexIndices.at(i)[1]-1);
		}
	}
	return result;
}

void supprAlonePoint(vector<float*>* vertices,vector<unsigned int*> faceVertexIndices)
{
	vector<bool> valide;
	for(int i=0;i<vertices->size();i++)
	{
		valide.push_back(false);
	}
	for(int i=0;i<faceVertexIndices.size();i++)
	{
		valide.at(faceVertexIndices.at(i)[0]-1)=true;
		valide.at(faceVertexIndices.at(i)[1]-1)=true;
		
		if(faceVertexIndices.at(i)[2]!=-1)
			valide.at(faceVertexIndices.at(i)[2]-1)=true;
	}
	int iBis=0;
	
	for(int i=0;i<vertices->size();i++)
	{
		if(!valide.at(iBis))
		{
			for(int y=0;y<faceVertexIndices.size();y++)
			{
				if(faceVertexIndices.at(y)[0]>i+1)
					faceVertexIndices.at(y)[0]--;
				if(faceVertexIndices.at(y)[1]>i+1)
					faceVertexIndices.at(y)[1]--;
				if(faceVertexIndices.at(y)[2]!=-1)
					if(faceVertexIndices.at(y)[2]>i+1)
						faceVertexIndices.at(y)[2]--;
			}
			vertices->erase(next(vertices->begin(), i));
			i--;
		}
		iBis++;
	}
	
}

Groupe getBoudingBox(vector<float*> vertices)
{
	Groupe bb;
	bb.limitMinX=vertices.at(0)[0];
	bb.limitMinY=vertices.at(0)[1];
	bb.limitMinZ=vertices.at(0)[2];
	bb.limitMaxX=vertices.at(0)[0];
	bb.limitMaxY=vertices.at(0)[1];
	bb.limitMaxZ=vertices.at(0)[2];

	for(int i=1;i<vertices.size();i++)
	{
		if(bb.limitMinX>vertices.at(i)[0])
			bb.limitMinX=vertices.at(i)[0];
		if(bb.limitMinY>vertices.at(i)[1])
			bb.limitMinY=vertices.at(i)[1];
		if(bb.limitMinZ>vertices.at(i)[2])
			bb.limitMinZ=vertices.at(i)[2];
		if(bb.limitMaxX<vertices.at(i)[0])
			bb.limitMaxX=vertices.at(i)[0];
		if(bb.limitMaxY<vertices.at(i)[1])
			bb.limitMaxY=vertices.at(i)[1];
		if(bb.limitMaxZ<vertices.at(i)[2])
			bb.limitMaxZ=vertices.at(i)[2];
	}
	
	bb.limitMinX-=0.01f;
	bb.limitMinY-=0.01f;
	bb.limitMinZ-=0.01f;
	bb.limitMaxX+=0.01f;
	bb.limitMaxY+=0.01f;
	bb.limitMaxZ+=0.01f;
	return bb;

}

int getNumberVertex(Groupe g,vector<float*>* vertices,Groupe bb)
{
	
	int nbVertexInGroup=0;
	for(int i=0;i<vertices->size();i++)
	{
		if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>vertices->at(i)[0])&&(g.limitMaxY>vertices->at(i)[1])&&(g.limitMaxZ>vertices->at(i)[2]))
		{
			nbVertexInGroup++;
		}
		else
		{
			if(g.limitMaxX==bb.limitMaxX)
			{
				if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>=vertices->at(i)[0])&&(g.limitMaxY>vertices->at(i)[1])&&(g.limitMaxZ>vertices->at(i)[2]))
					nbVertexInGroup++;
			}
			if(g.limitMaxY==bb.limitMaxY)
			{
				if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>vertices->at(i)[0])&&(g.limitMaxY>=vertices->at(i)[1])&&(g.limitMaxZ>vertices->at(i)[2]))
					nbVertexInGroup++;
			}
			if(g.limitMaxZ==bb.limitMaxZ)
			{
				if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>vertices->at(i)[0])&&(g.limitMaxY>vertices->at(i)[1])&&(g.limitMaxZ>=vertices->at(i)[2]))
					nbVertexInGroup++;
			}
		}
	}
	
	return nbVertexInGroup;
}

int getPoint(Groupe g,vector<float*>* vertices,Groupe bb)
{
	for(int i=0;i<vertices->size();i++)
	{
		if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>vertices->at(i)[0])&&(g.limitMaxY>vertices->at(i)[1])&&(g.limitMaxZ>vertices->at(i)[2]))
		{
			return i;
		}
		else
		{
			if(g.limitMaxX==bb.limitMaxX)
			{
				if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>=vertices->at(i)[0])&&(g.limitMaxY>vertices->at(i)[1])&&(g.limitMaxZ>vertices->at(i)[2]))
					return i;
			}
			else if(g.limitMaxY==bb.limitMaxY)
			{
				if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>vertices->at(i)[0])&&(g.limitMaxY>=vertices->at(i)[1])&&(g.limitMaxZ>vertices->at(i)[2]))
					return i;
			}
			else if(g.limitMaxZ==bb.limitMaxZ)
			{
				if((g.limitMinX<=vertices->at(i)[0])&&(g.limitMinY<=vertices->at(i)[1])&&(g.limitMinZ<=vertices->at(i)[2])&&(g.limitMaxX>vertices->at(i)[0])&&(g.limitMaxY>vertices->at(i)[1])&&(g.limitMaxZ>=vertices->at(i)[2]))
					return i;
			}
		}
	}


	return -1;
}

bool subdivideBox(Groupe p,vector<Groupe>* result)
{
	bool under = false;
	Groupe g1;
	Groupe g2;
    float interLimit=0;

	g1.limitMaxX=p.limitMaxX;
	g1.limitMaxY=p.limitMaxY;
	g1.limitMaxZ=p.limitMaxZ;

	g2.limitMaxX=p.limitMaxX;
	g2.limitMaxY=p.limitMaxY;
	g2.limitMaxZ=p.limitMaxZ;


	g1.limitMinX=p.limitMinX;
	g1.limitMinY=p.limitMinY;
	g1.limitMinZ=p.limitMinZ;

	g2.limitMinX=p.limitMinX;
	g2.limitMinY=p.limitMinY;
	g2.limitMinZ=p.limitMinZ;
	

	if(p.nextTurn==0) 		//X
	{
		interLimit=(p.limitMaxX-p.limitMinX)/2.0;
		g2.limitMaxX=p.limitMaxX-interLimit;
		g1.limitMinX=p.limitMinX+interLimit;
		under=true;
	}
	else if(p.nextTurn==1)	//Y
	{
		interLimit=(p.limitMaxY-p.limitMinY)/2.0;
		g2.limitMaxY=p.limitMaxY-interLimit;
		g1.limitMinY=p.limitMinY+interLimit;
	}
	else if(p.nextTurn==2)	//Z
	{
		interLimit=(p.limitMaxZ-p.limitMinZ)/2.0;
		g2.limitMaxZ=p.limitMaxZ-interLimit;
		g1.limitMinZ=p.limitMinZ+interLimit;
	}
	
	g1.nextTurn=p.nextTurn+1;
	g2.nextTurn=p.nextTurn+1;
	if(g1.nextTurn==3)
		g1.nextTurn=0;
	if(g2.nextTurn==3)
		g2.nextTurn=0;
	g1.Frère=&g2;
	g2.Frère=&g1;


	result->push_back(g1);
	result->push_back(g2);
	return under;
}


void test(vector<float*>* vertices)
{
	
	Groupe bb = getBoudingBox(*vertices);
    int nbbb=getNumberVertex(bb,vertices,bb);
	printf("%d\n",nbbb);
}

/*
	void operatorEdgeCollapse(Groupe* Groupe1,Groupe* Groupe2,vector<unsigned int*>* faceVertexIndices)
	{

		vector<int> n7n2;
		vector<int> cyan;
		vector<int> magenta;



			//printf("Nani : %d %d\n",j);


		//bool Add
		//printf("wait : %d %d\n",indGroupe1,indGroupe2);
		//printf("The Fuck : %zu\n",ensembleGroupe.at(indGroupe1).indexAdj.size());
		for(int i=0;i<Groupe1->indexAdj.size();i++)
		{
			//printf("The Fuck 2 : %d\n",ensembleGroupe.at(indGroupe1).indexAdj.at(i));
			for(int j=0;j<Groupe1->indexAdj.size();j++)
			{
				//printf("The Fuck : %d\n",ensembleGroupe.at(indGroupe2).indexAdj.at(j));
				if(Groupe1->indexAdj.at(j)==Groupe2->indexAdj.at(i))
				{
					n7n2.push_back(Groupe2->indexAdj.at(j));
				}
			} 
		}
		for(int i=0;i<ensembleGroupe.at(indGroupe2).indexAdj.size();i++)
		{
			printf("The Fuck 3 : %d\n",ensembleGroupe.at(indGroupe2).indexAdj.at(i));
		}
		//printf("Taille n7n2 : %zu\n",n7n2.size());
		if(n7n2.size()==0)
			n0++;
		else if(n7n2.size()==1)
			n1++;
		
		else if(n7n2.size()==2)
			n2++;
		else if(n7n2.size()>2)
			np++;




		for(int i=0;i<ensembleGroupe.at(indGroupe1).indexAdj.size();i++)
		{
			for(int j=0;j<ensembleGroupe.at(indGroupe2).indexAdj.size();j++)
			{
				if()
			}
		}

	}
	Groupe operatorPrecision(int indGroupe1,vector<Groupe> ensembleGroupe)
	{
		Groupe actu=ensembleGroupe.at(indGroupe1);
		//printf("Actu  : %f %f %f %f %f %f\n",actu.limitMinX,actu.limitMinY,actu.limitMinZ,actu.limitMaxX,actu.limitMaxY,actu.limitMaxZ);	
		//printf("Actu Père : %f %f %f %f %f %f\n",actu.Père->limitMinX,actu.Père->limitMinY,actu.Père->limitMinZ,actu.Père->limitMaxX,actu.Père->limitMaxY,actu.Père->limitMaxZ);

		Groupe Nouveau;
		//Nouveau.indexAdj=actu.Père->indexAdj;
		//Nouveau.Père=actu.Père->Père;
		//Nouveau.profondeur=actu.Père->profondeur;
		//Nouveau.Frère=actu.Père->Frère;
		
		//Nouveau.limitMinX=actu.Père->limitMinX;
		//Nouveau.limitMinY=actu.Père->limitMinY;
		//Nouveau.limitMinZ=actu.Père->limitMinZ;
		//Nouveau.limitMaxX=actu.Père->limitMaxX;
		//Nouveau.limitMaxY=actu.Père->limitMaxY;
		//Nouveau.limitMaxZ=actu.Père->limitMaxZ;

		//printf("Nouveau : %f %f %f %f %f %f\n",Nouveau.limitMinX,Nouveau.limitMinY,Nouveau.limitMinZ,Nouveau.limitMaxX,Nouveau.limitMaxY,Nouveau.limitMaxZ);
			
		return Nouveau;
	}
*/



fichierCompresse compress(vector<unsigned int*>* faceVertexIndices,vector<float*>* vertices,float precision, int nbIterationMerge)
{




	int profondeurTest=-1;


	int affich=0;

	int affichValue=0;

	Groupe bb = getBoudingBox(*vertices);
	vector<Groupe> ensembleGroupe;
	vector<Groupe> pile;


    int nbbb=getNumberVertex(bb,vertices,bb);




	vector<vector<Groupe>> ultimatePile;
	vector<vector<Groupe>> ultimateResult;




	int profondeur=0;
	vector<Groupe> newChallenger;
	ultimatePile.push_back(newChallenger);
	ultimateResult.push_back(newChallenger);
	Groupe* falseF=new Groupe;
	falseF->indicePoint=-1;
	bb.Frère=falseF;
	
	ultimatePile.at(0).push_back(bb);
	ultimateResult.at(0).push_back(bb);



	pile.push_back(bb);
	int compt=0;


	bool continue1=true;



	int compteurProd=0;


	int indexProfondeur=-1;
	while(continue1)
	{
		indexProfondeur++;
		Groupe actu=ultimatePile.at(profondeur).at(0);
		ultimatePile.at(profondeur).erase(ultimatePile.at(profondeur).begin());
		int numberOfPoint=getNumberVertex(actu,vertices,bb);
		if(numberOfPoint>1)
		{
			vector<Groupe> twoG;
			subdivideBox(actu,&twoG);
				if(ultimatePile.size()-1!=profondeur+1)
				{
					vector<Groupe> newChallenger;
					ultimatePile.push_back(newChallenger);
					ultimateResult.push_back(newChallenger);
				}
				
				twoG.at(0).Droite=true;
				twoG.at(0).indexPère=indexProfondeur;
				twoG.at(1).indexPère=indexProfondeur;

				ultimatePile.at(profondeur+1).push_back(twoG.at(0));
				ultimatePile.at(profondeur+1).push_back(twoG.at(1));
				ultimateResult.at(profondeur+1).push_back(twoG.at(0));
				ultimateResult.at(profondeur+1).push_back(twoG.at(1));
				
				
				
				actu.Fils.push_back(ultimateResult.at(profondeur+1).size()-2);
				actu.Fils.push_back(ultimateResult.at(profondeur+1).size()-1);
				ultimateResult.at(profondeur).at(indexProfondeur).Fils.push_back(ultimateResult.at(profondeur+1).size()-2);
				ultimateResult.at(profondeur).at(indexProfondeur).Fils.push_back(ultimateResult.at(profondeur+1).size()-1);
		}
		else
		{
			if(numberOfPoint==0)
			{

			}
			else
			{
				actu.indicePoint=getPoint(actu,vertices,bb);
				actu.numberVertex=1;


				float point[3];
				point[0]=(actu.limitMinX+actu.limitMaxX)/2.0;
				point[1]=(actu.limitMinY+actu.limitMaxY)/2.0;
				point[2]=(actu.limitMinZ+actu.limitMaxZ)/2.0;

				float distance=sqrt(pow(point[0]-vertices->at(actu.indicePoint)[0],2)+pow(point[1]-vertices->at(actu.indicePoint)[1],2)+pow(point[2]-vertices->at(actu.indicePoint)[2],2));
				if(distance<precision)
				{
					
					actu.indexAdj=getAdjacent(vertices,*faceVertexIndices,actu.indicePoint);
					actu.profondeur=profondeur;
					ultimateResult.at(profondeur).at(indexProfondeur).Vertex=true;
					ensembleGroupe.push_back(actu);
					
					int percent=(int)round(float(compt)/nbbb*100);
					if(( percent%10==0)&&(affichValue!= percent))
					{
						printf("--%d%% \n", percent);
						affichValue=percent;
					}
					compt++;
				}
				else
				{
					vector<Groupe> twoG;
						subdivideBox(actu,&twoG);

						if(ultimatePile.size()-1!=profondeur+1)
						{
							vector<Groupe> newChallenger;
							ultimatePile.push_back(newChallenger);
							ultimateResult.push_back(newChallenger);
						}
						

						if(getNumberVertex(twoG.at(0),vertices,bb)==1)
						{
 							Groupe* falseF=new Groupe;
							falseF->indicePoint=-1;
							twoG.at(0).Frère=falseF;

							
							twoG.at(0).indexPère=indexProfondeur;
							twoG.at(0).Droite=true;

							ultimatePile.at(profondeur+1).push_back(twoG.at(0));
							ultimateResult.at(profondeur+1).push_back(twoG.at(0));
							actu.Fils.push_back(ultimateResult.at(profondeur+1).size()-1);
							ultimateResult.at(profondeur).at(indexProfondeur).Fils.push_back(ultimateResult.at(profondeur+1).size()-1);
						}
						else
						{
							Groupe* falseF=new Groupe;
							falseF->indicePoint=-1;
							twoG.at(1).Frère=falseF;

							
							twoG.at(1).indexPère=indexProfondeur;

							ultimatePile.at(profondeur+1).push_back(twoG.at(1));
							ultimateResult.at(profondeur+1).push_back(twoG.at(1));
							
							actu.Fils.push_back(ultimateResult.at(profondeur+1).size()-1);
							ultimateResult.at(profondeur).at(indexProfondeur).Fils.push_back(ultimateResult.at(profondeur+1).size()-1);
						}
				}

			}
		}
		if(ultimatePile.at(profondeur).size()==0)
		{
			if(ultimatePile.size()-1!=profondeur+1)
				continue1=false;
			if(profondeur==profondeurTest)
				continue1=false;
			profondeur++;
			indexProfondeur=-1;
			
		}
	}
	printf("Fin : %zu\n",ensembleGroupe.size());
	
	vector<float*> vertices2;
	for(int i=0;i<ensembleGroupe.size();i++)
	{
		if(ensembleGroupe.at(i).numberVertex!=0)
		{
			float* point=new float[3]();
			point[0]=0; //(ensembleGroupe.at(i).limitMinX+ensembleGroupe.at(i).limitMaxX)/2.0;
			point[1]=0; //(ensembleGroupe.at(i).limitMinY+ensembleGroupe.at(i).limitMaxY)/2.0;
			point[2]=0; //(ensembleGroupe.at(i).limitMinZ+ensembleGroupe.at(i).limitMaxZ)/2.0;
			vertices2.push_back(point);
		}
	}
	vertices->swap(vertices2);
	
	
	replaceFace(faceVertexIndices,ensembleGroupe);



	int control=0;
	for(int j=0;j<ensembleGroupe.size();j++)
	{
		for(int x=0;x<ensembleGroupe.at(j).indexAdj.size();x++)
		{
			for(int y=0;y<ensembleGroupe.size();y++)
			{
				if(ensembleGroupe.at(y).indicePoint==ensembleGroupe.at(j).indexAdj.at(x))
				{
					ensembleGroupe.at(j).indexAdj.at(x)=y;
					y=ensembleGroupe.size();
				}
			}
		}
	}
	printf("Safe point %zu\n",ensembleGroupe.size());






	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	//////////////////////Fin Gestion Père et Fils//////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	
	//recLimite2(0,0,0, &ultimateResult);
	//recLimite(0,0,0, &ultimateResult);

	fichierCompresse fC;
	fC.limitMinX=ultimateResult.at(0).at(0).limitMinX;
	fC.limitMinY=ultimateResult.at(0).at(0).limitMinY;
	fC.limitMinZ=ultimateResult.at(0).at(0).limitMinZ;
	fC.limitMaxX=ultimateResult.at(0).at(0).limitMaxX;
	fC.limitMaxY=ultimateResult.at(0).at(0).limitMaxY;
	fC.limitMaxZ=ultimateResult.at(0).at(0).limitMaxZ;
	fC.NbProfondeur=ultimateResult.at(0).size();
	fC.faceVertexIndices=*faceVertexIndices;
	fC.arbreBinaire=setAffich1(&ultimateResult);






	ensembleGroupe.clear();
	for(int i=0;i<ultimateResult.size();i++)
	{
		for(int j=0;j<ultimateResult.at(i).size();j++)
		{
			if(ultimateResult.at(i).at(j).Vertex)
			{
				ensembleGroupe.push_back(ultimateResult.at(i).at(j));
			}
		}
	}


	printf("Ensemble groupe taille : %zu\n",ensembleGroupe.size());
	vector<float*> vertices3;

	for(int i=0;i<ensembleGroupe.size();i++)
	{

			float* point=new float[3]();
			point[0]=(ensembleGroupe.at(i).limitMinX+ensembleGroupe.at(i).limitMaxX)/2.0;
			point[1]=(ensembleGroupe.at(i).limitMinY+ensembleGroupe.at(i).limitMaxY)/2.0;
			point[2]=(ensembleGroupe.at(i).limitMinZ+ensembleGroupe.at(i).limitMaxZ)/2.0;

			vertices3.push_back(point);
	}
	vertices->swap(vertices3);

	return fC;
}

void decompress(fichierCompresse input ,vector<unsigned int*>* faceVertexIndices,vector<float*>* vertices)
{


	vector<vector<Groupe>> ultimateResult;
	vector<Groupe> newChallenger;
	ultimateResult.push_back(newChallenger);

	Groupe bb;

	bb.limitMinX=input.limitMinX;
	bb.limitMinY=input.limitMinY;
	bb.limitMinZ=input.limitMinZ;
	bb.limitMaxX=input.limitMaxX;
	bb.limitMaxY=input.limitMaxY;
	bb.limitMaxZ=input.limitMaxZ;
	
	ultimateResult.at(0).push_back(bb);

	faceVertexIndices=&input.faceVertexIndices;

	int maxProf=0;

	for(int i=0;i<input.arbreBinaire.size();i++)
	{
		if(i+1>maxProf)
		{
			vector<Groupe> newChallenger;
			ultimateResult.push_back(newChallenger);
			maxProf++;
		}


		for(int j=0;j<input.arbreBinaire.at(i).size();j++)
		{

			int index=(input.arbreBinaire.at(i).at(j)/10)-1;
			int action=input.arbreBinaire.at(i).at(j)-((input.arbreBinaire.at(i).at(j)/10)*10);

			
			bool gauche, droite;
			if(action==0)
			{
				gauche=false;
				droite=false;
			}
			else if(action==1)
			{
				gauche=false;
				droite=false;
				index+=1;
			}
			else if(action==2)
			{
				gauche=true;
				droite=false;
			}
			else if(action==3)
			{
				gauche=false;
				droite=true;
			}
			else if(action==4)
			{
				gauche=true;
				droite=false;
				index+=1;
			}
			else if(action==5)
			{
				gauche=false;
				droite=true;
				index+=1;
			}
			else if(action==6)
			{
				gauche=true;
				droite=true;
			}
			else if(action==7)
			{
				gauche=true;
				droite=true;
				index+=1;
			}
			//printf("Index : %d ; Action : %d  ; %d %d\n",index,action,gauche,droite);
			Groupe f1;
			Groupe f2;
			f1.Vertex=gauche;
			f2.Vertex=droite;
			f2.Droite=true;
			ultimateResult.at(i+1).push_back(f2);
			ultimateResult.at(i+1).push_back(f1);


			ultimateResult.at(i).at(index).Fils.push_back(ultimateResult.at(i+1).size()-2);
			ultimateResult.at(i).at(index).Fils.push_back(ultimateResult.at(i+1).size()-1);
			//printf("%zu\n",ultimateResult.at(i).at(index).Fils.size());
		}
	}


	recLimite(0,0,0, &ultimateResult);

	vector<Groupe> ensembleGroupe;
	for(int i=0;i<ultimateResult.size();i++)
	{
		for(int j=0;j<ultimateResult.at(i).size();j++)
		{
			if(ultimateResult.at(i).at(j).Vertex)
			{
				ensembleGroupe.push_back(ultimateResult.at(i).at(j));
			}
		}
	}


	printf("Ensemble groupe taille : %zu\n",ensembleGroupe.size());
	vector<float*> vertices3;

	for(int i=0;i<ensembleGroupe.size();i++)
	{

			float* point=new float[3]();
			point[0]=(ensembleGroupe.at(i).limitMinX+ensembleGroupe.at(i).limitMaxX)/2.0;
			point[1]=(ensembleGroupe.at(i).limitMinY+ensembleGroupe.at(i).limitMaxY)/2.0;
			point[2]=(ensembleGroupe.at(i).limitMinZ+ensembleGroupe.at(i).limitMaxZ)/2.0;

			vertices3.push_back(point);
	}
	vertices->swap(vertices3);

}