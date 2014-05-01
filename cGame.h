#pragma once

#include "cScene.h"
#include "cData.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

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

private:
	unsigned char keys[256];
	cScene Scene;
	cData Data;
	int camera;
	int xx,yy;
};
