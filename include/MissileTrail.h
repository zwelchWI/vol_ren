//*******************************************************************
//
//   MissileTrail.h
//
// Missile Trail class
//
//*******************************************************************

#ifndef BOOMBOOM
#define BOOMBOOM

#include "Volume.h"
#include <cmath>
#include "pyroclast.h"
#include "StampNoise.h"
#include "wisp.h"
#include "Particle.h"
#include <vector>
using namespace std;
namespace lux{
    
class PyroTrail : public Volume<float>{
public:
	PyroTrail(vector<Vector> &pts,int startFrame,int endFrame,int currFrame,Particle base,int numObjs){
		if(pts.size() < 2){
			fprintf(stderr,"NOT ENOUGH GUIDE POINTS");
			return;
		}
		if(currFrame < startFrame)
			return;
		float age  = currFrame - startFrame;
		float dAge = (endFrame - startFrame)/((float)numObjs);
		Particle *part;
		int ndx = 0;
		for(int pointndx = 0; pointndx < pts.size()-1;pointndx++){
			Vector start = pts[pointndx];
			Vector diff  = pts[pointndx+1] - pts[pointndx];
			float step = diff.magnitude()/(((float)numObjs)/(pts.size()-1));
			diff.normalize();
			Vector point = start;
			for(int ndx = pointndx *numObjs/(pts.size()-1) ;ndx <= (pointndx + 1)*numObjs/(pts.size()-1);ndx++){
			//while(point!=pts[pointndx+1]){	
				
				//check to see if its visible
			//	if(age > 0 && age < base.lifetime()){
					//alive
					part = new Particle(base);
					part->P()   = point;
		//		point.print();
					part->translate() = point;
					part->age() = age;
					if(age < 1)
						part->opacity() *= age;
					if(base.lifetime() - age < 1)
						part->opacity() *= .25;
					else if(base.lifetime() - age < 2)
						part->opacity() *= .5;
					else if(base.lifetime() - age < 3)
						part->opacity() *= .75;
					for(int i = 0;i < age;i++){
						if(part->octaves() > 1.5)
							part->octaves() -= .1;
						part->pyroGamma() -= .5/part->lifetime();
					}
					pyros.push_back(Pyroclast(part));
					
					
			//	}
				age -= dAge;
				point +=  step * diff;
			}
		}
	}
	const float eval(const Vector &P) const{
		if(pyros.size() == 0)
			return 0;
		float max = pyros[0].eval(P);
		for(int ndx = 1;ndx < pyros.size();ndx++){
			max = ::max(max,pyros[ndx].eval(P));
		}
		return max;
	
    }
	void stamp(BaseGrid<float> *Dens,BaseGrid<float> *ageGrid = NULL){
		for(int ndx = 0;ndx < pyros.size();ndx++){
			
		if(pyros[ndx].age() > 0 && pyros[ndx].age() <= pyros[ndx].lifetime()){
				pyros[ndx].stamp(Dens,ageGrid);
			}
		}
	}
	

private:				 
	vector<Pyroclast> pyros;
};

class NoiseTrail{
public:
	NoiseTrail(vector<Vector> &pts,int startFrame,int endFrame,int currFrame,Particle base,int numObjs){
		if(pts.size() < 2){
			fprintf(stderr,"NOT ENOUGH GUIDE POINTS");
			return;
		}
		if(currFrame < startFrame)
			return;
		float age  = currFrame - startFrame;
		float dAge = (endFrame - startFrame)/((float)numObjs);
		Particle *part;
		
		for(int pointndx = 0; pointndx < pts.size()-1;pointndx++){
			Vector start = pts[pointndx];
			Vector diff  = pts[pointndx+1] - pts[pointndx];
			float step = diff.magnitude()/(((float)numObjs)/(pts.size()-1));
			diff.normalize();
			Vector point = start;
			for(int ndx = pointndx *numObjs/(pts.size()-1) ;ndx <= (pointndx + 1)*numObjs/(pts.size()-1);ndx++){
				point +=  step * diff;
				
				//check to see if its visible
		//		if(age > 0 && age < base.lifetime()){
					//alive
				
				part = new Particle(base);
				part->P()   = point;
				part->translate() = point;
				part->age() = age;
				if(age < 1)
					part->opacity() *= age;
				if(base.lifetime() - age < 1)
					part->opacity() *= .25;
				else if(base.lifetime() - age < 2)
					part->opacity() *= .5;
				else if(base.lifetime() - age < 3)
					part->opacity() *= .75;
				for(int i = 0;i < age;i++){
			//		if(part->octaves() > 1)
			//			part->octaves() -= .1;
					part->fade() += .5/part->lifetime();
				}
				noises.push_back(StampNoise(part));
					
					
				//}
				age -= dAge;
			}
		}
	}
	void stamp(BaseGrid<float> *Dens,BaseGrid<float> *ageGrid = NULL){
		for(int ndx = 0;ndx < noises.size();ndx++){
			if(noises[ndx].age() > 0 && noises[ndx].age() <= noises[ndx].lifetime())
				noises[ndx].stamp(Dens,ageGrid);
		}
	}
	
	
private:
	vector<StampNoise> noises;
};
	
	
	class WispTrail{
	public:
		WispTrail(vector<Vector> &pts,int startFrame,int endFrame,int currFrame,Particle base,int numObjs){
			if(pts.size() < 2){
				fprintf(stderr,"NOT ENOUGH GUIDE POINTS");
				return;
			}
			if(currFrame < startFrame)
				return;
			
			float age  = currFrame - startFrame;
			float dAge = (endFrame - startFrame)/((float)numObjs);
			Particle *part;

			for(int pointndx = 0; pointndx < pts.size()-1;pointndx++){
				Vector start = pts[pointndx];
				Vector diff  = pts[pointndx+1] - pts[pointndx];
				float step = diff.magnitude()/(((float)numObjs)/(pts.size()-1));
				diff.normalize();
				Vector point = start;
			 for(int ndx = pointndx *numObjs/(pts.size()-1) ;ndx <= (pointndx + 1)*numObjs/(pts.size()-1);ndx++){
				 point +=  step * diff;
				 
				 //check to see if its visible
	//			 if(age > 0 && age < base.lifetime()){
					 //alive
					 part = new Particle(base);
					 part->P()   = point;
					 part->translate() = point;
					 part->age() = age;
					 part->id() = ndx;
					 if(age < 1)
						 part->opacity() *= age;
					 if(base.lifetime() - age < 1)
						 part->opacity() *= .25;
					 else if(base.lifetime() - age < 2)
						 part->opacity() *= .5;
					 else if(base.lifetime() - age < 3)
						 part->opacity() *= .75;
					 for(int i = 0;i < age;i++){
						 if(part->octaves() > 1.5)
							 part->octaves() -= .1;
						 part->wispGamma() -= 1.0/(6 * part->lifetime());
					 }

					 wisps.push_back(Wisp(part));
					 
					 
		//		 }
				 age -= dAge;
			 }
			 }
		}
		void stamp(BaseGrid<float> *Dens,BaseGrid<float> *ageGrid = NULL){
			for(int ndx = 0;ndx < wisps.size();ndx++){
					
				if(wisps[ndx].age() > 0 && wisps[ndx].age() <= wisps[ndx].lifetime())
					wisps[ndx].stamp(Dens,ageGrid);
			}
		}
		
		
	private:
		vector<Wisp> wisps;
	};

	
	
}   
    
#endif
