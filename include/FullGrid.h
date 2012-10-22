//*******************************************************************
//
//   FullGrid.h
//
// Full Grid Class
//
//*******************************************************************

#ifndef FULLGRID
#define FULLGRID

#include "BaseGrid.h"
namespace lux{
template <typename U>	
class FullGrid : public BaseGrid<U>{
public:
	bool init(Vector llC,int nX,int nY,int nZ,float dX,float dY,float dZ,U outside){
		
		
		BaseGrid<U>::init(llC,nX,nY,nZ,dX,dY,dZ,outside);
		
		grid =(U *) malloc(sizeof(U) * this->nx*this->ny*this->nz);
		if(grid == NULL)
			return false;
		
		for(int ndx = 0;ndx < this->nx*this->ny*this->nz;ndx++){
			grid[ndx] = this->outVal;
		}
		return true;
	}
	bool init(Vector llC,int nX,int nY,int nZ,Vector URC,U outside){
		
		BaseGrid<U>::init(llC,nX,nY,nZ,URC,outside);
		grid = (U *) malloc(sizeof(U) * this->nx*this->ny*this->nz);
		if(grid == NULL)
			return false;
		
		for(int ndx = 0;ndx < this->nx*this->ny*this->nz;ndx++){
			grid[ndx] = this->outVal;
		}
		return true;
	}
	
	//get based on grid vals
	const U get(const int i,const int j,const int k) const{
		return grid[i + this->nx * (j + this->ny * k)];
	}
	
	void clear(){return;	}
	
	void set(const int i,const int j,const int k,const U val){
		grid[i + this->nx * (j + this->ny * k)] = val;
	}
	
	
	
private:
	
	U *grid;
	
	
};	
}

#endif


