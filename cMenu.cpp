#include "cMenu.h"
#include "Globals.h"


cMenu::cMenu(void)
{
}

cMenu::~cMenu(void)
{
}


bool cMenu::Init()
{
	bool res=true;
	posArrow = 2;
	cd = 0;
	menu = -1;

	res = Data.LoadImage(IMG_MENU_CREDITS,"credits.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ARROW_MENU,"menu_arrow.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_CREDITS,"Credits.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_INSTUC,"instructions.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MENU,"menu.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTRE,"robot2iceaxe.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTRE2,"WarriorRobot.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTRE3,"SimpleRobot.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_WALL3,"wall3.png",GL_RGBA);
	if(!res) return false;

	return res;
}

void cMenu::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}


bool cMenu::Process()
{	
	bool res=true;

	if(keys[27] && cd==0){
		if(menu == 0 || menu == 1) menu = -1;
		else res=false;
		cd = 10;
	}
	if(keys[GLUT_KEY_UP] && cd==0){
		posArrow = (posArrow+1)%3;
		cd = 10;
	}
	if(keys[GLUT_KEY_DOWN] && cd==0){
		posArrow = (posArrow+2)%3;
		cd = 10;
	}
	if(keys[13]){ //intro
		
		if(posArrow == ARROW_PLAYER1)	menu = ARROW_PLAYER1;
		if(posArrow == ARROW_INSTUC)	menu = ARROW_INSTUC;
		if(posArrow == ARROW_CREDITS)	menu = ARROW_CREDITS;

	}

	return res;
}


bool cMenu::Loop()
{
	bool res=true;

	res = Process();
	glPushMatrix();
	if(res) Render();
	glPopMatrix();

	if(cd>0) cd -= 1;

	return res;
}

void cMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-25, 25, -25, 25, 0, 10);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);

	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	if(menu == -1){
		// Random sprites
		x = -20;
		y = 0;
		h = 15;
		w = 10;
		DrawImg2(Data.GetID(IMG_MONSTRE),x,y,h,w);

		x = -3;
		y = 0;
		h = 10;
		w = 5;
		DrawImg2(Data.GetID(IMG_MONSTRE2),x,y,h,w);

		x = 10;
		y = 0;
		h = 10;
		w = 5;
		DrawImg2(Data.GetID(IMG_MONSTRE3),x,y,h,w);

		DrawTurret();

		//Menu
		x = -24;
		y = -24;
		h = 48;
		w = 48;	
		DrawImg(Data.GetID(IMG_MENU),x,y,h,w);
		
		//Arrow
		x = -10;
		y = -22+6*posArrow;
		h = 5;
		w = 5;	
		DrawImg(Data.GetID(IMG_ARROW_MENU),x,y,h,w);
		
	}
	if(menu == ARROW_CREDITS){	//credits
		x = -20;
		y = -20;
		h = 40;
		w = 40;
		DrawImg(Data.GetID(IMG_CREDITS),x,y,h,w);
	}
	if(menu == ARROW_INSTUC){	//instruction
		x = -24;
		y = -24;
		h = 48;
		w = 48;
		DrawImg(Data.GetID(IMG_INSTUC),x,y,h,w);
	}
	
	glutSwapBuffers();
}


void cMenu::DrawImg(int tex_id, int x, int y, int h, int w)
{
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f,1.0f);	glVertex2i(x  ,y);
		glTexCoord2f(1.0f,1.0f);	glVertex2i(x+w,y);
		glTexCoord2f(1.0f,0.0f);	glVertex2i(x+w,y+h);
		glTexCoord2f(0.0f,0.0f);	glVertex2i(x  ,y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void cMenu::DrawImg2(int tex_id, int x, int y, int h, int w)
{
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f,0.25f);	glVertex2i(x  ,y);
		glTexCoord2f(0.25f,0.25f);	glVertex2i(x+w,y);
		glTexCoord2f(0.25f,0.0f);	glVertex2i(x+w,y+h);
		glTexCoord2f(0.0f,0.0f);	glVertex2i(x  ,y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void cMenu::DrawTurret()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_WALL3));
	glPushMatrix();
		glTranslatef(-17,-7,0);
		glRotatef(90,1.0f,0.0f,0.0f);
		GLUquadricObj *q = gluNewQuadric();
		gluQuadricTexture (q, GL_TRUE);
		gluCylinder(q, 0.7,2.5,16,16,16);
		gluSphere(q, 2.5,16,16);
		gluDeleteQuadric(q);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

int cMenu::GetMenu()
{
	return menu;
}

void cMenu::SetMenu(int x)
{
	menu = x;
}