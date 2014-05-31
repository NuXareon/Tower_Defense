#pragma once

#include "cScene.h"
#include "cData.h"

#include <stdio.h>
#include <stdlib.h>


#define SPECIAL_KEY_OFFSET 100 //Offset que sumem a les tecles especial (fletxes, F1, etc...) per evitar solapament amb caracters ASCII

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

#define ARROW_PLAYER1	2
#define ARROW_INSTUC	1
#define ARROW_CREDITS	0

class cMenu
{
public:
	cMenu(void);
	virtual ~cMenu(void);

	bool Init();
	bool Loop();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadKeyboardSpecial(unsigned char key, int x, int y, bool press);

	//Process
	bool Process();

	//Output
	void Render();
	void DrawImg(int tex_id, int x, int y, int h, int w);
	void DrawImg2(int tex_id, int x, int y, int h, int w);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);
	void DrawTurret();

	int GetMenu();
	void SetMenu(int x);

private:
	unsigned char keys[256];
	cData Data;
	cScene Scene;
	int x,y;
	int w,h;
	int posArrow;
	int cd;
	int menu;
};