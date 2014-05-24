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
	void SetID(int i);
	void setMonsterDl(int dl);
	void treuVida(int i);
	float GetVida();
	void  SetVida(float i);
	int GetDir();
	int GetNextPos();

	void Draw(cData *Data,float inc,int *map);
	void ColorVida();
	void Draw2(cData *Data, float inc,int *map);
	int  Direction();
	int  NextMov(int *map);
	int  PosAdj(int *dist,int i);
	void AI(int *map);
	int* BFS(int *map, int pos,int pI,bool b);
	int* inimap(int length, int x);
	void render_string(void* font, const char* string);
	void printPos();	
	void printVida();

	void animacio();

private:
	int MakeMonstreDL(float w,float h,float d,float tw,float th,float td);
	int MakeMonstre2DL(float w,float h,float d,float xo,float yo,float xf,float yf);
	int dl_monstre;
	int dl_monstre2;
	int dl_vida;
	int pi,pf;	// posició inicial d'on surten monstres, posició final
	int oldPos;
	int pos;
	int distAct;
	int distR;
	int distL;
	int distUp;
	int distDown;
	int id;
	int dir;
	float vida;
	int NextPos;
};
