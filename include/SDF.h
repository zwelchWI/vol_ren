//*******************************************************************
//
//   SDF.h
//
// code for signed distance function
//
//*******************************************************************

#ifndef SDFH
#define SDFH

#include "Volume.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
namespace lux{

class Triangle{
public:
	Triangle(){
		V0 = V1 = V2 = Vector();
	}
	Triangle(Vector v0,Vector v1, Vector v2):V0(v0),V1(v1),V2(v2){};
	Vector get(int i){
		if(i==0)
			return V0;
		if(i==1)
			return V1;
		if(i==2)
			return V2;
		return Vector();
	}
private:
	Vector V0,V1,V2;
};
	
	
class SDF : public Volume<float>{
public:
	SDF(Triangle *tri){
		tris.push_back(tri);
	}
    SDF(char *filename){
		mx = my = mz = 10000000;//big number
		Mx = My = Mz =-10000000;//big negative number
		std::vector<Vector> verts;
		verts.push_back(Vector());//useless, indexing starts at 1 in obj
		file = fopen(filename,"r");
		if(file == NULL){
			fprintf(stderr,"NO SUCH OBJ\n");
			exit(1);
		}
		char buf[512];
		char *parse;
		double x,y,z;
		int v0,v1,v2;
		while(fgets(buf,512,file)){
			if(buf[0] == 'v'){
				parse = strtok(buf," ");
				parse = strtok(NULL," ");
				sscanf(parse,"%lf",&x);
				if(x > Mx)
					Mx = x;
				if(x < mx)
					mx = x;
				parse = strtok(NULL," ");
				sscanf(parse,"%lf",&y);
				if(y > My)
					My = y;
				if(y < my)
					my = y;
				parse = strtok(NULL," ");
				sscanf(parse,"%lf",&z);
				if(z > Mz)
					Mz = z;
				if(z < mz)
					mz = z;
				
	//			printf("NEW VERTEX %d %lf %lf %lf\n",verts.size(),x,y,z);
				verts.push_back(Vector(x,y,z));
				
			}
			if(buf[0] == 'f'){
				parse = strtok(buf," ");
				parse = strtok(NULL," ");
				sscanf(parse,"%d",&v0);
				parse = strtok(NULL," ");
				sscanf(parse,"%d",&v1);
				parse = strtok(NULL," ");
				sscanf(parse,"%d",&v2);
				
	//			printf("NEW TRIANGLE %d %d %d\n",v0,v1,v2);
				tris.push_back(new Triangle(verts[v0],verts[v1],verts[v2]));
			}
		}
		//printf("mx %f Mx %f my %f My %f mz %f Mz %f \n",mx,Mx,my,My,mz,Mz);
		fclose(file);
	}
    const float eval(const Vector&X) const{
		Triangle *tri;
		double u,v,t;
		double min = 500000000;
		Vector minVect;
		for(int ndx = 0; ndx< tris.size();ndx++){
			tri = tris[ndx];
			Vector e1 = tri->get(1) - tri->get(0);
			Vector e2 = tri->get(2) - tri->get(0);
			Vector Xp = tri->get(0) - X;
			v = (e1.magnitude() * e1.magnitude() * (Xp * e2) - ((Xp * e1) *(e1 * e2)))/(e1.magnitude() * e1.magnitude() * e2.magnitude() * e2.magnitude() - ((e1 * e2) * (e1 * e2)));
			u = ((Xp *e1) - (e1 * e2) *v)/(e1.magnitude() * e1.magnitude());
			u *= -1;
			v *= -1;
			if(u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && u+v >= 0.0 && u+v <= 1.0){
				//works
				
				if(min > (tri->get(0) + u * e1 + v *e2 - X).magnitude()){
					//printf("here1\n");
					//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
					//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
					//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
					//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
					//printf("u %f v %f\n",u,v);
					min = (tri->get(0) + u * e1 + v *e2 - X).magnitude();
					minVect = tri->get(0) + u * e1 + v *e2 - X;
					//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
				}
			}
			else{
				//check edges
				//edge1
				//t = - (V0. - P.) * e2)/  e2^2
				t = -1.0 * (Xp * e1)/(e1.magnitude() * e1.magnitude());
				if(t >= 0.0 && t <= 1.0){
					if(min > (tri->get(0) + t * e1 - X).magnitude()) {
						//printf("here2\n");
						//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
						//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
						//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
						//printf("t %f %f\n",t,(tri->get(0) + t * e1 - X).magnitude());
						//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
						min = (tri->get(0) + t * e1 - X).magnitude();
						minVect = tri->get(0) + t * e1 - X;
						//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
					}
				}
				t = -1.0 * (Xp * e2)/(e2.magnitude() * e2.magnitude());
				if(t >= 0.0 && t <= 1.0){
					if(min > (tri->get(0) + t * e2 - X).magnitude()){
						min = (tri->get(0) + t * e2 - X).magnitude();
						minVect = tri->get(0) + t * e2 - X; 
						//printf("herez3\n");
						//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
						//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
						//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
						//printf("t %f %f\n",t,(tri->get(0) + t * e2 - X).magnitude());
						//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);

						//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);

											}
				}
				Vector e3 = tri->get(2) - tri->get(1);
				
				
				t = -1.0 * ((tri->get(1) - X) * e3)/(e3.magnitude() * e3.magnitude());
				if(t >= 0.0 && t <= 1.0){
					if(min > (tri->get(1) + t * e3 - X).magnitude()) {
						//printf("here4\n");
						//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
						//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
						//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
						//printf("t %f %f\n",t,(tri->get(1) + t * e3 - X).magnitude());
						//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
						min = (tri->get(1) + t * e3 - X).magnitude();
						minVect = tri->get(1) + t * e3 - X; 
						//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
					}
				}
				
				
				//check vertices
				if( min > (X - tri->get(0)).magnitude()){
					//printf("here5\n");
					min = (X - tri->get(0)).magnitude();
					minVect = tri->get(0) - X; 
					//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
					//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
					//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
					//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
					//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
					
					
				}
				if( min > (X - tri->get(1)).magnitude()){
					//printf("here6\n");
					min = (X - tri->get(1)).magnitude();
					minVect = tri->get(1) - X; 
					//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
					//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
					//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
					//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);

					//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);

				}
				if( min > (X - tri->get(2)).magnitude()){
					//printf("here7\n");
					//printf("V0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
					//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
					//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
					min = (X - tri->get(2)).magnitude();
					minVect = tri->get(2) - X; 
					//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);

					//printf("inside %f min ",min);//printf("minVect %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);

				}
			}
		}
	//	printf("min %f\n",min);
		minVect.normalize();
		minVect = Vector(0,0,minVect[2]);
	//	printf("MIN DIR %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
		int hit1 = 0;
		int hit2 = 0;
	//	Vector P,S,q;
		Vector hitPoint,firstHit;
		int inside = 0;
		double a,b,c,d,e,f,g,h,i,j,k,l,M;
		Vector hold;
		vector<Vector> uniqueHits;
		for(int ndx = 0; ndx< tris.size();ndx++){
			tri = tris[ndx];
		
			//printf("\n\nV0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
			//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
			//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);
			//my moller trumbore algorithm attempt
		

			hold = tri->get(0) - X;
			hold.normalize();
			if(abs(hold[0]  - minVect[0]) == 0){
				//printf("x same\n");
				if(abs(hold[1]  - minVect[1]) ==0){
					//printf("y same\n");
			//		//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
					if( abs(hold[2]  - minVect[2]) == 0){					
						
						//printf("WINNER1\n");
						
						hitPoint = tri->get(0);
						//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);

						if(uniqueHits.size() == 0){
							uniqueHits.push_back(hitPoint);
							//printf("NEW ADDITION\n");
						}
						else{
							int count = 0;
							for(int ndx = 0;ndx < uniqueHits.size();ndx++){
								if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0 ){
									//printf("x same\n");
									if(abs(uniqueHits[ndx][1]  - hitPoint[1]) == 0 ){
										//printf("y same\n");
										//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
										if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){
											//printf("z same\n");
											break;
										}
										else
											count++;
									}
									else
										count++;
								}
								else
									count++;
								//printf("count %d\n",count);
							}
							//not in list
							if(count == uniqueHits.size()){
								uniqueHits.push_back(hitPoint);
								//printf("NEW ADDITION\n");
							}
						}
						continue;
					}}}
			
			
			
			
			
			hold = tri->get(1) - X;
			hold.normalize();
			if(abs(hold[0]  - minVect[0]) == 0){
				//printf("x same\n");
				if(abs(hold[1]  - minVect[1]) ==0){
					//printf("y same\n");
					//		//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
					if( abs(hold[2]  - minVect[2]) == 0){						
				
						//printf("WINNER2\n");
						hitPoint = tri->get(1);

						//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);


				if(uniqueHits.size() == 0){
					uniqueHits.push_back(hitPoint);
					//printf("NEW ADDITION\n");
				}
				else{
					int count = 0;
					for(int ndx = 0;ndx < uniqueHits.size();ndx++){
						if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0 ){
							//printf("x same\n");
							if(abs(uniqueHits[ndx][1]  - hitPoint[1]) == 0 ){
								//printf("y same\n");
								//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
								if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){									//printf("z same\n");
									break;
								}
								else
									count++;
							}
							else
								count++;
						}
						else
							count++;
						//printf("count %d\n",count);
					}
					//not in list
					if(count == uniqueHits.size()){
						uniqueHits.push_back(hitPoint);
						//printf("NEW ADDITION\n");
					}
				}
				continue;
					}}}
			
			hold = tri->get(2) - X;
			hold.normalize();
			if(abs(hold[0]  - minVect[0]) == 0){
				//printf("x same\n");
				if(abs(hold[1]  - minVect[1]) ==0){
					//printf("y same\n");
					//		//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
					if( abs(hold[2]  - minVect[2]) == 0){						
						//printf("WINNER3\n");

						hitPoint = tri->get(2);
						//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);

				if(uniqueHits.size() == 0){
					uniqueHits.push_back(hitPoint);
					//printf("NEW ADDITION\n");
				}
				else{
					int count = 0;
					for(int ndx = 0;ndx < uniqueHits.size();ndx++){
						if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0 ){
							//printf("x same\n");
							if(abs(uniqueHits[ndx][1]  - hitPoint[1]) == 0 ){
								//printf("y same\n");
								//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
								if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){									
									//printf("z same\n");
									break;
								}
								else
									count++;
							}
							else
								count++;
						}
						else
							count++;
						//printf("count %d\n",count);
					}
					//not in list
					if(count == uniqueHits.size()){
						uniqueHits.push_back(hitPoint);
						//printf("NEW ADDITION\n");
					}
				}
				continue;
					}}}
			
			//printf("\n\nV0 : %lf %lf %lf\n",tri->get(0)[0],tri->get(0)[1],tri->get(0)[2]);
			//printf("V1 : %lf %lf %lf\n",tri->get(1)[0],tri->get(1)[1],tri->get(1)[2]);
			//printf("V2 : %lf %lf %lf\n",tri->get(2)[0],tri->get(2)[1],tri->get(2)[2]);

			
			Vector e1 = tri->get(1) - tri->get(0);
			Vector e2 = tri->get(2) - tri->get(0);
			
			Vector Xp = tri->get(0) - X;
			
			//printf("%f\n",(X - tri->get(0))*(e1 ^ minVect));

			if(abs((X - tri->get(0))*(e1 ^minVect)) == 0){
				/*
				 a = x2 - x1 e1
				 b = x4 - x3 minVect
				 c = x3 - x1 
				 */
				t = (((X-tri->get(0)) ^ minVect) * (e1 ^ minVect))/(e1^minVect).magnitude()/(e1^minVect).magnitude();
				//printf("t3 : %f\n",t);
				hitPoint = tri->get(0) + t * e1;
				//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);
				if(t >= 0 && t <= 1){
					//printf("WINNER4\n");

					if(uniqueHits.size() == 0){
						uniqueHits.push_back(hitPoint);
						//printf("NEW ADDITION1\n");
					}
					else{
						int count = 0;
						for(int ndx = 0;ndx < uniqueHits.size();ndx++){
							if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0){
								//printf("x same\n");
								if(abs(uniqueHits[ndx][1]  - hitPoint[1])== 0){
									//printf("y same\n");
									//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
									if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){
										//printf("z same\n");
										break;
									}
									else
										count++;
								}
								else
									count++;
							}
							else
								count++;
							//printf("count %d\n",count);
						}
						//not in list
						if(count == uniqueHits.size()){
							uniqueHits.push_back(hitPoint);
							//printf("NEW ADDITION2\n");
						
						}
					}
					continue;
				}
			}
			//printf("%f\n",(X - tri->get(0))*(e2 ^ minVect));
			if(abs((X - tri->get(0))*(e2 ^ minVect)) == 0){
				/*
				 a = x2 - x1 e1
				 b = x4 - x3 minVect
				 c = x3 - x1 
				 */
				t = (((X-tri->get(0)) ^ minVect) * (e2 ^ minVect))/(e2^minVect).magnitude()/(e2^minVect).magnitude();
				
				//printf("t4 : %f\n",t);
				hitPoint = tri->get(0) + t * e2;
				//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);
				if(t >= 0 && t <= 1){
					//printf("WINNER5\n");

					if(uniqueHits.size() == 0){
						uniqueHits.push_back(hitPoint);
						//printf("NEW ADDITION\n");
					}
					else{
						int count = 0;
						for(int ndx = 0;ndx < uniqueHits.size();ndx++){
							if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0 ){
								//printf("x same\n");
								if(abs(uniqueHits[ndx][1]  - hitPoint[1]) == 0 ){
									//printf("y same\n");
									//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
									if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){
										//printf("z same\n");
										break;
									}
									else
										count++;
								}
								else
									count++;
							}
							else
								count++;
							//printf("count %d\n",count);
						}
						//not in list
						if(count == uniqueHits.size()){
							uniqueHits.push_back(hitPoint);
							//printf("NEW ADDITION\n");
							
						}
					}
					continue;

				}

			}
			Vector e3 = tri->get(2) - tri->get(1);

			//printf("%f\n",(X - tri->get(1))*(e3 ^ minVect));
			if(abs((X - tri->get(1))*(e3 ^ minVect)) == 0){
				/*
				 a = x2 - x1 e1
				 b = x4 - x3 minVect
				 c = x3 - x1 
				 */
				t = (((X-tri->get(1)) ^ minVect) * (e3 ^ minVect))/(e3^minVect).magnitude()/(e3^minVect).magnitude();
				//printf("t5 : %f\n",t);
				hitPoint = tri->get(1) + t * e3;
				//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);

				if(t >= 0 && t <= 1){
					//printf("WINNER6\n");

					if(uniqueHits.size() == 0){
						uniqueHits.push_back(hitPoint);
						//printf("NEW ADDITION\n");
					}
					else{
						int count = 0;
						for(int ndx = 0;ndx < uniqueHits.size();ndx++){
							if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0 ){
								//printf("x same\n");
								if(abs(uniqueHits[ndx][1]  - hitPoint[1]) == 0 ){
									//printf("y same\n");
									//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
									if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){
										//printf("z same\n");
										break;
									}
									else
										count++;
								}
								else
									count++;
							}
							else
								count++;
							//printf("count %d\n",count);
						}
						//not in list
						if(count == uniqueHits.size()){
							uniqueHits.push_back(hitPoint);
							//printf("NEW ADDITION\n");
							
						}
					}
					continue;

				}

			}			
			
			
			
			
			Vector T = X - tri->get(0);
			Vector P = minVect ^ e2;
			Vector Q = T ^ e1;
			double epsilon = 10e-7;
			double det = e1 * P;
			if(det > -epsilon  && det < epsilon)
				continue;
			double invdet = 1/det;
			u = (T * P) * invdet;
			//printf("u : %.12f\n",u);
			if(u < 0.0  || u > 1.0 )
				continue;
			v = (minVect * Q) * invdet;
			//printf("v : %.9f\n",v);
			if(v < 0.0  || u + v > 1.0)
				continue;
			t = (e2 * Q) * invdet;
			//printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
			//printf("t : %f\n",t);
			//printf("MIN DIR %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
			if(t > 0.0){
				//printf("WINNER7\n");
				hitPoint = X + t * minVect;
				//printf("hit %.9lf %.9lf %.9lf\n",hitPoint[0],hitPoint[1],hitPoint[2]);

				if(uniqueHits.size() == 0){
					uniqueHits.push_back(hitPoint);
					//printf("NEW ADDITION\n");
				}
				else{
					int count = 0;
					for(int ndx = 0;ndx < uniqueHits.size();ndx++){
						if(abs(uniqueHits[ndx][0]  - hitPoint[0]) == 0 ){
							//printf("x same\n");
							if(abs(uniqueHits[ndx][1]  - hitPoint[1]) == 0 ){
								//printf("y same\n");
								//printf("%f %f\n",hitPoint[2],uniqueHits[ndx][2]);
								if( abs(uniqueHits[ndx][2]  - hitPoint[2]) == 0){
									//printf("z same\n");
									break;
								}
								else
									count++;
							}
							else
								count++;
						}
						else
							count++;
						//printf("count %d\n",count);
					}
					//not in list
					if(count == uniqueHits.size()){
						uniqueHits.push_back(hitPoint);
						//printf("NEW ADDITION\n");
					}
				}
			}
			
			
			//figure inside or outside
		/*	Vector e1 = tri->get(1) - tri->get(0);
			Vector e2 = tri->get(2) - tri->get(0);
			P = minVect ^ e2;
			a = e1 * P;
		//	printf("\n\na : %.9f\n",a);
			if(a > -10e-6 && a < 10e-6)
				continue;
			f = 1/a;
			S = X - tri->get(0);
			u = f * (S * P);
		//	printf("u : %.9f\n",u);
			if(u <0.0 || u > 1.0)
				continue;
			q = S ^ e1;
			v = f * (minVect * q);
		//	printf("v : %.9f\n",v);
			if(v < -0.0 || (u+v) > 1.0)
				continue;
			t = f * (e2 * q);
		//	printf("t : %.9f\n",t);
			if(t > 0.0){
		//		printf("WINNER\n");
				hitPoint = X + t * minVect;
				if(uniqueHits.size() == 0)
					uniqueHits.push_back(hitPoint);
				else{
					int count = 0;
					for(int ndx = 0;ndx < uniqueHits.size();ndx++){
						if(hitPoint ==uniqueHits[ndx])
							break;
						else
							count++;
					}
					//not in list
					if(count == uniqueHits.size()){
						uniqueHits.push_back(hitPoint);
					}
				}

			}
		/*	
			//CHECK FOR OTHER WAY
			P = (-1 * minVect) ^ e2;
			a = e1 * P;
			//	printf("a : %f\n",a);
			if(a > -10e-6 && a < 10e-6)
				continue;
			f = 1/a;
			S = X - tri->get(0);
			u = f * (S * P);
			if(u <0.0 || u > 1.0)
				continue;
			q = S ^ e1;
			v = f * ((-1 * minVect) * q);
			if(v < 0.0 || (u+v) > 1.0)
				continue;
			//	printf("u : %f\n",u);
			//	printf("v : %f\n",v);
			t = f * (e2 * q);
			//	printf("t : %f\n",t);
			if(t > 0.0)
				hit2 = 1;
			
		*/	
		 }
	//	printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
//		printf("MIN DIR %lf %lf %lf\n",minVect[0],minVect[1],minVect[2]);
//		printf("Point %lf %lf %lf\n",X[0]-minVect[0],X[1]-minVect[1],X[2]-minVect[2]);

	//	if((hit1 + hit2) == 2);printf("INSIDE\n");
	//	else{ 
		//	printf("OUTSIDE\n");
		//	min *= -1.0;
	//	}
		//printf("size %d\n",uniqueHits.size());printf("X %lf %lf %lf\n",X[0],X[1],X[2]);
		if(uniqueHits.size()%2 == 0){
			min *= -1.0;
		}
	//	else if(X[0] >= Mx || X[0] <= mx || X[1] >= My || X[1] <= my ||X[2] >= Mz || X[2] <= mz ){
	//		min *= -1.0;
	//	}
		
	//	if(!inside){
	//		min *= -1.0;
			//printf("outside\n");
	//	}
		//else printf("inside\n");
//		 printf("inside %f min ",min);printf("X %lf %lf %lf\n",X[0],X[1],X[2]);

		
        return min;
    }
private:
	char *fname;
	FILE *file;
	std::vector<Triangle *> tris;
	double mx,my,mz;
	double Mx,My,Mz;
	
};  
	
}
#endif



