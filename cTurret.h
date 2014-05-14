#pragma once

#include "cData.h"
#include "cMonstre.h"
#include <iostream>
#include <map>

class cTurret
{
public:
	cTurret(void);
	virtual ~cTurret(void);
	void Init(int type);
	void adquireTarget(int* monsters);
	int getRotationY();
	void AI(map<int,cMonstre> monsters, int pos, int w);

private:
	int type;
	int dl_turret;
	int cost;
	int lvl;
	int target;
	int distTarget;
	int attackCd;
	int rotationY;
	int range;
	int euclideanDist(int x1, int y1, int x2, int y2);
};