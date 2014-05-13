#pragma once

#include "cData.h"
#include "cMonstre.h"
#include "cTurret.h"
#include <vector>
#include <iostream>
#include <map>

#define SCENE_WIDTH		8	//#tiles
#define SCENE_DEPTH		8

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
	void DrawMonsters(cData *Data,int n);
	void DrawTurretPanel(cData *Data);
	void DrawLifePanel(cData *Data);
	void DrawContainer(cData *Data);
	void DrawInfoPanel(cData *Data);
	int *GetMap();
	void setSelected(int s);
	int getSelected();
	void setMoseOverTile(int s);
	int getMouseOverTile();
	void updateMap(int pos, int value);
	void setDlMonstre(int mons);
	void AI(int *map, int n);
	void addTurret(int type, int pos);
	void destroyTurret(int pos);
	std::map<int,cMonstre> GetMonsters();
	//cMonstre GetMonsters(int i);
	void BorraMonstre(int i);

	int mouseOverTile;
	
	

private:
	void MakeCubeDL(float w,float h,float d,float tw,float th,float td);
	void MakeFloorDL(float w,float d,float tw,float td);
	void MakeTurretDL(float w,float h,float d);
	int dl_cube;
	int dl_floor;
	int dl_turret;
	int dl_monstre;
	int selected;
	int map[SCENE_WIDTH * SCENE_DEPTH];		//scene
	std::map<int,cMonstre> monsters;		//monster vector for each level
	std::map<int,cTurret> turrets;			//Turret map, pos->turret
	int numMonstres;
	
};
