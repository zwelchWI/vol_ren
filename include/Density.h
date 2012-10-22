//*******************************************************************
//
//   Density.h
//
// Density/Color "cloud"
//
//*******************************************************************

#ifndef DENSITY
#define DENSITY

#include "Volume.h"
#include "ColorFields.h"
#include <cmath>
using namespace std;
namespace lux{
class Density{
public:
	//EXPECTS UNCLAMPED VOLUME
	Density(Volume<float> *p_,Volume<Color> *Cd_ = NULL,double k_ = 1,bool ramp = true){
		if(ramp == true)
			p = new ClampVolume(p_,0.0,1.0);
		else
			p = new MaskVolume(p_);
		if(Cd_ == NULL)
			cd = new SolidColorField(Color(1.0,1.0,1.0,0.0));
		else
			cd = Cd_;
		k = k_;
	}
	const float P(const Vector &X) const {
		return p->eval(X);
	}
	const Color Cd(const Vector &X) const {
		return cd->eval(X);
	}
	const double K(){return k;}
	
	
	
private:
	Volume<float> *p;
	Volume<Color> *cd;
	double k;
	
};
	
	
}   
    
#endif
