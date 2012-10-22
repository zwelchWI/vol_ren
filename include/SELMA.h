//*******************************************************************
//
//   ImplicitSurfaces.h
//
// Implicit Surfaces
//
//*******************************************************************

#ifndef SELMA
#define SELMA

#include "Volume.h"
#include "BaseGrid.h"
#include "Vector.h"
#include <cmath>
using namespace std;
namespace lux{
class Selma : public Volume<Vector>{
public:
	Selma(
		  BaseGrid<Vector> *grid_,
		  Volume<Vector> *U_,
		  Volume<float> *dt_,
		  float scale_ = 1.0):grid(grid_),U(U_),dt(dt_),scale(scale_){
		
		grid->sample(U);
	}
	const Vector eval(const Vector &P) const {
		return P - grid->get(P) * dt->eval(P) * scale;
	}
	
private:
	BaseGrid<Vector> *grid;
	Volume<Vector> *U;
	Volume<float> *dt;
	float scale;
	
};

class Composition : public Volume<float>{
public:
	Composition(Volume<float> *f_,Volume<Vector> *X_):f(f_),X(X_){};
	const float eval(const Vector &P) const {
		return f->eval(X->eval(P));
	}
private:
	Volume<float> *f;
	Volume<Vector> *X;
};
	

}   
    
#endif
