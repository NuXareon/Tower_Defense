#pragma once

#include "cData.h"

class cTurret
{
public:
	cTurret(void);
	virtual ~cTurret(void);
	void Init(int type);

private:
	int type;
	int dl_turret;
	int cost;
	int lvl;
};