#pragma once

#include "cTexture.h"
#include "Globals.h"
#include <vector>

#define FRAME_DELAY		8
#define STEP_LENGTH		2
#define ROLL_STEP		12
#define JUMP_HEIGHT		96
#define JUMP_STEP		4


#define STATE_WALKLEFT		0
#define STATE_WALKRIGHT		1
#define STATE_WALKUP		2
#define STATE_WALKDOWN		3
#define STATE_EXPLOSION		4

#define STATE_DEATH			16
#define STATE_DEATH_FOC		17

#define STATE_ATACL			18
#define STATE_ATACR			19

#define STATE_ROLLINGL		20
#define STATE_ROLLINGR		21

#define SHOT_CD				15
#define MAX_ROLL_COUNT		4

class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);
	void SetType(int i);
	int GetType();
	/*
	void SetShotCd(int cd);
	void GetShotCd(int *cd);
	int GetRollCount();
	void DisableBuff(int id);
	void EnableBuff(int id);
	bool GetBuffStatus(int id);*/

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	bool CollidesMapFloor(int *map);
	
	void GetArea(cRect *rc);
	/*void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void RollLeft(int *map);
	void RollRight(int *map);
	void Jump(int *map);
	void Stop(int *map);*/
	void Logic(int *map);

	int  GetState();
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();
	void ResetFrame();

private:
	int x,y;
	int w,h;
	int state;
	int type;	

	bool jumping;
	int jump_alfa;
	int jump_y;
	bool caure;

	int seq,delay;

	int shot_cd;
	int roll_count;

	bool buff_speed;
	bool buff_power;
};
