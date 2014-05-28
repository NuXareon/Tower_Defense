#pragma once

#include "cData.h"
#include "cMonstre.h"
#include "cTurret.h"
#include "cShot.h"
#include "fmod.hpp"
#include <vector>
#include <iostream>
#include <map>

#define SCENE_WIDTH		16	//#tiles
#define SCENE_DEPTH		16

#define TILE_SIZE		4	//space units

#define FILENAME		"level"
#define MONSTER_FILENAME	"monsters"
#define FILENAME_EXT	".txt"

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	void Init();
	bool LoadLevel(int level);
	bool LoadMonsters(int level);
	void Draw(cData *Data);
	void DrawMonsters(cData *Data,int n, float inc);
	void DrawTurretPanel(cData *Data, int n);
	void DrawLifePanel(cData *Data);
	void DrawUpgradePanel(cData *Data);
	void DrawContainer(cData *Data);
	void DrawInfoPanel(cData *Data);
	void DrawExplosion(cData *Data,int i);
	void DrawShots(cData *Data);
	int *GetMap();
	int *GetMap2();
	void setSelected(int s);
	int getSelected();
	void setMoseOverTile(int s);
	int getMouseOverTile();
	void updateMap(int pos, int value);
	void setDlMonstre(int mons);
	void AI(int *map, int n);
	void turretLogic(float inc);
	void shotLogic(float inc);
	void addTurret(int type, int pos);
	void destroyTurret(int pos);
	std::map<int,cMonstre> GetMonsters();
	void BorraMonstre(int id);
	void treuVida(int id, int v);
	void DeathMonstre(int id);
	void IncExpMonstre(int id);
	int getStart();
	int getEnd();
	int getNumMonstres();
	void upgadeTurret();
	int getSelectedTurretLvl();
	std::map<int,cTurret> GetTowers();
	void pauseMusic();
	void pauseSound();

	int mouseOverTile;

	FMOD::System	*soundSystem;
	FMOD::Sound		*TowerShot1;
	FMOD::Sound		*TowerShot2;
	FMOD::Sound		*MonsterDeath1;
	FMOD::Sound		*MonsterDeath2;
	FMOD::Sound		*MonsterDeath3;
	FMOD::Sound		*Background;
	FMOD::Sound		*TurretExplosion;
	FMOD::Channel	*channel;
	FMOD::Channel	*channelBackground;

private:
	void MakeCubeDL(float w,float h,float d,float tw,float th,float td);
	void MakeFloorDL(float w,float d,float tw,float td);
	void MakeTurretDL(float w,float h,float d);
	void MakeTurretDL2(float w, float h, float d);
	void printTurret(int r, int t);
	void MakeShotDL(float w, float h, float d);
	void addShot(float x, float y, float z, int target, int damage);
	void InitSoundSystem();
	int dl_cube;
	int dl_floor;
	int dl_turret;
	int dl_turret2;
	int dl_monstre;
	int dl_shot;
	int selected;
	int map[SCENE_WIDTH * SCENE_DEPTH];		//scene
	int map2[SCENE_WIDTH * SCENE_DEPTH * TILE_SIZE];	
	std::map<int,cMonstre> monsters;		//monster vector for each level
	std::map<int,cTurret> turrets;			//Turret map, pos->turret
	std::vector<cShot> shots;				//vector de dispars
	int numMonstres;
	int start,end;
	bool soundPaused;
};
