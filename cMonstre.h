#pragma once

#include "cBicho.h"
#include "cData.h"
#include <stdlib.h>
#include <time.h>


#define MONSTRE_START_CX		3
#define MONSTRE_START_CY		2
#define MONSTER_HP				100
#define MONSTER_REGEN_CD		30
#define MONSTER_REGEN_AMOUNT	10

class cMonstre: public cBicho
{
public:
	cMonstre();
	~cMonstre();

	void Init();
	void Draw(cData *Data);
	void AI(int *map);
	

private:
	void MakeMonstreDL(float w,float h,float d,float tw,float th,float td);
	int dl_monstre;
	int oldPos;
	int pos;

};