//*******************************************************************
//
//   ColorFields.h
//
// Color fields
//
//*******************************************************************

#ifndef COLORFIELD
#define COLORFIELD

#include "Volume.h"
#include "Color.h"
namespace lux{
    
class SolidColorField: public Volume<Color>{
public:

    SolidColorField(const Color &color_):color(color_){};
    SolidColorField(const float &r,const float &g,const float &b,const float &a):color(r,g,b,a){};

    const Color eval(const Vector &X) const{
        return color;
    }
private:
    const Color color;
};

	
class ShapedColorField: public Volume<Color>{
public:
	ShapedColorField(Volume<float> *f_,Color color_){
		f = new ClampVolume(f_,0,1);
		color = color_;
	}
	const Color eval(const Vector &X) const{
        if(f->eval(X) > 0){
			return color;
		}
		else{
			return Color(0.0,0.0,0.0,0.0);
		}
    }

private:
	Volume<float> *f;
	Color color;
	
};
	
class UnionColor : public Volume<Color>{
public:
	UnionColor(Volume<Color> *f_,Volume<Color> *g_):f(f_),g(g_){};
    const Color eval(const Vector &P) const {
        if(g->eval(P) == Color(0.0,0.0,0.0,0.0))
			return f->eval(P);
		else if(f->eval(P) == Color(0.0,0.0,0.0,0.0))
			return g->eval(P);
		
		return ((f->eval(P) + g->eval(P))/2);
    }
private:
    Volume<Color> *f;
    Volume<Color> *g;

	
	
};

	
class MixColors : public Volume<Color>{
public:
	MixColors(Volume<Color> *f_,Volume<Color> *g_,float fperc_):f(f_),g(g_),fperc(fperc_){};
	const Color eval(const Vector &P) const {
		if(g->eval(P) == Color(0.0,0.0,0.0,0.0))
			return f->eval(P);
		else if(f->eval(P) == Color(0.0,0.0,0.0,0.0))
			return g->eval(P);
		
		return ((f->eval(P)*fperc + g->eval(P)*(1-fperc)));
	}
private:
	Volume<Color> *f;
	Volume<Color> *g;
	float fperc;
	
	
};

	
	
class TempColor : public Volume<Color>{
public:
	TempColor(Volume<float> *py,float T):pyro(py),Temp(T){};
    const Color eval(const Vector &P) const {
		float dens = pyro->eval(P);
		if(dens < 0)
			dens = 0;
		if(dens > 1) 
			dens =1;
		float effTemp = dens * Temp;
		float x,y,z;
		x = 0;
		y = 0;
		if (effTemp == 0)
			return Color();
		if(effTemp >= 1667 && effTemp < 4000){
			x = -.2661239 * 1e9/(effTemp*effTemp*effTemp) - .234358 * 1e6/(effTemp*effTemp) + 0.8776956 * 1e3/effTemp + .179910;
			if(effTemp <= 2222)
				y = -1.1063814 * x * x * x - 1.3481102 * x * x + 2.18555832 * x -.20219683;
			else
				y = -.9549476 * x * x * x - 1.37418593 * x * x + 2.09137015 * x -.16748867;
		}
			
		else if(effTemp >=4000 && effTemp <= 25000){
			x = -3.0258469 * 1e9/effTemp/effTemp/effTemp + 2.1070379 * 1e6/effTemp/effTemp + .2226347 * 1e3/effTemp + .24039;
			y = 3.081758 * x * x * x - 5.8733867 * x * x + 3.75112997 * x -.37001483;
		//	if(effTemp == 5000)
		//	printf("Here2 %f %f %f\n",effTemp, x,y);

		}
		else
			return Color();
		
		
		float X,Y,Z;
		Y = 1;
		X = x/y;
		Z = (1-x-y)/y;
//		x = x/y;
//		z = (1 - x - y);
//		y = 1;

	//	Vector RGB = Matrix(3.1956,-2.5455,0,2.4478,7.0492,0,-.1434,.9963,1) * Vector(X,Y,Z);
	
//		Vector RGB = Matrix(3.2406,-.9689,0.0557,-1.5372,1.8758,-.204,-.4986,.0415,1.057) * Vector(X,Y,Z);
		Vector RGB = Matrix(3.2406,-1.5372,-.4986,-.9689,1.8758,.0415,.0557,-.204,1.057) * Vector(X,Y,Z);

		
		//	printf("%f %f %f\n",RGB[0],RGB[1],RGB[2]);
	/*	if(RGB[0] >1)
			RGB[0] = 1;
		if(RGB[0] <0)
			RGB[0] = 0;
		if(RGB[1] >1)
			RGB[1] = 1;
		if(RGB[1] <0)
			RGB[1] = 0;
		if(RGB[2] >1)
			RGB[2] = 1;
		if(RGB[2] <0)
			RGB[2] = 0;
	 
	 */
		return Color(RGB[0],RGB[1],RGB[2],0);
		
		
    }
private:
    Volume<float> *pyro;
    float Temp;

	
	
	
};
	
	
	
}   
    
#endif
