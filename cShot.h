#pragma once

#include "cData.h"

#define SPEED_FACTOR	2/3

class cShot
{
public:
	cShot(void);
	virtual ~cShot(void);
	void Init(float x, float y, float z, int target, int damage);
	void Init(float x, float y, float z, int target, int damage, bool freeze);
	void getCoord(float &xcoord, float &ycoord, float &zcoord);
	void IA(int mpos, int w, int s, int dir, float inc);

	int getTarget();
	void setErase();
	void setExplosion();
	bool getErase();
	int getExpAnim();
	bool getExplosion();
	void incExpAnim();
	int getDamage();
	bool getFreeze();

private:
	float x,y,z;
	int target;
	bool erase;
	bool explosion;
	int expAnim;
	int damage;
	bool freeze;
};