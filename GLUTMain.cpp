
#include "Globals.h"
#include "cGame.h"
#include "cMenu.h"

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

cGame Game;
cMenu Menu;
bool iniGame;

void AppRender()
{
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	if (Menu.GetMenu() == 2) Game.ReadKeyboard(key,x,y,true);
	else Menu.ReadKeyboard(key,x,y,true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	if (Menu.GetMenu() == 2) Game.ReadKeyboard(key,x,y,false);
	else Menu.ReadKeyboard(key,x,y,false);
}
void AppSpecialKeys(int key, int x, int y)
{
	if (Menu.GetMenu() == 2) Game.ReadKeyboard(key,x,y,true);
	else Menu.ReadKeyboard(key,x,y,true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	if (Menu.GetMenu() == 2) Game.ReadKeyboard(key,x,y,false);
	else Menu.ReadKeyboard(key,x,y,false);
}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button,state,x,y);
}
void AppMouseMovement(int x, int y)
{
	Game.UpdateCursorPosition(x,y);
}

void AppIdle()
{
	if(Menu.GetMenu() == 2){
		if (!iniGame)
		{
			iniGame=true;
			Game.Init();
		}
		if(!Game.Loop()) exit(0);
		if(Game.getDead())
		{
			Menu.Init();
			iniGame = false;
		}
	}
	else{
		if(!Menu.Loop()) exit(0);
	}
}

void main(int argc, char** argv)
{
	int res_x,res_y,pos_x,pos_y;

	//GLUT initialization
	glutInit(&argc, argv);

	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(SCREEN_WIDTH>>1);
	pos_y = (res_y>>1)-(SCREEN_HEIGHT>>1);
	
	glutInitWindowPosition(pos_x,pos_y);
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	glutCreateWindow("Tower Defense");

	/*glutGameModeString("800x600:32");
	glutEnterGameMode();*/

	//Make the default cursor disappear
	//glutSetCursor(GLUT_CURSOR_NONE);

	//Register callback functions
	glutDisplayFunc(AppRender);			
	glutKeyboardFunc(AppKeyboard);		
	glutKeyboardUpFunc(AppKeyboardUp);	
	glutSpecialFunc(AppSpecialKeys);	
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutIdleFunc(AppIdle);
	glutPassiveMotionFunc(AppMouseMovement);

	//Game initializations
	//Game.Init();
	iniGame = false;
	Menu.Init();

	//Application loop
	glutMainLoop();	
}
