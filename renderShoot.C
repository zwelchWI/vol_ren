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
#include "RWGrid.h"
#include "SELMA.h"

using namespace lux;
using namespace std;

void buildEggRobot(Volume<float> **vol, Volume<Color> **Cd);
int main(int argc, char** argv){
	CmdLineFind clf( argc, argv );
    int imageWidth  = clf.find( "-NX", 1920, "Image width");
    int imageHeight = clf.find( "-NY", 1080, "Image height");
    double ds = clf.find( "-ds",.1f, "Size of ray march step" );
    string fname = clf.find("-fname","","Name of output file");
    float brightness = clf.find( "-brightness", 1.0f, "Scale brightness of image" );
    float gamma = clf.find( "-gamma", 1.0f, "Gamma of image" );
	int frame =  clf.find( "-frame", 1, "Frame number" );
	
  //  float k = clf.find("-k",1.0f,"Scattering Coefficient");
	vector<float> cPos;
	cPos.push_back(3.0);
	cPos.push_back(-5.0);
	cPos.push_back(-3.5);
	
	cPos = clf.findArray( "-camera", cPos, "Position of the camera");
	
	
	Vector cam( cPos[0], cPos[1], cPos[2] );
	
	Vector dir = -1 * cam;
	dir.normalize();
	
	
	vector<float> Dir;
	Dir.push_back(-.6);
	Dir.push_back(.35);
	Dir.push_back(.5);
	
	Dir = clf.findArray( "-dir", Dir, "look direction");
	
	dir = Vector(Dir[0], Dir[1], Dir[2] );
	dir.normalize();
	
	
	
    Image image;
    image.reset(imageWidth,imageHeight);
    
    Camera camera;
    camera.setEyeViewUp( cam, dir, Vector(0,1,0) );
	camera.setNearPlane(0);
    camera.setFarPlane(15);
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

	Vector trophyPos(0,5,0);
	Color  trophyColor(212/187.5,175/187.5,55/187.5,0.0);
	
	Volume<float> *eggy;
	Volume<Color> *eggColor;
	buildEggRobot(&eggy,&eggColor);
	
	eggy =  new ScaleScalarField(eggy,.3);
	eggy = new RotateScalarField(eggy,Vector(1,0,0),M_PI/2);
	eggy = new RotateScalarField(eggy,Vector(0,0,1),M_PI/2);
	eggy = new RotateScalarField(eggy,Vector(-1,0,0),M_PI/2);    
	eggColor =  new ScaleColorField(eggColor,.3);
	eggColor = new RotateColorField(eggColor,Vector(1,0,0),M_PI/2);
	eggColor = new RotateColorField(eggColor,Vector(0,0,1),M_PI/2);
	eggColor = new RotateColorField(eggColor,Vector(-1,0,0),M_PI/2);

	
	
	float eggH = clf.find( "-eggH", 0.0f, "Height of Eggy" );
	eggy = new TranslateScalarField(eggy,Vector(-5,eggH,0));
	eggColor = new TranslateColorField(eggColor,Vector(-5,eggH,0));
	
	float scale = clf.find( "-scale", 1.0f, "Height of Eggy" );

	
	
	Particle part1;
	part1.freq()  = clf.find("-freq1",0.6666f,"Pyroclast Frequency");
	part1.octaves() = clf.find("-octaves1",3.0f,"Pyroclast Octaves");
	part1.roughness()= clf.find("-roughness1",0.5f,"Pyroclast Roughness");
	part1.fjump()= clf.find("-fjump1",2.0f,"Pyroclast Fjump");
	part1.offset()= clf.find("-offset1",0.0f,"Pyroclast Offset");
	part1.pyroDensity() = scale;
	vector<float> trans1;
	trans1.push_back(0.0);trans1.push_back(0.0);trans1.push_back(0.0);
	trans1 = clf.findArray( "-trans1", trans1, "Pyroclast translation");
	part1.translate()= Vector(trans1[0],trans1[1],trans1[2]);
	part1.pyroAmplitude() = clf.find("-radius1",0.25f,"Pyroclast radius");
	float Temp1 = clf.find("-temp1",5000.0f,"Pyroclast Temp");	
	vector<float> pos1;
	pos1.push_back(0.0);pos1.push_back(0.0);pos1.push_back(0.0);
	pos1 = clf.findArray( "-pos1", pos1, "Pyroclast translation");
	Vector t1(-3 + pos1[0],-.75 + pos1[1],-1.5 + pos1[2]);	
	part1.P() = t1;
	Volume<float> *pyro1 = new Pyroclast(&part1);
	pyro1 = new ClampVolume(pyro1,0,1);	
	Volume<Color>* cPyro1 = new TempColor(pyro1,Temp1);
	

	Particle part2;
	part2.freq()  = clf.find("-freq2",0.6666f,"Pyroclast Frequency");
	part2.octaves() = clf.find("-octaves2",3.0f,"Pyroclast Octaves");
	part2.roughness()= clf.find("-roughness2",0.5f,"Pyroclast Roughness");
	part2.fjump()= clf.find("-fjump2",2.0f,"Pyroclast Fjump");
	part2.offset()= clf.find("-offset2",0.0f,"Pyroclast Offset");
	vector<float> trans2;
	trans2.push_back(0.0);trans2.push_back(0.0);trans2.push_back(0.0);
	trans2 = clf.findArray( "-trans2", trans2, "Pyroclast translation");
	part2.translate()= Vector(trans2[0],trans2[1],trans2[2]);
	part2.pyroAmplitude() = clf.find("-radius2",.25f,"Pyroclast radius");
	part2.pyroDensity() = scale;

	float Temp2 = clf.find("-temp2",5000.0f,"Pyroclast Temp");
	vector<float> pos2;
	pos2.push_back(0.0);pos2.push_back(0.0);pos2.push_back(0.0);
	pos2 = clf.findArray( "-pos2", pos2, "Pyroclast translation");
	Vector t2(-3 + pos2[0],-.75 + pos2[1],-1.5 + pos2[2]);	
	part2.P() = t2;	
	Volume<float> *pyro2 = new Pyroclast(&part2);
	pyro2 = new ClampVolume(pyro2,0,1);	
	Volume<Color>* cPyro2 = new TempColor(pyro2,Temp2);
	
	
	Particle part3;
	part3.freq()  = clf.find("-freq3",0.6666f,"Pyroclast Frequency");
	part3.octaves() = clf.find("-octaves3",3.0f,"Pyroclast Octaves");
	part3.roughness()= clf.find("-roughness3",0.5f,"Pyroclast Roughness");
	part3.fjump()= clf.find("-fjump3",0.50f,"Pyroclast Fjump");
	part3.offset()= clf.find("-offset3",0.0f,"Pyroclast Offset");
	vector<float> trans3;
	trans3.push_back(0.0);trans3.push_back(0.0);trans3.push_back(0.0);
	trans3 = clf.findArray( "-trans3", trans3, "Pyroclast translation");
	part3.translate()= Vector(trans3[0],trans3[1],trans3[2]);
	part3.pyroAmplitude() = clf.find("-radius3",0.25f,"Pyroclast radius");
	part3.pyroDensity() = scale;

	float Temp3 = clf.find("-temp3",5000.0f,"Pyroclast Temp");
	vector<float> pos3;
	pos3.push_back(0.0);pos3.push_back(0.0);pos3.push_back(0.0);
	pos3 = clf.findArray( "-pos3", pos3, "Pyroclast translation");
	Vector t3(-3 + pos3[0],-.75 + pos3[1],-1.5 + pos3[2]);	
	part3.P() = t3;	
	Volume<float> *pyro3 = new Pyroclast(&part3);
	pyro3 = new ClampVolume(pyro3,0,1);	
	Volume<Color>* cPyro3 = new TempColor(pyro3,Temp3);
	
	
	Particle part4;
	part4.freq()  = clf.find("-freq4",0.6666f,"Pyroclast Frequency");
	part4.octaves() = clf.find("-octaves4",3.0f,"Pyroclast Octaves");
	part4.roughness()= clf.find("-roughness4",0.5f,"Pyroclast Roughness");
	part4.fjump()= clf.find("-fjump4",2.0f,"Pyroclast Fjump");
	part4.offset()= clf.find("-offset4",0.0f,"Pyroclast Offset");
	vector<float> trans4;
	trans4.push_back(0.0);trans4.push_back(0.0);trans4.push_back(0.0);
	trans4 = clf.findArray( "-trans4", trans4, "Pyroclast translation");
	part4.translate()= Vector(trans4[0],trans4[1],trans4[2]);
	part4.pyroAmplitude() = clf.find("-radius4",0.25f,"Pyroclast radius");
	float Temp4 = clf.find("-temp4",5000.0f,"Pyroclast Temp");
	part4.pyroDensity() = scale;

	vector<float> pos4;
	pos4.push_back(0.0);pos4.push_back(0.0);pos4.push_back(0.0);
	pos4 = clf.findArray( "-pos4", pos4, "Pyroclast translation");
	Vector t4(-3 + pos4[0],-.75 + pos4[1],-1.5 + pos4[2]);	
	part4.P() = t4;	
	Volume<float> *pyro4 = new Pyroclast(&part4);
	pyro4 = new ClampVolume(pyro4,0,1);	
	Volume<Color>* cPyro4 = new TempColor(pyro4,Temp4);
	
	
	Particle part5;
	part5.freq()  = clf.find("-freq5",0.6666f,"Pyroclast Frequency");
	part5.octaves() = clf.find("-octaves5",3.0f,"Pyroclast Octaves");
	part5.roughness()= clf.find("-roughness5",0.5f,"Pyroclast Roughness");
	part5.fjump()= clf.find("-fjump5",2.0f,"Pyroclast Fjump");
	part5.offset()= clf.find("-offset5",0.0f,"Pyroclast Offset");
	vector<float> trans5;
	trans5.push_back(0.0);trans5.push_back(0.0);trans5.push_back(0.0);
	trans5 = clf.findArray( "-trans5", trans5, "Pyroclast translation");
	part5.translate()= Vector(trans5[0],trans5[1],trans5[2]);
	part5.pyroAmplitude() = clf.find("-radius5",0.25f,"Pyroclast radius");
	part5.pyroDensity() = scale;

	float Temp5 = clf.find("-temp5",5000.0f,"Pyroclast Temp");
	vector<float> pos5;
	pos5.push_back(0.0);pos5.push_back(0.0);pos5.push_back(0.0);
	pos5 = clf.findArray( "-pos5", pos5, "Pyroclast translation");
	Vector t5(-3 + pos5[0],-.75 + pos5[1],-1.5 + pos5[2]);	
	part5.P() = t5;	
	Volume<float> *pyro5 = new Pyroclast(&part5);	
	
	pyro5 = new ClampVolume(pyro5,0,1);	
	Volume<Color>* cPyro5 = new TempColor(pyro5,Temp5);
	
	
	
	
	Volume<float> *pyro = new UnionVolume(pyro1,pyro2);
	pyro = new UnionVolume(pyro,pyro3);
	pyro = new UnionVolume(pyro,pyro4);
	pyro = new UnionVolume(pyro,pyro5);
	Volume<float> *kpyro = new kField(1.0,pyro);
	Cs = new UnionColor(cPyro1,cPyro2);
	Cs = new UnionColor(Cs,cPyro3);
	Cs = new UnionColor(Cs,cPyro4);
	Cs = new UnionColor(Cs,cPyro5);
	
	
	
	Volume<float> *krobot = new kField(15.0,eggy);
	Volume<float> *k = new UnionVolume(kpyro,krobot);
	
	
	Ps = new UnionVolume(eggy,pyro);

	Pe = new emptyField<float>(0.0);	
	
	Cs = new UnionColor(Cs,eggColor);
	Ce = new emptyField<Color>(Color(0,0,0,1));
	
	
	//Volume<float> *k = new kField(15);
	Volume<float> *holdout = new emptyField<float>(0);

	
	DSMgroup DSMs(k);
	BaseGrid<float> *keyDgrid = new SparseGrid<float>();
	keyDgrid->init(Vector(-10,-10,-10),200,200,200,Vector(10,10,10),0.0);
	DSM *key = new DSM(keyDgrid,Ps,ds);
	key->sample(new Light(Vector(10,10,0),Color(1,1,1,1)));
	DSMs.add(key);
	BaseGrid<float> *fillDgrid = new SparseGrid<float>();
	fillDgrid->init(Vector(-10,-10,-10),200,200,200,Vector(10,10,10),0.0);
	DSM *fill = new DSM(fillDgrid,Ps,ds);
	fill->sample(new Light(Vector(0,-10,0),Color(2/3.0,2.0/3,2.0/3,1)));
	DSMs.add(fill);
	BaseGrid<float> *trophyLGrid = new SparseGrid<float>();
	trophyLGrid->init(Vector(-10,-10,-10),250,250,250,Vector(10,10,10),0.0);
	DSM *trophyL = new DSM(trophyLGrid,Ps,ds);
	trophyL->sample(new Light(trophyPos,trophyColor));
	DSMs.add(trophyL);

	
	
	
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
	
	
	/*
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
	 */
	Volume<float> *lfarmb = new ImplicitEllipse(2,1,Vector(1.0,0.0,0.0));
	Volume<float> *drillb = new ImplicitSphere(1.5);
	Volume<float> *drillt = new TranslateScalarField(drillb,Vector(-3,0,0));
	Volume<Color> *Cdrill = new ShapedColorField(drillt,Color(1,215/256.0,0,0));
	Volume<Color> *Clfarm = new ShapedColorField(lfarmb,Color(100/256.0,100/256.0,100/256.0,0));
	Volume<float> *lfarmdrb= new UnionVolume(lfarmb,drillt);
	Clfarm = new UnionColor(Cdrill,Clfarm);
	Volume<float> *lfarmr = new RotateScalarField(lfarmdrb,Vector(0.0,0,1.0),M_PI*3.0/16);
	Clfarm = new RotateColorField(Clfarm,Vector(0.0,0,1.0),M_PI*3.0/16);
	Volume<float> *lfarm  = new TranslateScalarField(lfarmr,Vector(-4.75,-1.4,0.0));
	Clfarm = new TranslateColorField(Clfarm,Vector(-4.75,-1.4,0.0));
	//*/
	
	
	
	
	
	
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
	//	Volume<float> *hb  = new CutoutVolume(body,head);//,0.95);
	Volume<Color> *Cbody = new ShapedColorField(body,Color(220.0/256,220.0/256,220.0/256,0.0));
	Volume<float> *bodyFace = new UnionVolume(body,face);
	
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
	//shape = new TranslateScalarField(shape,Vector(0,2,0));
	
	
	*vol  = new ClampVolume(shape,0,1);
	if(Cd != NULL){
		*Cd = new UnionColor(Clarm,Crarm);
		*Cd = new UnionColor(*Cd,Chand);
		//	*Cd = new UnionColor(*Cd,Cface);
		*Cd = new UnionColor(*Cd,Cbody);
		*Cd = new UnionColor(*Cd,Clegs);
		//	*Cd = new TranslateColorField(*Cd,Vector(0,2,0));
	}
}

