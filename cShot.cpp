#include "cShot.h"
#include "Globals.h"

cShot::cShot(void){}
cShot::~cShot(void){}

void cShot::Init(int a, int b, int c, int t)
{
	x = a;
	y = b;
	z = c;
	target = t;
}
void cShot::getCoord(int &xcoord, int &ycoord, int &zcoord)
{
	xcoord = x;
	ycoord = y;
	zcoord = z;
}