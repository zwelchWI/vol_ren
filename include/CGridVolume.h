//*******************************************************************
//
//   color grid volume.h
//
// code for treating a grid as a volume
//
//*******************************************************************

#ifndef CGRIDVOLH
#define CGRIDVOLH

#include "Volume.h"
#include "BaseGrid.h"
#include "Color.h"
namespace lux{
	
	class CGridVolume : public Volume<Color>{
	public:
		CGridVolume(BaseGrid<Color> *grid_):grid(grid_){};
		const Color eval(const Vector&X)const {return grid->get(X);}
	private:
		BaseGrid<Color> *grid;
		
	};  
	
}
#endif



