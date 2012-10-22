//*******************************************************************
//
//   SparseGrid.h
//
// SPARSE Grid Class
//
//*******************************************************************

#ifndef SPARSEGRID
#define SPARSEGRID

#include "BaseGrid.h"
namespace lux{
template <typename U>	
class SparseGrid : public BaseGrid<U>{
public:
	SparseGrid(int PSize = 10):pSize(PSize){};
	bool init(Vector llC,int nX,int nY,int nZ,float dX,float dY,float dZ,U outside){
	
		if(nX% (pSize) != 0){
			nX = nX + pSize - nX % ( pSize);
			printf("NX ADJUSTED TO %d\n",nX);
		}
		if(nY % (pSize ) != 0){
			nY = nY + pSize - nY% (pSize);
			printf("NY ADJUSTED TO %d\n",nY);
		}
		if(nZ % (pSize) != 0){ 
			nZ = nZ + pSize - nZ% (pSize);
			printf("NZ ADJUSTED TO %d\n",nZ);
		}
	
		BaseGrid<U>::init(llC,nX,nY,nZ,dX,dY,dZ,outside);
		
		int totalSize = int(ceil(1.0 *nX/pSize)*ceil(1.0*nY/pSize)*ceil(1.0*nZ/pSize));
		grid =new U* [totalSize];
		if(grid == NULL)
			return false;
		for(int ndx = 0;ndx < totalSize;ndx++){
			grid[ndx] = NULL;
		}
		return true;
	}
	bool init(Vector llC,int nX,int nY,int nZ,Vector URC,U outside){
		
		if(nX% (pSize) != 0){
			nX = nX + pSize - nX % ( pSize);
		}
		if(nY % (pSize ) != 0){
			nY = nY + pSize - nY% (pSize);
		}
		if(nZ % (pSize) != 0){ 
			nZ = nZ + pSize - nZ% (pSize);
		}
		
		BaseGrid<U>::init(llC,nX,nY,nZ,URC,outside);
		int totalSize = int(ceil(1.0 *nX/pSize)*ceil(1.0*nY/pSize)*ceil(1.0*nZ/pSize));
		grid =new U* [totalSize];
		if(grid == NULL)
			return false;
		for(int ndx = 0;ndx < totalSize;ndx++){
			grid[ndx] = NULL;
		}
		return true;

	}
	
	//get based on grid vals
	const U get(const int i,const int j,const int k) const{
		int myIndex = index(i, j, k);
		if(grid[myIndex] == NULL){
			return(this->outVal);
		}
		else{
			int ii = i % pSize;
			int jj = j % pSize;
			int kk = k % pSize;
			
			int partitionIndex = ii + pSize*( jj + pSize * kk );
			return grid[myIndex][partitionIndex];
		}	
	}
	
	void set(const int i,const int j,const int k,const U val){
		if(val != this->outVal){
			int myIndex = index(i, j, k);
			if(grid[myIndex]==NULL){
				grid[myIndex] = new U[pSize * pSize * pSize];
				for(int i = 0; i < pSize * pSize * pSize; i++){
					grid[myIndex][i] = this->outVal;
				}
			}
			int ii = i % pSize;
			int jj = j % pSize;
			int kk = k % pSize;
			int partitionIndex = ii + pSize*( jj + pSize * kk );
			grid[myIndex][partitionIndex] = val;
		}
	}
	
	void clear(){
		int totalSize = int(ceil(1.0 *this->nx/pSize)*ceil(1.0*this->ny/pSize)*ceil(1.0*this->nz/pSize));
		for(int ndx = 0;ndx < totalSize;ndx++){
			if(grid[ndx] != NULL){
				delete grid[ndx];
				grid[ndx] = NULL;
			}
		}

	}
	
private:
	
	int index(int i, int j, int k) const{  
		int ii = i/pSize;
		int jj = j/pSize;
		int kk = k/pSize;
		int nnx = this->nx/pSize;
		int nny = this->ny/pSize;
		int nnz = this->nz/pSize;
		return ii + nnx*( jj + nny*kk );
		
	}
	
	U **grid;
	int pSize;
	
};	
}

#endif


