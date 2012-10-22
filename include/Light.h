//*******************************************************************
//
//   LIGHT.h
//
// light class
//
//*******************************************************************

#ifndef LIGHT
#define LIGHT

#include "Vector.h"
#include "Color.h"
using namespace std;
namespace lux{
    
	class Light{
	public:
		Light(Vector Pos,Color Col):pos(Pos),color(Col){};
		const Vector getPos(){return pos;}
		const Color  getColor(){return color;}
	private:
		Vector pos;
		Color color;
	};
    
}   

#endif
