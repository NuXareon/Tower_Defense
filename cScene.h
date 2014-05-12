#pragma once

#include "cData.h"
#include "cMonstre.h"
#include <vector>

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
	void DrawMonsters(cData *Data);
	void DrawTurretPanel(cData *Data);
	void DrawContainer(cData *Data);
	int *GetMap();
	void setSelected(int s);
	int getSelected();
	void setMoseOverTile(int s);
	void updateMap(int pos, int value);
	void setDlMonstre(int mons);
	void AI(int *map);

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
	std::vector<cMonstre> monsters;			//monster vector for each level
	int numMonstres;
	
};
