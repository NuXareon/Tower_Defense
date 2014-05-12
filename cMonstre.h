#pragma once

#include "cBicho.h"
#include "cData.h"
#include <stdlib.h>
#include <time.h>

#include <queue>
using namespace std;

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

	int Init();
	void SetPositionI(int p);
	void GetPositionI(int *p);
	void SetPositionF(int p);
	int GetPositionF();
	int GetPositionAct();
	void SetErase(bool b);
	int GetErase();

	void Draw(cData *Data);
	void AI(int *map);
	int* BFS(int *map, int pos,int pI);
	int* inimap(int length, int x);
	void render_string(void* font, const char* string);
	void printPos();
	void setMonsterDl(int dl);

	

private:
	int MakeMonstreDL(float w,float h,float d,float tw,float th,float td);
	void MakeMonstre2DL(float w,float h,float d,float tw,float th,float td);
	int dl_monstre;
	int dl_monstre2;
	int pi,pf;	// posició inicial d'on surten monstres, posició final
	int elimina;
	int oldPos;
	int pos;
	int posFi;
	int distAct;
	int distR;
	int distL;
	int distUp;
	int distDown;
	
	bool borra;

};
