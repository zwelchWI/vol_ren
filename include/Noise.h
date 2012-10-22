

//
//
// noise.h
//
// General purpose noise code
//
// Integrates multiple noise sources into a single framework
//
//
//


#ifndef __NOISE_H__
#define __NOISE_H__

#include "Vector.h"

namespace lux
{

struct Noise_t 
{

  Noise_t() :
  wavelength   (1),
  translate    (Vector(0,0,0)),
  octaves      (1.0),
  amplitude    (1),
  offset       (0),
  fjump        (2),
  roughness    (0.5),
  time         (0.0),
  fftLowCutoff (0.01),
  fftHighCutoff (1.0),
  fftPower      (3.5),
  fftNbGridPoints (128),
  fftLength     (10.0),
  lognormalmean (1.0),
  gaussianstandarddeviation (1.0),
  seed         (12345)
  {}

	float wavelength;
	Vector translate;
	float octaves;
	float amplitude;
	float offset;
	float fjump;
        float roughness;
	float time;
	float fftLowCutoff;
	float fftHighCutoff;
	float fftPower;
	int   fftNbGridPoints;
	float fftLength;
	float lognormalmean;
	float gaussianstandarddeviation;
	int   seed;
};


class Noise
{
  public:

    Noise(){}
    virtual ~Noise(){}

    virtual const float eval( const float x ) const { return 0; }
    virtual const float eval( const Vector& x ) const { return 0; }

    virtual void setParameters( const Noise_t& parameters ){}
    virtual void getParameters( Noise_t& parameters ) const {}
};



template< typename BaseNoise>
class FractalSum : public Noise
{
  public:

    FractalSum() : 
       octaves    (3.0), 
       fjump      (2.0), 
       roughness  (0.5),
       wavelength (0.666666),
       translate  (Vector(0,0,0)),
       offset     (0.0)
    {}

   ~FractalSum(){}

    const float eval( const float x ) const
    {
       float exponent = 1;
       float amplitude = 1;
       float accum = 0;
       for( int oc=0;oc<octaves;oc++ )
       {
          const float X = (x - translate[0]) * wavelength * exponent;
          accum += amplitude * _noise.eval( X );
          exponent *= fjump;
	  amplitude *= roughness;
       }
       const float X = (x - translate[0]) * wavelength * exponent;
       float val = amplitude * _noise.eval( X );
       accum += (octaves - (int)octaves) * val;
       return (accum + offset);
    }

    const float eval( const Vector& x ) const
    {
       float exponent = 1;
       float amplitude = 1;
       float accum = 0;
       Vector X = (x - translate)*wavelength*exponent;
       for( int oc=0;oc<octaves;oc++ )
       {
          accum += amplitude * _noise.eval( X );
          X *= fjump;
	  amplitude *= roughness;
       }
       float val = amplitude * _noise.eval( X );
       accum += (octaves - (int)octaves) * val;
       return (accum+offset);
    }

	const Vector evalVec(const Vector &x, float offset = 0.1) const
	{
		return Vector(eval(x),eval(x+Vector(offset,0,0)),eval(x + Vector(0,offset,0))); 
		
	}

    void setParameters( const Noise_t& parameters )
    {
       octaves = parameters.octaves;
       fjump = parameters.fjump;
       roughness = parameters.roughness;
       wavelength = parameters.wavelength; 
       translate = parameters.translate; 
       offset = parameters.offset; 
       _noise.setTime( parameters.time );
    }

   
    void getParameters( Noise_t& parameters ) const
    {
       parameters.octaves = octaves;
       parameters.fjump = fjump;
       parameters.roughness = roughness;
       parameters.wavelength = wavelength; 
       parameters.translate = translate; 
       parameters.offset = offset; 
    }
 


  private:

    BaseNoise _noise;

    float octaves;
    float fjump;
    float roughness;
    float wavelength;
    Vector translate;
    float offset;
    
};



class PRN
{
  public:

    PRN(){}
    virtual ~PRN(){}

    virtual const float eval(){ return 0; }

    virtual void setParameters( const Noise_t& parameters ){}
};


}

#endif



