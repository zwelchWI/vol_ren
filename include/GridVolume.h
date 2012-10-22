//*******************************************************************
//
//   grid volume.h
//
// code for treating a grid as a volume
//
//*******************************************************************

#ifndef GRIDVOLH
#define GRIDVOLH

#include "Volume.h"
#include "BaseGrid.h"

namespace lux{
	template <typename U>
	class GridVolume : public Volume<U>{
	public:
		GridVolume(BaseGrid<U> *grid_):grid(grid_){};
		const U eval(const Vector&X) const{return grid->get(X);}
	private:
		BaseGrid<U> *grid;
		
	};  
	
}
#endif



