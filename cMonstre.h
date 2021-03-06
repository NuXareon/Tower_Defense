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
	int GetPositionI();
	void SetPositionF(int p);
	int GetPositionF();
	int GetPositionAct();
	void SetID(int i);
	void setMonsterDl(int dl);
	void treuVida(int i);
	float GetVida();
	void SetVida(float i);
	int GetDir();
	int GetInc();
	int GetNextPos();

	void Draw(cData *Data,float inc,int *map, int img);
	void ColorVida();
	void Draw2(cData *Data, float inc,int *map);
	int  Direction();
	int  NextMov(int *map);
	//int  PosAdj(int *dist,int i);
	void AI(int *map);
	int* BFS(int *map, int pos);
	int* inimap(int length, int x);
	void render_string(void* font, const char* string);
	void printPos();	
	void printVida();
	void incExpAnim();
	int  getExpAnim();
	bool getDeath();
	void setDeath();
	void setOn(bool b);
	bool getOn();
	void setFreezeCd(int f);
	int getFreezeCd();
	void decreaseFreezeCd();
	int getNoFreeze();
	void setNoFreeze(int nf);
	void setMaxVida(int i);

	void animacio();

private:
	int MakeMonstreDL(float w,float h,float d,float tw,float th,float td);
	int MakeMonstre2DL(float w,float h,float d,float xo,float yo,float xf,float yf);
	int dl_monstre;
	int dl_monstre2;
	int dl_vida;
	int pi,pf;	// posici� inicial d'on surten monstres, posici� final
	int oldPos;
	int pos;
	int distAct;
	int distR;
	int distL;
	int distUp;
	int distDown;
	int id;
	int dir;
	int inc;
	float vida;
	int NextPos;
	int expAnim;
	bool death;
	bool on;
	int freezeCd;
	int noFreeze;
	int maxVides;
};
