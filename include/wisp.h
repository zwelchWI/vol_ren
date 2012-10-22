//*******************************************************************
//
//   wisp.h
//
// wisp class
//
//*******************************************************************

#ifndef WISPY
#define WISPY

#include "Volume.h"
#include <cmath>
#include "PerlinNoise.h"
#include "Noise.h"
#include "Particle.h"
#include "BaseGrid.h"
#include "UniformPRN.h"

using namespace std;
namespace lux{
    
class Wisp{
public:
    Wisp(const Particle *guide_){
		guide = guide_;
		prn = new UniformPRN(guide->id());
		
		
		Noise_t walkparam;
		walkparam.octaves    = guide->octaves();
		walkparam.fjump      = guide->fjump();
		walkparam.roughness  = guide->roughness();
		walkparam.wavelength = guide->freq(); 
		walkparam.translate  = guide->translate(); 
		walkparam.offset     = guide->offset();
		walkfs = new FractalSum<PerlinNoise>();
		walkfs->setParameters(walkparam);
		
		
		Noise_t wispparam;
		wispparam.octaves    = guide->wispOctaves();
		wispparam.fjump      = guide->wispFjump();
		wispparam.roughness  = guide->wispRoughness();
		wispparam.wavelength = guide->wispFreq(); 
		wispparam.translate  = guide->wispTranslate(); 
		wispparam.offset     = guide->wispOffset();
		wispfs = new FractalSum<PerlinNoise>();
		wispfs->setParameters(wispparam);
	}
    void stamp(BaseGrid<float> *Dens,BaseGrid<float> *ageGrid = NULL,BaseGrid<Color> *C = NULL){
		
		float x,y,z,Vx,Vy,Vz;
		x=y=z=0.0;
		Vector P0,P1,X,Xwisp;
		float r;
		for(int ndx = 0;ndx < guide->nbWisps();ndx++){
			Vx = 2 * prn->eval() -1;
			Vy = 2 * prn->eval() -1;
			Vz = 2 * prn->eval() -1;
			x = x * (1 -guide->wispCorrelation()) + Vx *guide->wispCorrelation();
			y = y * (1 -guide->wispCorrelation()) + Vy *guide->wispCorrelation();
			z = z * (1 -guide->wispCorrelation()) + Vz *guide->wispCorrelation();
			if(guide->wispRadialGroup()){
				x = x/pow(x*x + y*y + z * z,guide->wispRadialGroup());
				y = y/pow(x*x + y*y + z * z,guide->wispRadialGroup());
				z = z/pow(x*x + y*y + z * z,guide->wispRadialGroup());
			}
			P0=Vector(x,y,z);
			P0.normalize();
			r = pow(abs(walkfs->eval(Vector(x,y,z))),guide->wispGamma());
			P1 = P0 * r;
			X = guide->P() + guide->pscale() * (P1[0] * guide->right() + P1[1] * guide->up() + P1[2] * guide->normal());
			Xwisp = X + wispfs->evalVec(X) * guide->wispDispScale();
			Dens->accum(Xwisp,guide->opacity());
			if(ageGrid != NULL){
				float value = guide->age()/guide->lifetime();
				if(value > ageGrid->get(Xwisp))
					ageGrid->set(Xwisp,value);
			}
			if(C != NULL)
				C->accum(Xwisp,guide->Cd());
			if(ndx%1000 == 0){
				printf("STAMPING %d of %d\r",ndx,guide->nbWisps());
				fflush(stdout);
			}
		}
		
		printf("\rSAMPLING WISP COMPLETE          \n");

	}
	float age(){return guide->age();}
	float lifetime(){return guide->lifetime();}
private:
	const Particle *guide;
	PRN *prn;
	FractalSum<PerlinNoise> *walkfs;
	FractalSum<PerlinNoise> *wispfs;
};
    
}   
    
#endif
