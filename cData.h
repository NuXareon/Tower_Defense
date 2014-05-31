#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG		17

//Image identifiers
#define IMG_WALL1	0
#define IMG_WALL2	1
#define IMG_WALL3	2
#define IMG_FLOOR	3
#define IMG_ROOF	4
#define IMG_FINISH	5
#define IMG_MONSTRE 6
#define IMG_MONSTRE2 7
#define IMG_MONSTRE3 8
#define IMG_COR		9
#define IMG_EXPLOSION	10
#define IMG_ARROW	11
#define IMG_MENU_CREDITS	12
#define IMG_ARROW_MENU	13
#define IMG_CREDITS	14
#define IMG_INSTUC	15
#define IMG_MENU	16


class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	bool LoadImage(int img,char *filename,int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};
