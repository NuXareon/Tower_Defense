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
	int getRotationY();
	int getTarget();
	bool AI(map<int,cMonstre> monsters, int pos, int w, int s, float inc);
	int getDamage();
	void upgrade();

private:
	int type;
	int dl_turret;
	int cost;
	int lvl;
	int damage;
	int target;
	int distTarget;
	int attackCd;
	int rotationY;
	int range;
	void adquireTarget(map<int,cMonstre> monsters, int pos, int w, int s, float inc);
	bool shootTarget(int pos, int w);
	int euclideanDist(int x1, int y1, int x2, int y2);
	void updateTarget(int mpos, int x, int z, int w, int targetId);
	void updateRotationY(int mpos, int x, int z, int w, int s, float inc, int dir);
};