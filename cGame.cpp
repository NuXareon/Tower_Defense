#include "cGame.h"
#include "Globals.h"


cGame::cGame(void) {}
cGame::~cGame(void){}

void cGame::render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}

bool cGame::Init()
{
	bool res=true;
	camera = 1;
	debug=false;
	releaseF1=true;
	showUI = true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1,100);	// Si es modifica s'ha de modificar tambe a la funcio PrintCursorTile()
	glMatrixMode(GL_MODELVIEW);
	
	glEnable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_WALL1,"wall1.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_WALL2,"wall2.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_WALL3,"wall3.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_FLOOR,"floor.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ROOF,"roof.png",GL_RGBA);
	if(!res) return false;
	Scene.Init();
	Monstre.Init();
	res = Scene.LoadLevel(1);
	if(!res) return false;

	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
	if (key == GLUT_KEY_F1 && !press) releaseF1 = true;
	if (key == 't' && press) Scene.setSelected(SCENE_WIDTH*SCENE_DEPTH+1);
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
	if 	((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		GLuint buff[SELECT_BUF_SIZE] = {0};
		GLuint hits = SelectCursorTile(x,SCREEN_HEIGHT-y,&buff);
		if (hits == 0) Scene.setSelected(-1);
		else
		{
			if(hits != -1 && Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+1 && Scene.GetMap()[buff[3]] == 0) Scene.updateMap(buff[3],TOWER_ID_1);
			Scene.setSelected(buff[3]);
		}
	}
	if 	((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		GLuint buff[SELECT_BUF_SIZE] = {0};
		GLuint hits = SelectCursorTile(x,SCREEN_HEIGHT-y,&buff);
		if (hits > 0) 
		{
			if(Scene.GetMap()[buff[3]] == TOWER_ID_1) Scene.updateMap(buff[3],0);
			Scene.setSelected(-1);
		}
	}
}

void cGame::UpdateCursorPosition(int x, int y)
{
	xx = x;
	yy = y;
	/*
	GLuint buff[SELECT_BUF_SIZE] = {0};
	GLuint hits = SelectCursorTile(xx,SCREEN_HEIGHT-yy,&buff);
	if (hits <= 0) Scene.setMoseOverTile(-1);
	else Scene.setMoseOverTile(buff[3]);
	*/
}

//Process
bool cGame::Process()
{
	bool res=true;
	
	//Process Input
	if(keys[27])	res=false;	
	if(keys['1'])	camera = 1;
	if(keys['2'])	camera = 2;
	if(keys['3'])	camera = 3;
	if(keys['4'])	camera = 4;

	// F1 to show debug info.
	if(keys[GLUT_KEY_F1] && releaseF1)	
	{
		debug=!debug;
		releaseF1 = false;
	}

	//Game Logic
	//...
	 
	Monstre.AI(Scene.GetMap());


	return res;
}


void cGame::printUI()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(-25, 25, -25, 25, 0, 10);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	Scene.DrawContainer(&Data);

	glTranslatef(-19.0f,-23.5f,0.0f);

	Scene.DrawTurretPanel(&Data);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
}

void cGame::printCursorPosition()
{	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buffx[10], buffy[10];
	itoa(xx,buffx,10 );
	itoa(yy,buffy,10 );
	char *s[]={	"Pos X: ", buffx,
				"Pos Y: ", buffy
				};

	glColor3f(1.0f,1.0f,1.0f);

	glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.95f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.80f,0.95f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);

			glRasterPos2f(-0.95f,0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(-0.80f,0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);
		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void cGame::printSelectedTile()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buff[10];
	int selected = Scene.getSelected();
	itoa(selected,buff,10 );
	char *s[]={	"Selected: ", buff
				};

	glColor3f(1.0f,1.0f,1.0f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.85f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.75f,0.85f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);

		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLuint cGame::SelectCursorTile(int x, int y, GLuint (*buff)[SELECT_BUF_SIZE])
{
	GLint hits, view[4];
	int id;

	glSelectBuffer(SELECT_BUF_SIZE,*buff);

	glGetIntegerv(GL_VIEWPORT,view);

	glRenderMode(GL_SELECT);

	glInitNames();

	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		gluPickMatrix(x,y,1.0,1.0,view);
		gluPerspective(45.0,(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1,100);

		glMatrixMode(GL_MODELVIEW);

		bool tmpDebug = debug;
		debug = false;
		showUI = false;
		Render();
		showUI = true;
		debug = tmpDebug;

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	hits = glRenderMode(GL_RENDER);

	if (hits == 0)
	{
		//glOrtho(-25, 25, -25, 25, 0, 10);
		float posx = -25.0 + (float)x/SCREEN_WIDTH * 50;
		float posy = -25.0 + (float)y/SCREEN_HEIGHT * 50;
		if(posx >= -18.5 && posx <= -15.5 && posy >= -24.0 && posy <= -17.0) 
		{
			(*buff)[3] = SCENE_WIDTH*SCENE_DEPTH+1;
			hits = -1;
		}
		xx = posx;
		yy = posy;
	}

	return hits;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(camera == 1) gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);
	else if(camera == 2) gluLookAt(0.0,50.0,-45.0, 0.0,0.0,-45.0, 0.0,0.0,-1.0);
	else if(camera == 3) gluLookAt(40.0,0.0,-20.0, 0.0,0.0,-40.0, 0.0,1.0,0.0);
	else if(camera == 4) gluLookAt(-40.0,0.0,-20.0, 0.0,0.0,-40.0, 0.0,1.0,0.0);

	glTranslatef(-16.0f,-10.0f,-38.0f);
	glRotatef(60,1.0f,0.0f,0.0f);

	Scene.Draw(&Data);
	Monstre.Draw(&Data);

	if (debug) 
	{
		printCursorPosition();
		printSelectedTile();
	}

	if (showUI) printUI();
	
	glutSwapBuffers();
}