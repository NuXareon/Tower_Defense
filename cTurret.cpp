#include "cTurret.h"
#include "Globals.h"

cTurret::cTurret(void){}
cTurret::~cTurret(void){}

void cTurret::Init(int t)
{
	type = t;
	cost = 100;
	lvl = 1;
}