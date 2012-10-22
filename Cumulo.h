//*******************************************************************
//
//   pyroclast.h
//
// pyroclast class
//
//*******************************************************************

#ifndef CUMULO
#define CUMULO

#include "Volume.h"
#include <cmath>
#include "PerlinNoise.h"
#include "Noise.h"
#include "ImplicitSurfaces.h"
#include "Particle.h"
using namespace std;
namespace lux{
    
class Cumulo : public Volume<float>{
public:
    Cumulo(Volume<float> *l,Particle *guide_){
		L = l;
		L0 = NULL;
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
    Cumulo(Volume<float> *l0,Particle *guide_,Volume<float> *base_,Particle *baseP_){
		L = NULL;
		L0 = l0;
		guide = guide_;
		base  = base_;
		baseP = baseP_;
		
		Noise_t param;
		param.octaves    = guide->octaves();
		param.fjump      = guide->fjump();
		param.roughness  = guide->roughness();
		param.wavelength = guide->freq(); 
		param.translate  = guide->translate(); 
		param.offset     = guide->offset();
		fs = new FractalSum<PerlinNoise>();
		fs->setParameters(param);
		param.octaves    = baseP->octaves();
		param.fjump      = baseP->fjump();
		param.roughness  = baseP->roughness();
		param.wavelength = baseP->freq(); 
		param.translate  = baseP->translate(); 
		param.offset     = baseP->offset();
		basefs = new FractalSum<PerlinNoise>();
		basefs->setParameters(param);

	}

    const float eval(const Vector &P){
        
		Vector X = getX(P);
		
		if(L != NULL){
			if(isnan(L->eval(P))){printf("\n2NAN HERE %f %f %f \n",P[0],P[1],P[2]);
				exit(0);
			}
			if(isnan(fs->eval(X))){printf("\n3NAN HERE %f %f %f \n",X[0],X[1],X[2]);
				exit(0);
			}
			return(L->eval(P) + pow(abs(fs->eval(X)),guide->pyroGamma())/10);
		}
		
		return L0->eval(P) + pow(abs(fs->eval(X)),guide->pyroGamma()) * clampNoise(P)/5; 
	
	}
	float clampNoise(const Vector &P){
		//return 1.0;
		//calculate X
		Vector X;
		Vector grad;			
			//gonna cheat and assume its an sdf
		X = P;
		
		
		int sign,newSign;
		if(base->eval(X)< 0)
			sign = 1;
		else sign = -1;
		float ds = .1;
		int count = 0; 
		grad = Vector((base->eval(X) - base->eval(X - Vector(ds,0,0)))/ds,
					  (base->eval(X) - base->eval(X - Vector(0,ds,0)))/ds,
					  (base->eval(X) - base->eval(X - Vector(0,0,ds)))/ds);
		//
		grad.normalize();
		while(abs(base->eval(X)) > 10e-3&& ds > 10e-3 && count < 100){
			
			
			X += sign * ds * grad;
			
			if(sign * base->eval(X) >= 0.0){
				sign *= -1;
				ds /= 2.0;
			}
			grad = Vector((base->eval(X) - base->eval(X - Vector(ds,0,0)))/ds,
						  (base->eval(X) - base->eval(X - Vector(0,ds,0)))/ds,
						  (base->eval(X) - base->eval(X - Vector(0,0,ds)))/ds);
			//
			grad.normalize();
			count++;
			
		}

		
		
		float retVal = pow(abs(basefs->eval(X)),baseP->pyroGamma());
		if(retVal > 1)retVal = 1;
		if(retVal < 0)retVal = 0;
		return retVal;
		
	}
	
	Vector getX(const Vector &P){
		Vector X;
		Vector grad;
		if(L != NULL){
			
			//gonna cheat and assume its an sdf
	//		grad = Vector((L->eval(P) - L->eval(P - Vector(.05,0,0)))/.05,
	//					  (L->eval(P) - L->eval(P - Vector(0,.05,0)))/.05,
	//					  (L->eval(P) - L->eval(P - Vector(0,0,.05)))/.05);
//
//			grad.normalize();
		//	printf("\ngrad %f %f %f \n",P[0],P[1],P[2]);

			X = P;
			
			
			int sign,newSign;
			if(L->eval(X)< 0)
				sign = 1;
			else sign = -1;
			float ds = .1;
			int count = 0; 
			grad = Vector((L->eval(X) - L->eval(X - Vector(ds,0,0)))/ds,
						  (L->eval(X) - L->eval(X - Vector(0,ds,0)))/ds,
						  (L->eval(X) - L->eval(X - Vector(0,0,ds)))/ds);
			//
			grad.normalize();
			while(abs(L->eval(X)) > 10e-3&& ds > 10e-3 && count < 100){
			

				X += sign * ds * grad;
				
				if(sign * L->eval(X) >= 0.0){
					sign *= -1;
					ds /= 2.0;
				}
				grad = Vector((L->eval(X) - L->eval(X - Vector(ds,0,0)))/ds,
							  (L->eval(X) - L->eval(X - Vector(0,ds,0)))/ds,
							  (L->eval(X) - L->eval(X - Vector(0,0,ds)))/ds);
				//
				grad.normalize();
				count++;
				
			}
		//	printf("done");
			return(X);
		}
	//	grad = Vector((L0->eval(P) - L0->eval(P - Vector(ds,0,0)))/ds,
	//				  (L0->eval(P) - L0->eval(P - Vector(0,ds,0)))/ds,
	//				  (L0->eval(P) - L0->eval(P - Vector(0,0,.ds)))/ds);

	//	grad.normalize();
		X = P;
		
		int sign,newSign;
		if(L0->eval(X)< 0)
			sign = 1;
		else sign = -1;
		float ds = .1;
		grad = Vector((L0->eval(X) - L0->eval(X - Vector(ds,0,0)))/ds,
					  (L0->eval(X) - L0->eval(X - Vector(0,ds,0)))/ds,
					  (L0->eval(X) - L0->eval(X - Vector(0,0,ds)))/ds);
		
		grad.normalize();
		int count = 0;
		while(abs(L0->eval(X)) > 10e-3 && ds > 10e-3 && count < 100){
			
		
			X += sign * ds * grad;
			
		
			if(sign * L0->eval(X) >= 0.0){
				sign *= -1;
				ds /= 2.0;
			}
			grad = Vector((L0->eval(X) - L0->eval(X - Vector(ds,0,0)))/ds,
						  (L0->eval(X) - L0->eval(X - Vector(0,ds,0)))/ds,
						  (L0->eval(X) - L0->eval(X - Vector(0,0,ds)))/ds);
			
			grad.normalize();
			count++;
	//		printf("\ngrad: %f %f %f \n",grad[0],grad[1],grad[2]);
	//		printf("\nds %f\n",ds);
	//		printf("\nsign %d \n",sign);
	//		printf("\nP: %f %f %f \n",P[0],P[1],P[2]);
	//		printf("\nX: %f %f %f \n",X[0],X[1],X[2]);
	//		printf("\nvalue %f\n",L0->eval(X));
		}
	//	printf("done1\n");
		return(X);
	}
	
	
private:
	Volume<float> *L;
	Volume<float> *L0;
    Particle* guide;
	FractalSum<PerlinNoise> *fs;
	Volume<float> *base;
	Particle *baseP;
	FractalSum<PerlinNoise> *basefs;

};
	
	
class CumuloVolume : public Volume<float>{
public:
	CumuloVolume(Cumulo *cloud_):cloud(cloud_){};
	const float eval(const Vector &P) const{
		cloud->eval(P);
	}
	
private:
	Cumulo *cloud;
};

    
}   
    
#endif
