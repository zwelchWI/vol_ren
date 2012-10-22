//*******************************************************************
//
//   DSM.h
//
// Deep Shadow Map
//
//*******************************************************************

#ifndef DSMFLAG
#define DSMFLAG


#include "Vector.h"
#include "Volume.h"
#include "BaseGrid.h"
#include "Light.h"
#include <cstdio>
#include <cmath>
#include <vector>

namespace lux{
class DSM{
public:
	DSM(BaseGrid<float> *Grid,Volume<float> *Vol,float dS):grid(Grid),vol(Vol),ds(dS),light(NULL){};
	
	Light * getLight(){return light;}
	void sample(Light *l){
		light = l;
		Vector nHat;
		float S;
		float SMax;
		Vector X;
		float T,dT;
		for(int i = 0;i < grid->NX();i++){
			for(int j = 0;j < grid->NY();j++){
				for(int k = 0;k < grid->NZ();k++){		
					if(vol->eval(grid->getVect(i,j,k))>0){
						nHat = light->getPos() -  grid->getVect(i,j,k);
						nHat.normalize();
						S = 0;
						X = grid->getVect(i,j,k);
						T = 0;
						SMax = (light->getPos() -  grid->getVect(i,j,k)).magnitude();
						while(S < SMax){
							X += nHat * ds;
							S+=ds;
							T +=ds *vol->eval(X);
						}
						grid->set(i,j,k,T);
					}
					
				}
			}
			printf("SETTING DSM %.1f %%\r",(i * 100.0/grid->NX()));
			fflush(stdout);
		
		}
		printf("\rSAMPLING DSM COMPLETE\n");
	}
	

					
	
	float get(Vector P){return grid->get(P);}
	
	
private:
	BaseGrid<float> *grid;
	Volume<float>   *vol;
	float ds;
	Light *light;
};	
	
class DSMgroup{
public:
	DSMgroup(Volume<float> *K):k(K){};
	void add(DSM *dsm){
		if(dsm->getLight() == NULL){
			fprintf(stderr,"DSM NOT INITIALIZED, not adding\n");
			return;
		}
		DSMs.push_back(dsm);
	}
	Color illuminate(Vector P){
		if(DSMs.size() == 0){
			return Color(1.0,1.0,1.0,1.0);
		}
		Color ret;
		DSM *dsm;
		Light *light;
		for(int ndx = 0; ndx < DSMs.size(); ndx++){
			dsm = DSMs[ndx];
			light = dsm->getLight();
			ret = ret + ::exp(-1.0  * dsm->get(P) * k->eval(P)) * light->getColor();
		}
		return ret;
	}
	
private:
	vector<DSM *> DSMs;
	Volume<float> *k;
};

	
}

#endif


