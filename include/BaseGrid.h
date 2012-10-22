//*******************************************************************
//
//   Grid.h
//
// Full Grid Class
//
//*******************************************************************

#ifndef BASE__GRID
#define BASE__GRID


#include "Vector.h"
#include "Volume.h"
#include <cstdio>
namespace lux{
template <typename U>
class BaseGrid{
	
protected:
	Vector llc;
	int nx,ny,nz;
	float dx,dy,dz;
	U outVal;

	void interpolate( float x, float dx, int nx, int& i, int& ii, float& w, float& ww )
	{
		float r = x/dx;
		i  =  r;
		if( i >= -1 && i < nx )
		{
			ii = i + 1;
			w = r-i;
			ww = 1.0 - w;
			if( i < 0 || i >= nx ){
				ww = 0;
				i = 0;
			}
			if( ii < 0 || ii >= nx ){
				w = 0;
				ii = 0;
			}
		}
		else
		{
			i = ii = -1;
			w = ww = 0;
		}
		
	}

	

public:
	BaseGrid():llc(Vector()),nx(0),ny(0),nz(0),dx(0),dy(0),dz(0){};
	~BaseGrid(){};
	virtual bool init(Vector llC,int nX,int nY,int nZ,float dX,float dY,float dZ,U outside){
		
		llc = llC;
		nx = nX;
		ny = nY;
		nz = nZ;
		dx = dX;
		dy = dY;
		dz = dZ;
		outVal = outside;
		return true;
		
	}
	virtual bool init(Vector llC,int nX,int nY,int nZ,Vector URC,U outside){
		
		llc = llC;
		nx = nX;
		ny = nY;
		nz = nZ;
		dx = (URC[0]-llc[0])/nx;
		dy = (URC[1]-llc[1])/ny;
		dz = (URC[2]-llc[2])/nz;
		outVal = outside;
		return true;
	}
	//get based on grid vals
	virtual const U get(const int i,const int j,const int k) const{
		U base;
		return base;
	}
	
	const U get(const float i,const float j,const float k) const{
		int ii = i;
		int jj = j;
		int kk = k;
		return get(ii,jj,kk);
	}
	
	//world coords
	const U get(const Vector &P){

		int ix, iix;
		float wx, wwx;
		float x = P[0] - llc[0];
		float y = P[1] - llc[1];
		float z = P[2] - llc[2];
		if(x <= 0 || y <= 0 || z <= 0)
			return outVal;
		if(x >= nx * dx || y >= ny * dy || z >= nz * dz)
			return outVal;
	//	printf("%f %f %f\n",x,y,z);
		interpolate( x, dx, nx, ix, iix, wx, wwx );
		if( ix < 0 ){ return outVal; }
		int iy, iiy;
		float wy, wwy;
		interpolate( y, dy, ny, iy, iiy, wy, wwy );
		if( iy < 0 ){ return outVal; }
		int iz, iiz;
		float wz, wwz;
		interpolate( z, dz, nz, iz, iiz, wz, wwz );
		if( iz < 0 ){ return outVal; }
		
		//printf("%i %i %i\n",ix,iy,iz);
		
		U density = 
		  get(ix,iy,iz) * wwx * wwy * wwz
		+ get(iix,iy,iz) * wx * wwy * wwz
		+ get(ix,iiy,iz) * wwx * wy * wwz
		+ get(iix,iiy,iz) * wx * wy * wwz
		+ get(ix,iy,iiz) * wwx * wwy * wz
		+ get(iix,iy,iiz) * wx * wwy * wz
		+ get(ix,iiy,iiz) * wwx * wy * wz
		+ get(iix,iiy,iiz) * wx * wy * wz;
		
		return density;
	}

	
	
	void accum(const int i,const int j,const int k,const U val){
		set(i,j,k,get(i,j,k) + val);
	}
	void accum(const Vector &P,const U val){
		int ix, iix;
		float wx, wwx;
		float x = P[0] - llc[0];
		float y = P[1] - llc[1];
		float z = P[2] - llc[2];
		interpolate( x, dx, nx, ix, iix, wx, wwx );
		if( ix < 0 ){ return; }
		int iy, iiy;
		float wy, wwy;
		interpolate( y, dy, ny, iy, iiy, wy, wwy );
		if( iy < 0 ){ return; }
		int iz, iiz;
		float wz, wwz;
		interpolate( z, dz, nz, iz, iiz, wz, wwz );
		if( iz < 0 ){ return; }
		
		//printf("%i %i %i\n",ix,iy,iz);
		//accum(ix,iy,iz,val * rx * ry * rz   );
		accum(ix,iy,iz,val * wwx * wwy * wwz);
		accum(iix,iy,iz,val * wx * wwy * wwz);
		accum(ix,iiy,iz,val * wwx * wy * wwz);
		accum(iix,iiy,iz,val * wx * wy * wwz);
		accum(ix,iy,iiz,val * wwx * wwy * wz);
		accum(iix,iy,iiz,val * wx * wwy * wz);
		accum(ix,iiy,iiz,val * wwx * wy * wz);
		accum(iix,iiy,iiz,val * wx * wy * wz);
	
	}
	void set(const Vector &P,const U val){
		int ix, iix;
		float wx, wwx;
		float x = P[0] - llc[0];
		float y = P[1] - llc[1];
		float z = P[2] - llc[2];
		interpolate( x, dx, nx, ix, iix, wx, wwx );
		if( ix < 0 ){ return; }
		int iy, iiy;
		float wy, wwy;
		interpolate( y, dy, ny, iy, iiy, wy, wwy );
		if( iy < 0 ){ return; }
		int iz, iiz;
		float wz, wwz;
		interpolate( z, dz, nz, iz, iiz, wz, wwz );
		if( iz < 0 ){ return; }
		
		//printf("%i %i %i\n",ix,iy,iz);
		//accum(ix,iy,iz,val * rx * ry * rz   );
		set(ix,iy,iz,val * wwx * wwy * wwz);
		set(iix,iy,iz,val * wx * wwy * wwz);
		set(ix,iiy,iz,val * wwx * wy * wwz);
		set(iix,iiy,iz,val * wx * wy * wwz);
		set(ix,iy,iiz,val * wwx * wwy * wz);
		set(iix,iy,iiz,val * wx * wwy * wz);
		set(ix,iiy,iiz,val * wwx * wy * wz);
		set(iix,iiy,iiz,val * wx * wy * wz);
		
	}

	
		
	
	Vector getVect(const int i,const int j,const int k){
		return llc + Vector(i * dx, j * dy, k *dz);
	}
	virtual void clear(){
		fprintf(stderr,"CALL TO BASE FUNCTION\n");
	}
	void sample(Volume<U> *vol){
		//clear();
		printf("STARTING GRID\r");
		fflush(stdout);
		for(int k = 0;k < nz;k++){
			for(int j = 0;j < ny;j++){
				for(int i = 0;i < nx;i++){
	//				if(k*100/nz == 20)printf("%f %f %f \n",getVect(i,j,k)[0],getVect(i,j,k)[1],getVect(i,j,k)[2]);
					set(i,j,k,vol->eval(getVect(i,j,k)));
				}
			}
			printf("SETTING GRID %.1f %%\r",(k * 100.0/nz));
			fflush(stdout);
		}
		printf("\rSAMPLING GRID COMPLETE     \n");
	}
	
	
	virtual void set(const int i,const int j,const int k,const U val){
		fprintf(stdout,"ERROR BASE FN CALLED\n");
		return;
	}
	
	void set(const float i,const float j,const float k,const U val){
		int ii = i;
		int jj = j;
		int kk = k;
		set(ii,jj,kk,val);
		return;
	}
	
	const int NX() const { return nx; }
    const int NY() const { return ny; }
    const int NZ() const { return nz; }
	
	const Vector &LLC() const{return llc;}
	
	const float DX() const { return dx; }
    const float DY() const { return dy; }
    const float DZ() const { return dz; }
	
	bool isInside(const Vector &P){
		Vector oP = P - llc;
		
		if(oP[0] < 0.0 || oP[0] > (nx-1) * dx)
			return false;
		if(oP[1] < 0.0 || oP[1] > (ny-1) * dy)
			return false;
		if(oP[2] < 0.0 || oP[2] > (nz-1) * dz)
			return false;
		return true;
	}
	
	
};
	
	//nx ny nz
	
	
	
}

#endif


