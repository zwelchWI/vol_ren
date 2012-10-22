//*******************************************************************
//
//   StampNoise.h
//
// StampNoise class
//
//*******************************************************************

#ifndef STAMPNOISE
#define STAMPNOISE

#include "Volume.h"
#include <cmath>
#include "PerlinNoise.h"
#include "Noise.h"
#include "Particle.h"
#include "BaseGrid.h"
#include "UniformPRN.h"

using namespace std;
namespace lux{
    
class StampNoise{
public:
    StampNoise(const Particle *guide_){
		guide = guide_;
		Noise_t param;
		param.octaves    = guide->octaves();
		param.fjump      = guide->fjump();
		param.roughness  = guide->roughness();
		param.wavelength = guide->freq(); 
		param.translate  = guide->translate(); 
		param.offset     = guide->offset();
		fs = new FractalSum<PerlinNoise>();
		fs->setParameters(param);
	}
    void stamp(BaseGrid<float> *Dens,BaseGrid<float> *ageGrid = NULL,BaseGrid<Color> *C = NULL){
		Vector bbLLC(guide->P()[0] - guide->pscale(),
					 guide->P()[1] - guide->pscale(),
					 guide->P()[2] - guide->pscale());
		Vector bbURC(guide->P()[0] + guide->pscale(),
					 guide->P()[1] + guide->pscale(),
					 guide->P()[2] + guide->pscale());
		
		const float dx = Dens->DX();
		const float dy = Dens->DY();
		const float dz = Dens->DZ();
		
		
		Vector adjbbLLC = bbLLC - Dens->LLC();
		Vector adjbbURC = bbURC - Dens->LLC();	
		
		adjbbLLC = Vector(adjbbLLC[0] / dx,adjbbLLC[1] / dy,adjbbLLC[2] / dz);
		adjbbURC = Vector(adjbbURC[0] / dx,adjbbURC[1] / dy,adjbbURC[2] / dz);
		
		float value,fade;
		for(float k = adjbbLLC[2]; k < adjbbURC[2];k += 1.0){
			for(float j = adjbbLLC[1]; j < adjbbURC[1];j += 1.0){
				for(float i = adjbbLLC[0]; i < adjbbURC[0];i += 1.0){
		//			printf("here %f %f %f\n",(Dens->getVect(i,j,k))[0],(Dens->getVect(i,j,k))[1],(Dens->getVect(i,j,k))[2]);
			//		printf("%f %f %f\n",i,j,k);
					if((Dens->getVect(i,j,k) - guide->P()).magnitude() < guide->pscale()){ 
						value = fs->eval(Dens->getVect(i,j,k)) * guide->opacity();
						if(guide->fade() > 0){
							fade = (1 - tanh(( (Dens->getVect(i,j,k) - guide->P()).magnitude()) * guide->fade()))/(1 - tanh(guide->fade()));
							value *= fade;
						}
						//(1 - tanh( (|Pijk-P0.|/pscale)/fadescale))/(1-tanh(1/fadescale));
						if(value > Dens->get(i,j,k)){
						//	printf("here %f\n",value);
							Dens->set(i,j,k,value);
							if(C!= NULL)
								C->set(i,j,k,guide->Cd());
						}
					
					}	
				}
			}
			printf("STAMPING NOISE %.1f %%\r",((k-adjbbLLC[2])/(adjbbURC[2]-adjbbLLC[2]) * 100.0));

		}
		
		printf("\rSAMPLING NOISE COMPLETE\n");

		
		if(ageGrid != NULL){
			
			
			const float gdx = ageGrid->DX();
			const float gdy = ageGrid->DY();
			const float gdz = ageGrid->DZ();
			
			
			adjbbLLC = bbLLC - ageGrid->LLC();
			adjbbURC = bbURC - ageGrid->LLC();	
			
			adjbbLLC = Vector(adjbbLLC[0] / gdx,adjbbLLC[1] / gdy,adjbbLLC[2] / gdz);
			adjbbURC = Vector(adjbbURC[0] / gdx,adjbbURC[1] / gdy,adjbbURC[2] / gdz);
			
			float value,fade;
			for(float k = adjbbLLC[2]; k < adjbbURC[2];k += 1.0){
				for(float j = adjbbLLC[1]; j < adjbbURC[1];j += 1.0){
					for(float i = adjbbLLC[0]; i < adjbbURC[0];i += 1.0){
						//			printf("here %f %f %f\n",(Dens->getVect(i,j,k))[0],(Dens->getVect(i,j,k))[1],(Dens->getVect(i,j,k))[2]);
						//		printf("%f %f %f\n",i,j,k);
						if(value = guide->age()/guide->lifetime() > 0){ 
							if(value > ageGrid->get(i,j,k)){
								//	printf("here %f\n",value);
								ageGrid->set(i,j,k,value);
								
							}
							
						}	
					}
				}
				printf("STAMPING PYRO AGE %.1f %%\r",((k-adjbbLLC[2])/(adjbbURC[2]-adjbbLLC[2]) * 100.0));
				
			}
			
			printf("\rSAMPLING PYRO AGE COMPLETE\n");			
		}		
	}
	float age(){return guide->age();}
	float lifetime(){return guide->lifetime();}

	
private:
	const Particle *guide;
	FractalSum<PerlinNoise> *fs;
};
    
}   
    
#endif
