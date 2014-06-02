#pragma once

#include "cScene.h"
#include "cData.h"
#include "cBicho.h"
#include "cMonstre.h"
#include "fmod.hpp"

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define TIME_FRAME		28	//40 FPS
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define SELECT_BUF_SIZE	16
#define TOWER_ID_1		9
#define TOWER_ID_2		8
#define COST_TURRET_1	100
#define COST_TURRET_2	150
#define COST_UPGRADE_1	150
#define COST_UPGRADE_2	200
#define CD_IA_M			16
#define CD_MONSTER		10
#define MONSTER_GOLD	50
#define MONSTER_GOLD2	75

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

	void render_string(void* font, const char* string);
	void printCursorPosition();
	void UpdateCursorPosition(int x, int y);
	GLuint SelectCursorTile(int x, int y, GLuint (*buff)[SELECT_BUF_SIZE]);
	void printSelectedTile();
	void printUI();
	int getSelected();
	int GetVida();
	bool hihaCami(int* map,int pi,int pf);
	bool TurretNextPosM(int p);
	bool getDead();

private:
	unsigned char keys[256];
	cScene Scene;
	cData Data;
	int camera;
	int xx,yy;
	bool showUI;
	bool debug,releaseF1;

	cMonstre Monstre;
	int cdAi;
	int cdCursorTile;

	int gold;
	int vidasP;
	int numM;	//numero de monstres "instanciats"
	int cdSpawnM;

	void printGameInfo();
	void printTurretInfo(int t);
	void printTurretInfo2(int t);
	void printUpgradeInfo();
	void printTurretBadPos();
	void printTurretNoGold();

	float inc;

	bool pause;
	int cdBadPos;
	int cdNoGold;
	int lvl;
	int numgrups;
	int grup;
	bool dead;
	int cdlvl;
};
