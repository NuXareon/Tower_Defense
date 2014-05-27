#include "cTurret.h"
#define _USE_MATH_DEFINES
#include "Globals.h"

cTurret::cTurret(void){}
cTurret::~cTurret(void){}

void cTurret::Init(int t)
{
	type = t;
	lvl = 1;
	rotationY = 0;
	distTarget = -1;
	if (type == 1)
	{
		cost = 100;
		attackCd = 0;
		range = 3;
		damage = 2;
	}
	else if (type == 2)
	{
		cost = 150;
		attackCd = 0;
		range = 4;
		damage = 3;
	}
}

int cTurret::getRotationY()
{
	return rotationY;
}
bool cTurret::AI(map<int,cMonstre> monsters, int pos, int w, int s, float inc)
{
	adquireTarget(monsters, pos, w, s, inc);
	return shootTarget(pos, w);
}
void cTurret::adquireTarget(map<int,cMonstre> monsters, int pos, int w, int s, float inc)
{
	int x = pos%w;
	int z = pos/w;
	target.clear();
	distTarget = -1;
	rotationY = 0;
	if (!monsters.empty())
	{
		std::map<int,cMonstre>::iterator iter;
		for(iter=monsters.begin(); iter != monsters.end(); ++iter){	
			int mpos = iter->second.GetPositionAct();
			if (type == 1) updateTarget(mpos,x,z,w,iter->first);
			else if (type == 2) updateTarget2(mpos,x,z,w,iter->first);
		}

		if (distTarget != -1 && !target.empty()) 
		{
			int mpos = monsters[target[0]].GetPositionAct();
			int dir = monsters[target[0]].GetDir();
			updateRotationY(mpos,x,z,w,s,inc,dir);
		}
	}
}
bool cTurret::shootTarget(int pos, int w)
{
	if (--attackCd <= 0)
	{
		int baseCd = 60;
		if (type == 1) baseCd = 15;
		else if (type == 2) baseCd = 25;
		attackCd = baseCd-2*lvl;
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
		target.clear();
		target.push_back(targetId);
		distTarget = dist;
	}
}
void cTurret::updateTarget2(int mpos, int x, int z, int w, int targetId)
{
	int xm = mpos%w;
	int zm = mpos/w;
	int dist = euclideanDist(x,z,xm,zm);
	if (dist < range)
	{
		target.push_back(targetId);
	}
}
void cTurret::updateRotationY(int mpos, int x, int z, int w, int s, float inc, int dir)
{

	x *= s;
	z *= s;

	float xm = x-(mpos%w)*s;
	float zm = z-(mpos/w)*s;

	if (dir == 1) xm -= inc;
	else if (dir == 2) xm += inc;
	else if (dir == 3) zm -= inc;
	else if (dir == 4) zm += inc;

	float dotTM = z*zm;
	float magTM = sqrt((float)z*z)*sqrt((float)xm*xm+(float)zm*zm);
	float angle = acos((float)dotTM/magTM);
	rotationY = angle*180/M_PI;
	if (xm > 0) rotationY = -rotationY;
}
vector<int> cTurret::getTarget()
{
	return target;
}
int cTurret::getDamage()
{
	return damage+lvl;
}
void cTurret::upgrade()
{
	lvl+=1;
}
int cTurret::getLvl()
{
	return lvl;
}
int cTurret::getType()
{
	return type;
}