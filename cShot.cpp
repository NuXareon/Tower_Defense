#include "cShot.h"
#include "Globals.h"

cShot::cShot(void){}
cShot::~cShot(void){}

void cShot::Init(float a, float b, float c, int t)
{
	x = a;
	y = b;
	z = c;
	target = t;
}
void cShot::getCoord(float &xcoord, float &ycoord, float &zcoord)
{
	xcoord = x;
	ycoord = y;
	zcoord = z;
}
void cShot::IA(int mpos, int w, int s, int dir, float inc)
{
	float xm = (mpos%w)*s;
	float ym = -0.2*s;
	float zm = -(mpos/w)*s;

	float xaux = x;
	float yaux = y;
	float zaux = z;

	if (dir == 1) xm += inc;
	else if (dir == 2) xm -= inc;
	else if (dir == 3) zm -= inc;
	else if (dir == 4) zm += inc;

	float vxm = (xm-xaux);
	float vym = (ym-yaux);
	float vzm = (zm-zaux);

	if (vxm*vxm+vym*vym+vzm*vzm != 0)
	{
	
	float magvm = sqrt(vxm*vxm+vym*vym+vzm*vzm);

		vxm /= magvm;
		vym /= magvm;
		vzm /= magvm;

		xaux += vxm*SPEED_FACTOR;
		yaux += vym*SPEED_FACTOR;
		zaux += vzm*SPEED_FACTOR;
	
		if (x < xm && xaux > xm) xaux = xm;
		else if (x > xm && xaux < xm) xaux = xm;
		if (y < ym && yaux > ym) yaux = ym;
		else if (y > ym && yaux < ym) yaux = ym;
		if (z < zm && zaux > zm) zaux = zm;
		else if (z > zm && zaux < zm) zaux = zm;

		x = xaux;
		y = yaux;
		z = zaux;
	}
}
int cShot::getTarget()
{
	return target;
}