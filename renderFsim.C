//*******************************************************************
//
//   render.C
//
// contains main
//
//*******************************************************************
//may have to fix b/c of include/ subdir now
#include "Volume.h"
#include "ImplicitSurfaces.h"
#include "CmdLineFind.h"
#include "Image.h"
#include "ImageFiles.h"
#include "ProgressMeter.h"
#include "Camera.h"
#include "ColorFields.h"
#include "Density.h"
#include "OIIOFiles.h"
#include <stdio.h>
#include <iostream>
#include "BaseGrid.h"
#include "GridVolume.h"
#include "CGridVolume.h"
#include "FullGrid.h"
#include "SparseGrid.h"
#include "pyroclast.h"
#include "DSM.h"
#include "Particle.h"
#include "wisp.h"
#include "SDF.h"
#include "Cumulo.h"
#include "StampNoise.h"
#include "MissileTrail.h"
#include "PoissonSolvers.h"


using namespace lux;
using namespace std;

void buildEggRobot(Volume<float> **vol, Volume<Color> **Cd);
int main(int argc, char** argv){
	CmdLineFind clf( argc, argv );
    int imageWidth  = clf.find( "-NX", 1920, "Image width");
    int imageHeight = clf.find( "-NY", 1080, "Image height");
    double ds = clf.find( "-ds",1.0f, "Size of ray march step" );
    string fname = clf.find("-fname","","Name of output file");
    float brightness = clf.find( "-brightness", 1.0f, "Scale brightness of image" );
    float gamma = clf.find( "-gamma", 1.0f, "Gamma of image" );
	int frame =  clf.find( "-frame", 1, "Frame number" );
	
  //  float k = clf.find("-k",1.0f,"Scattering Coefficient");
	
	
	vector<float> cPos;
	cPos.push_back(0.0);
	cPos.push_back(0.0);
	cPos.push_back(10);
	
	cPos = clf.findArray( "-camera", cPos, "Position of the camera");
	
	
	Vector cam( cPos[0], cPos[1], cPos[2] );
	
	Vector dir = -1 * cam;
//	Vector dir = -1 * Vector(0,0,1);
	dir.normalize();
	
	
	vector<float> Dir;
	Dir.push_back(dir[0]);
	Dir.push_back(dir[1]);
	Dir.push_back(dir[2]);

	Dir = clf.findArray( "-dir", Dir, "look direction");
	
	dir = Vector(Dir[0], Dir[1], Dir[2] );
	dir.normalize();
	
	  
        
    Image image;
    image.reset(imageWidth,imageHeight);
    
    Camera camera;
    camera.setEyeViewUp( cam, dir, Vector(0,1,0) );
	camera.setNearPlane(6);
    camera.setFarPlane(14);
    Vector X;
    Vector Xc = camera.eye();
    Vector nHat;
    double sMin = camera.nearPlane();
    double sMax = camera.farPlane();
    Color L;
    double T,dT;
    double alpha;
    double S;



	Volume<float> * Pe,*Ps;
	Volume<Color> * Ce,*Cs;
	
	/*Volume<Vector> *grd = new randVectField();
	BaseGrid<Vector> *Ugrid = new FullGrid<Vector>();
	Ugrid->init(Vector(-5,-5,-5),200,200,200,Vector(5,5,5),Vector(0,0,0));
	Ugrid->sample(grd);
	FFTDivFree(Ugrid);
	Volume<Vector> *U = new GridVolume<Vector>(Ugrid);
	*/
	
	
	Noise_t param1;
	//	part.nbWisps() = 1000000;
	//	part.pscale() = 2.5;
	param1.roughness   = 0.9;
	param1.octaves   = 3; 
	param1.fjump     = 2;
//	param1.wavelength      = 5;
	param1.translate = Vector(1,1,1);
	
	Noise_t param2;
	param2.octaves   = 2; 
	param2.fjump     = 1;
	param2.wavelength      = 2;
	param2.translate = Vector(-2,1,-1);	
	
	Noise_t param3;
	param3.octaves   = 2; 
	param3.fjump     = 2;
	param3.wavelength      = 2;
	param3.translate = Vector(4,-2,1);	
	
	FractalSum<PerlinNoise> *fs1 = new FractalSum<PerlinNoise>();
	fs1->setParameters(param1);

	FractalSum<PerlinNoise> *fs2 = new FractalSum<PerlinNoise>();
	fs2->setParameters(param2);

	
	FractalSum<PerlinNoise> *fs3 = new FractalSum<PerlinNoise>();
	fs3->setParameters(param3);
	Volume<Vector> *U = new FractalPNVField(fs1,fs2,fs3);
	U = new Curl(U);
//	BaseGrid<Vector> *Ugrid = new FullGrid<Vector>();
//	Ugrid->init(Vector(-5,-5,-5),100,100,100,Vector(5,5,5),Vector(0,0,0));
//	Ugrid->sample(U);
//	U = new GridVolume<Vector>(Ugrid);
	
	 
	 
	 
	
	float time = clf.find( "-time", 1.0f, "time of advect" );
	
	Ps = new Advect<float>(new ImplicitSphere(2),U,time);
	//Ps = new ImplicitSphere(2);
	/*
	
	BaseGrid<Vector> *Ugrid = new FullGrid<Vector>();
	Ugrid->init(Vector(-2,-2,-2),80,400,80,Vector(2,18,2),Vector(0,0,0));
	Volume<Vector> *U = new GridVolume<Vector>(Ugrid);

	BaseGrid<float> *Dgrid = new SparseGrid<float>();
	Dgrid->init(Vector(-2,-2,-2),80,400,80,Vector(2,18,2),0.0);
	
	
	Ps = new FluidSphere(1,Vector(0,0,0));
	Ps = new ClampVolume(Ps,0,1);
	
	Vector Gravity(0,-.5,0);
	float dt = clf.find( "-dt", .0025f, "timestep of fluid sim" );

	for(int ndx  = 1;ndx < frame;ndx++){
		Volume<float> *hold = new multFields<float>(Ps,new emptyField<float>(dt));
		Volume<Vector>*Vhold = new scaledVField(hold,-Gravity);
		U = new addFields<Vector>(U,Vhold);
		U = new Advect<Vector>(U,U,dt);
		Ugrid->sample(U);
		FFTDivFree(Ugrid);
		U = new GridVolume<Vector>(Ugrid);
		Ps = new Advect<float>(Ps,U,dt);
		Dgrid->sample(Ps);
		Ps = new GridVolume<float>(Dgrid);
	}
	*/
	
	Ps = new ClampVolume(Ps,0,1);

	Pe = new emptyField<float>(0.0);
	
	Cs = new SolidColorField(Color(.75,.75,.75,1));//CGridVolume(cgrid);
	Ce = new emptyField<Color>(Color(0.0,0.0,0.0,0.0));
	Volume<float> *k = new kField(1);
	Volume<float> *holdout = new emptyField<float>(0.0);
	
	
	DSMgroup DSMs(k);
	BaseGrid<float> *keyDgrid = new SparseGrid<float>();
	keyDgrid->init(Vector(-5,-5,-5),100,100,100,Vector(5,5,5),0.0);
	DSM *key = new DSM(keyDgrid,Ps,ds);
	key->sample(new Light(Vector(10,10,0),Color(1,1,1,1)));
	DSMs.add(key);
	BaseGrid<float> *fillDgrid = new SparseGrid<float>();
	fillDgrid->init(Vector(-5,-5,-5),100,100,100,Vector(5,5,5),0.0);
	DSM *fill = new DSM(fillDgrid,Ps,ds);
	fill->sample(new Light(Vector(0,-10,0),Color(.75,.75,.75,1)));
	DSMs.add(fill);

	
	
	
	clf.usage("-h");
	clf.printFinds();  
	ProgressMeter meter(imageWidth,"render");
	float Pscatter,Pemmit, P;
	for(int i = 0;i < imageWidth; i ++){
        for(int j = 0; j < imageHeight; j++){
            
            nHat = camera.view((double)(i)/(imageWidth-1),(double)(j)/(imageHeight-1));
            X = Xc + nHat * sMin;
            L.set(0.0,0.0,0.0,0.0);
            T = 1;
            S = sMin;
            //raymarch
            while(S < sMax && T >1.0e-6){
                X += nHat * ds;
				if(holdout->eval(X) > 0)
					break;
				Pscatter = Ps->eval(X);
				Pemmit   = Pe->eval(X);
				P = Pscatter + Pemmit;
				

			
				dT = exp (-1.0 * k->eval(X) * ds * P);
				if(P>0){
					L += T * (1-dT) * (Pemmit * Ce->eval(X) + Pscatter * Cs->eval(X) * DSMs.illuminate(X))/P ; 

				}
				//L += Cd->eval(X) * T *(1-dT);
				T *= dT;
				S += ds;
			//	printf("HERE %d\n",__LINE__);
            }
            //after march
            alpha = 1-T;
			
		
			L += Color(0.0,0.0,0.0,alpha);
            image.set(i,j,L);
			
        }
        meter.update();
    }

#ifndef MAGICK
    writeOIIOImage(fname.c_str(),image,brightness,gamma);
#else
    writeMagickImage( clf, image );
#endif
    printf("\a");
    return 0;
    
}


void buildEggRobot(Volume<float> **vol, Volume<Color> **Cd){
	
	Volume<float> *larmb = new ImplicitEllipse(2.2,0.85,Vector(1.0,0.0,0.0));
	Volume<float> *larmr = new RotateScalarField(larmb,Vector(0.0,0.0,1.0),M_PI*5.0/16);
	Volume<float> *luarm  = new TranslateScalarField(larmr,Vector(-3.0,-0.25,0));
	
	
	
	Volume<float> *lfarmb = new ImplicitEllipse(1.5,0.75,Vector(1.0,0.0,0.0));
	Volume<float> *drillb = new ImplicitCone(Vector(1,0,0),3,M_PI/7);
	Volume<float> *drillt = new TranslateScalarField(drillb,Vector(-4.0,0,0));
	Volume<Color> *Cdrill = new ShapedColorField(drillt,Color(1,215/256.0,0,0));
	Volume<Color> *Clfarm = new ShapedColorField(lfarmb,Color(100/256.0,100/256.0,100/256.0,0));
	Volume<float> *lfarmdrb= new UnionVolume(lfarmb,drillt);
	Clfarm = new UnionColor(Cdrill,Clfarm);
	Volume<float> *lfarmr = new RotateScalarField(lfarmdrb,Vector(0.0,0.0,-1.0),M_PI*4.0/16);
	Clfarm = new RotateColorField(Clfarm,Vector(0.0,0.0,-1.0),M_PI*4.0/16);
	Volume<float> *lfarm  = new TranslateScalarField(lfarmr,Vector(-5,-0.75,0.0));
	Clfarm = new TranslateColorField(Clfarm,Vector(-5,-0.75,0.0));
	
	
	Volume<float> *larm  = new UnionVolume(luarm,lfarm);
    Volume<Color> *Clarm = new ShapedColorField(luarm,Color(100/256.0,100/256.0,100/256.0,0));
	Clarm = new UnionColor(Clarm,Clfarm);
	
	
	
	Volume<float> *rarmb = new ImplicitEllipse(2.5,1.25,Vector(1.0,0.0,0.0));
	Volume<float> *rarmr = new RotateScalarField(rarmb,Vector(0.0,0.0,-1.0),M_PI*4.0/16);
	Volume<float> *ruarm  = new TranslateScalarField(rarmr,Vector(3.4,0,0));
	
	
	Volume<float> *rfarmb = new ImplicitEllipse(2,1,Vector(0.0,0.0,1.0));
	Volume<float> *rfarm  = new TranslateScalarField(rfarmb,Vector(5,-2.5,1.75));
	
	Volume<float> *rarms = new BlendVolume(ruarm,rfarm,1.25);
	
	Volume<Color> *Crarm = new ShapedColorField(rarms,Color(100/256.0,100/256.0,100/256.0,0));
	
	
	
	
	
	
	Volume<float> *rhandb = new ImplicitIcosahedron();
	Volume<float> *rhands = new ScaleScalarField(rhandb,0.3);
	
	Volume<float> *rhandsc = new ImplicitPlane(Vector(0,1,0),Vector(0,-0.25,0));
	Volume<float> *rhand   = new IntersectVolume(rhandsc,rhands);
	Volume<float> *rhandr  = new RotateScalarField(rhand,Vector(-1,0,0),M_PI/2);
	Volume<float> *rhand1  = new TranslateScalarField(rhandr,Vector(5,-2.5,4.25));
	Volume<Color> *Chand = new ShapedColorField(rhand1,Color(1,215/256.0,0,0));
	
	
	
	
	
	/*face*/
	Volume<float> *leyeb = new ImplicitStiener();
	Volume<float> *leye  = new TranslateScalarField(leyeb,Vector(-0.6,2,1.0));
	
	Volume<float> *reyeb = new ImplicitStiener();
	Volume<float> *reyer = new RotateScalarField(reyeb,Vector(0,0,-1),M_PI/2);
	Volume<float> *reye  = new TranslateScalarField(reyer,Vector(0.6,2,1.0));
	
	Volume<float> *eyes = new UnionVolume(reye,leye);
	Volume<float> *mouthb= new ImplicitTorus(.25,.1,Vector(0,0,1));
	Volume<float> *mouth  = new TranslateScalarField(mouthb,Vector(0,1.25,1.0));
	Volume<float> *face = new UnionVolume(eyes,mouth);
	Volume<Color> *Cface = new ShapedColorField(face,Color(50,0,0,1));
	
	
	
	Volume<float> *rarm = new UnionVolume(rhand1,rarms);
	
	Volume<float> *arms  = new UnionVolume(larm,rarm);
	
	
	Volume<float> *headb = new ImplicitInfCylinder(Vector(0,0,-1),1.25);
	Volume<float> *head  = new TranslateScalarField(headb,Vector(0.0,1.8,2.25));
	Volume<float> *body = new ImplicitEllipse(4,2,Vector(0,1,0));
	Volume<float> *hb  = new CutoutVolume(body,head);//,0.95);
	Volume<Color> *Cbody = new ShapedColorField(hb,Color(220.0/256,220.0/256,220.0/256,0.0));
	Volume<float> *bodyFace = new UnionVolume(hb,face);
	
	Volume<float> *UpperBody = new UnionVolume(bodyFace,arms);
	
	
	Volume<float> *ulegb = new ImplicitBox(2,4);
	Volume<float> *lpl1 = new ImplicitPlane(Vector(1,0,0),Vector(-0.75,0,0));
	Volume<float> *lpl2 = new ImplicitPlane(Vector(-1,0,0),Vector(0.75,0,0));
	Volume<float> *lpl3 = new ImplicitPlane(Vector(0,0,1),Vector(0,0,-0.75));
	Volume<float> *lpl4 = new ImplicitPlane(Vector(0,0,-1),Vector(0,0,0.75));
	
	Volume<float> *leg1 = new IntersectVolume(ulegb,lpl1);
	Volume<float> *leg2 = new IntersectVolume(leg1,lpl2);
	Volume<float> *leg3 = new IntersectVolume(leg2,lpl3);
	Volume<float> *leg4 = new IntersectVolume(leg3,lpl4);
	
	Volume<float> *knee  = new ImplicitSphere(1);
	Volume<float> *mknee = new TranslateScalarField(knee,Vector(0,-1.75,0));
	
	
	
	Volume<float> *uLegb = new UnionVolume(leg4,mknee);
	
	
	Volume<float> *uLegR = new RotateScalarField(uLegb,Vector(0,0,-1),M_PI/4);
	Volume<float> *uLegT = new TranslateScalarField(uLegR,Vector(-2.3,-4.7,0));
	Volume<float> *lLegT = new TranslateScalarField(leg4,Vector(-3.5,-7.6,0));
	Volume<float> *leftLeg = new UnionVolume(uLegT,lLegT);
	
	Volume<float> *uRLegR = new RotateScalarField(uLegb,Vector(0,0,1),M_PI/4);
	Volume<float> *uRLegT = new TranslateScalarField(uRLegR,Vector(2.3,-4.7,0));
	Volume<float> *RLegT = new TranslateScalarField(leg4,Vector(3.5,-7.6,0));
	Volume<float> *rightLeg = new UnionVolume(uRLegT,RLegT);
	
	Volume<float> *Legs = new UnionVolume(leftLeg,rightLeg);
	Volume<Color> *Clegs = new ShapedColorField(Legs,Color(100/256.0,100/256.0,100/256.0,0));
	
	Volume<float> *shape = new UnionVolume(Legs,UpperBody);
	shape = new TranslateScalarField(shape,Vector(0,2,0));
	
	
	*vol  = new ClampVolume(shape,0,1);
	*Cd = new UnionColor(Clarm,Crarm);
	*Cd = new UnionColor(*Cd,Chand);
	*Cd = new UnionColor(*Cd,Cface);
	*Cd = new UnionColor(*Cd,Cbody);
	*Cd = new UnionColor(*Cd,Clegs);
	*Cd = new TranslateColorField(*Cd,Vector(0,2,0));
	
}

