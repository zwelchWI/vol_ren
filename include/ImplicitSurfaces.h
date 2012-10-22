//*******************************************************************
//
//   ImplicitSurfaces.h
//
// Implicit Surfaces
//
//*******************************************************************

#ifndef IMPSURFACE
#define IMPSURFACE

#include "Volume.h"
#include <cmath>
using namespace std;
namespace lux{
    
class ImplicitSphere : public Volume<float>{
public:
    ImplicitSphere(const float R):radius(R){};
    ~ImplicitSphere(){}
    const float eval(const Vector &P) const{
        return radius - P.magnitude();
    }
private:
    float radius;
};
	
class FluidSphere : public Volume<float>{
public:
	FluidSphere(const float R, Vector X_,float factor_ = .1):radius(R),X(X_),factor(factor_){};
	~FluidSphere(){}
	const float eval(const Vector &P) const{
		return (radius - (X - P).magnitude() * (X - P).magnitude())/factor;
	}
private:
	float radius,factor;
	Vector X;
};

class ImplicitTorus : public Volume<float>{
public:
    ImplicitTorus(const float RM,const float Rm,const Vector N):Rmaj(RM),Rmin(Rm),normal(N){};
    ~ImplicitTorus(){}
    const float eval(const Vector &X) const{
        return 4 * Rmaj * Rmaj * (X - (X * normal) * normal).magnitude() * (X - (X * normal) * normal).magnitude() - (X.magnitude() * X.magnitude() + Rmaj * Rmaj - Rmin * Rmin) * (X.magnitude() * X.magnitude() + Rmaj * Rmaj - Rmin * Rmin);
    }
private:
    float Rmaj,Rmin;
    Vector normal;
};
    
class ImplicitCone : public Volume<float>{
public:
    ImplicitCone(const Vector N,const float H,const float t):
        n(N),h(H),theta(t){};
    const float eval(const Vector &X) const{
        if(((X) * n) > h)
            return h - (X) * n;
        else if((X * n) < 0.0)
            return (X) * n;
        else
            return X*n - X.magnitude() * cos(theta);
    }
private:
    Vector n;
    float h,theta;
    
};

    
class ImplicitPlane : public Volume<float>{
public:
    ImplicitPlane(const Vector N,const Vector X0):normal(N),x0(X0){};
    ~ImplicitPlane(){}
    const float eval(const Vector &X) const{
        return (X - x0) * normal;
    }
private:
    Vector normal, x0;
};
     
    
    
    
class ImplicitBox: public Volume<float>{
public:
    ImplicitBox(const double Rad,const double exp):R(Rad),p(exp){};
    const float eval(const Vector &X) const{
        return std::pow(R,2.0*p) - std::pow(X[0],2.0*p) - std::pow(X[1],2.0*p) - std::pow(X[2],2.0*p);
    }
private:
    const double R;
    const double p;

};

    

class ImplicitIcosahedron : public Volume<float>{
public:
    ImplicitIcosahedron(){};
    const float eval(const Vector&X) const{
        if(X.magnitude() > 1.8 * M_PI)
            return -1.8 * M_PI;
        double T = 1.61803399;
        return cos(X[0] + T*X[1]) + cos(X[0] - T*X[1]) + cos(X[1] + T*X[2])+
        cos(X[1] - T*X[2]) + cos(X[2] + T*X[0]) + cos(X[2] - T*X[0]) -2.0;
    }
};
    
    
class ImplicitStiener : public Volume<float>{
public:
    ImplicitStiener(){};
    const float eval(const Vector&X) const{
        return -1.0 * (X[0]*X[0]*X[1]*X[1] + X[0]*X[0]*X[2]*X[2]
                       + X[1]*X[1]*X[2]*X[2] - X[0]*X[1]*X[2]);
    }
};
    
class ImplicitEllipse : public Volume<float>{
public:
    ImplicitEllipse(const float RM,const float Rm,const Vector Norm):
        Rmaj(RM),Rmin(Rm),n(Norm){};
    const float eval(const Vector&X) const{
        float Z = X * n;
        Vector Xp = X - (Z*n);
        return 1.0 - Z*Z/(Rmaj*Rmaj) - Xp.magnitude()*Xp.magnitude()/(Rmin*Rmin);
    }
private:
    float Rmaj,Rmin;
    Vector n;
};
    
    
class ImplicitInfCylinder : public Volume<float>{
public:
    ImplicitInfCylinder(Vector N,float r):n(N),R(r){};
    const float eval(const Vector&X) const{
        Vector Xp = X - (X * n) * n;
        return R - Xp.magnitude();
    }
private:
    Vector n;
    float R;
};   
    
class ImplicitCylinder : public Volume<float>{
public:
    ImplicitCylinder(Vector N,float r, float h){
        cyl = new ImplicitInfCylinder(N,r);
        p1 = new ImplicitPlane(N * h/2,-1 *N);
        p2 = new ImplicitPlane(N * h/-2,N);
        hold = new IntersectVolume(cyl,p1);
        v = new IntersectVolume(hold,p2);
    }
    const float eval(const Vector&X) const{
        return v->eval(X);
    }
private:
    Volume<float> *cyl;
    Volume<float> *p1,*p2;
    Volume<float> *hold,*v;
    
};

}   
    
#endif
