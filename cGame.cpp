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

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1,100);
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
	
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
	xx = x;
	yy = y;
}



//Process
bool cGame::Process()
{
	bool res=true;
	
	//Process Input
	if(keys[27])	res=false;	
	if(keys['1'])	camera = 1;
	if(keys['2'])	camera = 2;
	//Game Logic
	//...

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(camera == 1) gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);
	else if(camera == 2) gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,-1.0,0.0);
	
	//pinta pos
	char buffx[10], buffy[10];
	itoa(xx,buffx,10 );
	itoa(yy,buffy,10 );
	char *s[]={	"Pos X: ", buffx,
				"Pos Y: ", buffy
			  };

	glColor3f(1.0f,1.0f,1.0f);
	
	glDisable(GL_TEXTURE_2D);
		glRasterPos2f(10,10);
		render_string(GLUT_BITMAP_HELVETICA_18,s[0]);
		glRasterPos2f(SCREEN_WIDTH/4 + 30,SCREEN_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_18,s[1]);

		glRasterPos2f(SCREEN_WIDTH/44 + 100,SCREEN_HEIGHT/4);
		render_string(GLUT_BITMAP_HELVETICA_18,s[2]);
		glRasterPos2f(SCREEN_WIDTH/4 + 140,SCREEN_HEIGHT-10);
		render_string(GLUT_BITMAP_HELVETICA_18,s[3]);
	glEnable(GL_TEXTURE_2D);

	glTranslatef(-16.0f,-8.0f,-50.0f);
	glRotatef(60,1.0f,0.0f,0.0f);

	Scene.Draw(&Data);

	glutSwapBuffers();
}

