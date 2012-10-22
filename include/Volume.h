//*******************************************************************
//
//   Volume.h
//
// Base class for a field
//
//*******************************************************************

#ifndef VOLUME
#define VOLUME


#include "Vector.h"
#include "Matrix.h"
#include "Color.h"
#include "LinearAlgebra.h"
#include "Noise.h"
#include "PerlinNoise.h"
#include <algorithm>
#include "UniformPRN.h"


#include <stdio.h>

using namespace std;
namespace lux{
    
template <typename U>
struct GradType{
    typedef int GType;
};

template<>
struct GradType<float>{
    typedef Vector GType;
};

template<>
struct GradType<Vector>{
    typedef Matrix GType;
};    

    
template <typename U>
class Volume{
public:
    Volume(){}
    ~Volume(){}
    
    typedef U volumeDataType;
	typedef typename GradType<U>::GType volumeGradType;
    
    //returns value of field at every point in space
    virtual const volumeDataType eval(const Vector &P) const {
		fprintf(stderr,"CALL TO BASE FN EVAL\n");exit(0);
        volumeDataType base; return base;
    } 
    virtual const volumeGradType grad(const Vector & P) const{
        volumeGradType base; return base;
    } 
        
};

template <typename U>
class emptyField : public Volume<U>{
public:
	emptyField(U K):k(K){};
	const U eval(const Vector &P) const {
		return k;
		
	}
private:
	U k;
};

    
class kField : public Volume<float>{
public:
	kField(float K,Volume<float> *v = NULL):k(K),elem(v){};
	const float eval(const Vector &P) const {
		if(elem == NULL)
			return k;
		if (elem->eval(P) > 0)
			return k;
		return 0;
		
		}
private:
	float k;
	Volume<float> *elem;
};

	

class TranslateScalarField : public Volume<float>{
public:
    TranslateScalarField(Volume<float> *v,Vector x0):elem(v),x(x0){};
    const float eval(const Vector &P) const {return elem->eval(P - x);}
    const Vector grad(const Vector &P)const {return elem->grad(P - x);}
private:
    Volume<float> *elem;
    Vector x;
};


    
	
    
class TranslateColorField : public Volume<Color>{
public:
    TranslateColorField(Volume<Color> *v,Vector x0):elem(v),x(x0){};
    const Color eval(const Vector &P) const {return elem->eval(P - x);}
    const int grad(const Vector &P)const {return elem->grad(P - x);}
private:
    Volume<Color> *elem;
    Vector x;
};
    
    
class TranslateVectorField : public Volume<Vector>{
public:
    TranslateVectorField(Volume<Vector> *v,Vector x0):elem(v),x(x0){};
    const Vector eval(const Vector &P) const {return elem->eval(P - x);}
    const Matrix grad(const Vector &P)const {return elem->grad(P - x);}
private:
    Volume<Vector> *elem;
    Vector x;
};

    
    
    
class TranslateMatrixField : public Volume<Matrix>{
public:
    TranslateMatrixField(Volume<Matrix> *v,Vector x0):elem(v),x(x0){};
    const Matrix eval(const Vector &P) const {return elem->eval(P - x);}
    const int    grad(const Vector &P) const {return elem->grad(P - x);}
private:
    Volume<Matrix> *elem;
    Vector x;
};
    
    
    
class ScaleScalarField : public Volume<float>{
public:
    ScaleScalarField(Volume<float> *V,float t): elem(V),scale(t){};
    const float eval(const Vector &P) const {return elem->eval(P/scale);}
    const Vector grad(const Vector &P)const {return elem->grad(P/scale);} 
private:
    Volume<float> *elem;
    float scale;
};

    
    
    
class ScaleColorField : public Volume<Color>{
public:
    ScaleColorField(Volume<Color> *V,float t): elem(V),scale(t){};
    const Color eval(const Vector &P) const {return elem->eval(P/scale);}
    const int grad(const Vector &P)const {return elem->grad(P/scale);} 
private:
    Volume<Color> *elem;
    float scale;
};

    
    
    
class ScaleVectorField : public Volume<Vector>{
public:
    ScaleVectorField(Volume<Vector> *V,float t): elem(V),scale(t){};
    const Vector eval(const Vector &P) const {return scale * elem->eval(P/scale);}
    const Matrix grad(const Vector &P) const {return scale * elem->grad(P/scale);} 
private:
    Volume<Vector> *elem;
    float scale;
};
    
    
    
    
class ScaleMatrixField : public Volume<Matrix>{
public:
    ScaleMatrixField(Volume<Matrix> *V,float t): elem(V),scale(t){};
    const Matrix eval(const Vector &P) const {return scale * scale * elem->eval(P/scale);}
    const int    grad(const Vector &P) const {return scale * scale * elem->grad(P/scale);} 
private:
    Volume<Matrix> *elem;
    float scale;
};
    
    
  
class RotateScalarField : public Volume<float>{
public:
    RotateScalarField(Volume<float> *V,Matrix r): elem(V),matInv(r.inverse()){};
    RotateScalarField(Volume<float> *V,const Vector& axis, const double angle ):
    elem(V),matInv(rotation(axis,angle).inverse()){};
    const float eval(const Vector &P) const {return elem->eval(matInv * P);}
    const Vector grad(const Vector &P)const {return elem->grad(matInv * P);} 
private:
    Volume<float> *elem;
    Matrix matInv;
};    

    
    
    
class RotateColorField : public Volume<Color>{
public:
    RotateColorField(Volume<Color> *V,Matrix r): elem(V),matInv(r.inverse()){};
    RotateColorField(Volume<Color> *V,const Vector& axis, const double angle ):
    elem(V),matInv(rotation(axis,angle).inverse()){};
    const Color eval(const Vector &P) const {return elem->eval(matInv * P);}
    const int grad(const Vector &P)const {return elem->grad(matInv * P);} 
private:
    Volume<Color> *elem;
    Matrix matInv;
};    
  
    
    
    
class RotateVectorField : public Volume<Vector>{
public:
    RotateVectorField(Volume<Vector> *V,Matrix r): elem(V),mat(r){};
    RotateVectorField(Volume<Vector> *V,const Vector& axis, const double angle ):
    elem(V),mat(rotation(axis,angle)){};
    const Vector eval(const Vector &P) const {return mat * elem->eval(mat.inverse() * P);}
    const Matrix grad(const Vector &P)const {return mat * (elem->grad(mat.inverse() * P));} 
private:
    Volume<Vector> *elem;
    Matrix mat;
};    

    
    
class RotateMatrixField : public Volume<Matrix>{
public:
    RotateMatrixField(Volume<Matrix> *V,Matrix r): elem(V),mat(r){};
    RotateMatrixField(Volume<Matrix> *V,const Vector& axis, const double angle ):
    elem(V),mat(rotation(axis,angle)){};
    const Matrix eval(const Vector &P) const {return mat.transpose() * mat * elem->eval(mat.inverse() * P);}
    const int grad(const Vector &P)const {return 0;} 
private:
    Volume<Matrix> *elem;
    const Matrix mat;
};    

class UnionVolume : public Volume<float>{
public:
    UnionVolume(Volume<float> *f_,Volume<float> *g_):f(f_),g(g_){};
    const float eval(const Vector &P) const {
        return max(f->eval(P),g->eval(P));
    }
private:
    Volume<float> *f;
    Volume<float> *g;
};

    
class IntersectVolume : public Volume<float>{
public:
    IntersectVolume(Volume<float> *f_,Volume<float> *g_):f(f_),g(g_){};
    const float eval(const Vector &P) const {
        return min(f->eval(P),g->eval(P));
    }
private:
    Volume<float> *f;
    Volume<float> *g;
};    
    


class CutoutVolume : public Volume<float>{
public:
    CutoutVolume(Volume<float> *f_,Volume<float> *g_):f(f_),g(g_){};
    const float eval(const Vector &P) const {
        const float gVal = -1.0 * g->eval(P);
        return min(f->eval(P),gVal);
    }
private:
    Volume<float> *f;
    Volume<float> *g;
};    

class BlendVolume : public Volume<float>{
public:
    BlendVolume(Volume<float> *f_,
				Volume<float> *g_,
				float beta = 1,
				float ft = 1,
				float gt = 1):
	f(f_),
	g(g_),
	Beta(beta),
	fT(ft),
	gT(gt){};
    const float eval(const Vector &P) const {
        return std::exp(f->eval(P)/fT) + std::exp(g->eval(P)/gT) - Beta;
    }
private:
    Volume<float> *f;
    Volume<float> *g;
	float Beta;
	float fT;
	float gT;
};    

class MaskVolume : public Volume<float>{
public:
    MaskVolume(Volume<float> *f_):f(f_){};
    const float eval(const Vector &P) const {
        if(f->eval(P) <= 0.0)
            return 0.0;
        return 1.0;
    }
private:
    Volume<float> *f;
};
    
class ClampVolume : public Volume<float>{
public:
    ClampVolume(Volume<float> *f_,float f0_,float f1_):f(f_),f0(f0_),f1(f1_){};
    const float eval(const Vector &P) const {
        if(f->eval(P) <= f0)
            return f0;
        else if(f->eval(P) >= f1)
            return f1;
        else
            return f->eval(P);
    }
private:
    Volume<float> *f;
    float f0,f1;
};


class Gradient : public Volume<Vector>{
public:
	Gradient(Volume<float> *base_,float ds_ = 0.01):base(base_),ds(ds_){};
	const Vector eval(const Vector&P) const{
		return Vector((base->eval(P) - base->eval(P - Vector(ds,0,0)))/ds,
			   (base->eval(P) - base->eval(P - Vector(0,ds,0)))/ds,
			   (base->eval(P) - base->eval(P - Vector(0,0,ds)))/ds);
	}
private:
	Volume<float> *base;
	float ds;
};
	
class Curl : public Volume<Vector>{
public:
	Curl(Volume<Vector> *base_,float ds_ = 0.01):base(base_),ds(ds_){};
	const Vector eval(const Vector &P) const {
		
		float dyfz = ((base->eval(P) - base->eval(P-Vector(0,ds,0)))[2])/ds;
		float dzfy = ((base->eval(P) - base->eval(P-Vector(0,0,ds)))[1])/ds;
		float dzfx = ((base->eval(P) - base->eval(P-Vector(0,0,ds)))[0])/ds;
		float dxfz = ((base->eval(P) - base->eval(P-Vector(ds,0,0)))[2])/ds;
		float dxfy = ((base->eval(P) - base->eval(P-Vector(ds,0,0)))[1])/ds;
		float dyfx = ((base->eval(P) - base->eval(P-Vector(0,ds,0)))[0])/ds;
		
		
		
		return Vector(dyfz - dzfy,dzfx - dxfz,dxfy - dyfx);	
	}

private:
	Volume<Vector> *base;
	float ds;
};
	
class Divergence : public Volume<float>{
public:
	Divergence(Volume<Vector> *base_,float ds_ = 0.01):base(base_),ds(ds_){};
	const float eval(const Vector &P) const {
		return (base->eval(P) - base->eval(P - Vector(ds,0,0)))[0]/ds + 
		(base->eval(P) - base->eval(P - Vector(0,ds,0)))[1]/ds + 
		(base->eval(P) - base->eval(P - Vector(0,0,ds)))[2]/ds;
	}
		
private:
	Volume<Vector> *base;
	float ds;
};
	
    
class FractalPNVField :public Volume<Vector>{
public:
	FractalPNVField(FractalSum<PerlinNoise> *f1_,FractalSum<PerlinNoise> *f2_,FractalSum<PerlinNoise> *f3_):f1(f1_),f2(f2_),f3(f3_){};
	const Vector eval(const Vector &P) const {
		return(Vector(f1->eval(P),f2->eval(P),f3->eval(P)));
	}
	
private:
	FractalSum<PerlinNoise> *f1,*f2,*f3;
};
	
class randVectField :public Volume<Vector>{
public:
	randVectField(int seed = 0){
		prn = new UniformPRN(seed);
	
	}
	const Vector eval(const Vector &P) const {
		return(Vector(2 * prn->eval() - 1,2 * prn->eval() - 1,2 * prn->eval() - 1));
	}
	
private:
	PRN *prn;
};

	
	
template <typename T>
class Advect : public Volume<T>{
public:
	Advect(Volume<T> *f_,Volume<Vector> *U_,float dt_ = .1):f(f_),U(U_),dt(dt_){};
	const T eval(const Vector &P) const {
		return f->eval(P - U->eval(P) * dt);
	}
	
private:
	Volume<T> *f;
	Volume<Vector>*U;
	float dt;
	
};
	

class Identity : public Volume<Vector>{
public:
	Identity(){};
	const Vector eval(const Vector &P) const{
		return P;
	}
};
	
	
template <typename U>
class multFields : public Volume<U>{
public:
	multFields(Volume<U> *f1_,Volume<U> *f2_):f1(f1_),f2(f2_){};
	const U eval(const Vector &P) const{
		return f1->eval(P) * f2->eval(P);
	}

private:
	Volume<U> *f1,*f2;
	
};

template <typename U>
class addFields : public Volume<U>{
public:
	addFields(Volume<U> *f1_,Volume<U> *f2_):f1(f1_),f2(f2_){};
	const U eval(const Vector &P) const{
		return f1->eval(P) + f2->eval(P);
	}
	
private:
	Volume<U> *f1,*f2;
	
};

	
	
class scaledVField : public Volume<Vector>{
public:
	scaledVField(Volume<float> *f1_,Vector V_):f1(f1_),V(V_){};
	const Vector eval(const Vector &P) const{
		return f1->eval(P) * V;
	}
	
private:
	Volume<float> *f1;
	Vector V;
	
};
	
	
	
	
}

#endif



