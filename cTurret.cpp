#include "cTurret.h"
#define _USE_MATH_DEFINES
#include "Globals.h"

cTurret::cTurret(void){}
cTurret::~cTurret(void){}

void cTurret::Init(int t)
{
	type = t;
	// switch type
	cost = 100;
	lvl = 1;
	rotationY = 0;
	target = -1;
	distTarget = -1;
	attackCd = 0;
	range = 3;
}

int cTurret::getRotationY()
{
	return rotationY;
}
bool cTurret::AI(map<int,cMonstre> monsters, int pos, int w)
{
	adquireTarget(monsters, pos, w);
	return shootTarget(pos, w);
}
void cTurret::adquireTarget(map<int,cMonstre> monsters, int pos, int w)
{
	int x = pos%w;
	int z = pos/w;
	target = -1;
	distTarget = -1;
	rotationY = 0;
	if (monsters.size() > 0)
	{
		std::map<int,cMonstre>::iterator iter;
		for(iter=monsters.begin(); iter != monsters.end(); ++iter){	
			int mpos = iter->second.GetPositionAct();
			updateTarget(mpos,x,z,w,iter->first);
		}

		if (distTarget != -1) 
		{
			int mpos = monsters[target].GetPositionAct();
			updateRotationY(mpos,x,z,w);
		}
	}
}
bool cTurret::shootTarget(int pos, int w)
{
	if (--attackCd <= 0)
	{
		attackCd = 100;
		return true;
	}
	return false;
}
int cTurret::euclideanDist(int x1, int y1, int x2, int y2)
{
	float f = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	return sqrt(f);
}
void cTurret::updateTarget(int mpos, int x, int z, int w, int targetId)
{
	int xm = mpos%w;
	int zm = mpos/w;
	int dist = euclideanDist(x,z,xm,zm);
	if (dist < range && (dist < distTarget || distTarget == -1))
	{
		target = targetId;
		distTarget = dist;
	}
}
void cTurret::updateRotationY(int mpos, int x, int z, int w)
{
	int xm = x-mpos%w;
	int zm = z-mpos/w;
	float dotTM = z*zm;
	float magTM = sqrt((float)z*z)*sqrt((float)xm*xm+(float)zm*zm);
	float angle = acos((float)dotTM/magTM);
	rotationY = angle*180/M_PI;
	if (xm > 0) rotationY = -rotationY;
}
int cTurret::getTarget()
{
	return target;
}
