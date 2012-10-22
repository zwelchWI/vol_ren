

#ifndef __POISSON_SOLVERS_H__
#define __POISSON_SOLVERS_H__
#include "Vector.h"
#include "BaseGrid.h"

namespace lux
{

   void FFTDivFree( BaseGrid<Vector>* divfreeU );

}


#endif
