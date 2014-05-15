#pragma once

#include "cData.h"

class cShot
{
public:
	cShot(void);
	virtual ~cShot(void);
	void Init(int x, int y, int z, int target);
	void getCoord(int &xcoord, int &ycoord, int &zcoord);

private:
	int x,y,z;
	int target;
};