#pragma once

#include "cData.h"

#define SPEED_FACTOR	2/3

class cShot
{
public:
	cShot(void);
	virtual ~cShot(void);
	void Init(float x, float y, float z, int target);
	void getCoord(float &xcoord, float &ycoord, float &zcoord);
	void IA(int mpos, int w, int s, int dir, float inc);

	int getTarget();
	void setErase();
	bool getErase();

private:
	float x,y,z;
	int target;
	bool erase;
};