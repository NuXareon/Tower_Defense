#include "cGame.h"
#include "cTurret.h"
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
	cdCursorTile = 5;
	cdAi = 3;
	gold = 1000;
	vidasP = 5;
	cdSpawnM = 3;
	numM = 0;
	inc = 0;
	pause = false;
	cdBadPos = 0;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1,200);	// Si es modifica s'ha de modificar tambe a la funcio PrintCursorTile()
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
	res = Data.LoadImage(IMG_COR,"cor.png",GL_RGBA);
	if(!res) return false;
	Scene.Init();
	int dl_mon = Monstre.Init();
	Scene.setDlMonstre(dl_mon);
	res = Scene.LoadLevel(1);
	if(!res) return false;
	res = Scene.LoadMonsters(1);
	if(!res) return false;

	return res;
}

bool cGame::Loop()
{
	int initialTime = glutGet(GLUT_ELAPSED_TIME);
	bool res=true;
	if (!pause){
		res = Process();
		if(res) Render();
	}
	int finalTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = finalTime-initialTime;
	if (deltaTime < TIME_FRAME) 
	{
		Sleep(TIME_FRAME-deltaTime);
	}
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
	if (key == 'p' && press) pause=!pause;
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
			if(hits != -1 && Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+1 && Scene.GetMap()[buff[3]] == 0 && gold >= COST_TURRET_1) 
			{
				int pi = Scene.getStart();
				int pf = Scene.getEnd();
				int *map = Scene.GetMap();
				map[buff[3]]=TOWER_ID_1;
				if(hihaCami(map,pi,pf) && TurretNextPosM(buff[3])){ //hi ha cami && no es on anira algun monstre
					Scene.updateMap(buff[3],TOWER_ID_1);
					Scene.addTurret(1,buff[3]);
					gold -= COST_TURRET_1;
				}
				else{
					map[buff[3]]=0;
					cdBadPos = 10;	// temps de printar printTurretBadPos
				}
			}
			Scene.setSelected(buff[3]);
		}
	}
	if 	((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		GLuint buff[SELECT_BUF_SIZE] = {0};
		GLuint hits = SelectCursorTile(x,SCREEN_HEIGHT-y,&buff);
		if (hits > 0) 
		{
			if(Scene.GetMap()[buff[3]] == TOWER_ID_1) 
			{
				Scene.updateMap(buff[3],0);
				Scene.destroyTurret(buff[3]);
				gold += COST_TURRET_1/2;
			}
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
	bool z=false;
	
	//Process Input
	if(keys[27])	res=false;	
	if(keys['1'])	camera = 1;
	if(keys['2'])	camera = 2;
	if(keys['3'])	camera = 3;
	if(keys['4'])	camera = 4;
	if(keys['z'])   z = true;
	// F1 to show debug info.
	if(keys[GLUT_KEY_F1] && releaseF1)	
	{
		debug=!debug;
		releaseF1 = false;
	}

	//Game Logic
	//...

	if (cdCursorTile > 0) cdCursorTile--;
	else {
		cdCursorTile = 3;
		GLuint buff[SELECT_BUF_SIZE] = {0};
		GLuint hits = SelectCursorTile(xx,SCREEN_HEIGHT-yy,&buff);
		if (hits == 0) Scene.setMoseOverTile(-1);
		else Scene.setMoseOverTile(buff[3]);
	}

	
	if(cdAi>0){
		cdAi--;
		inc += (float)TILE_SIZE/(float)CD_IA_M;
	}
	else {
		cdAi=CD_IA_M;
		if(cdSpawnM == 0){
			if(Scene.getNumMonstres()>numM){
				numM++;
				cdSpawnM = 3;
			}
		}
		else cdSpawnM--;
		inc = 0;
		Scene.AI(Scene.GetMap(),numM);
	}
	
	//
	std::map<int,cMonstre> monsters = Scene.GetMonsters();
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->second.GetVida() <= 0){	// si el monstre no te vida borra
			gold += 20;
			Scene.BorraMonstre(iter->first);
		}
		if(iter->second.GetPositionF() == iter->second.GetPositionAct()){	//si arriba al final treu vidaPlayer i borra
			vidasP--;
			Scene.BorraMonstre(iter->first);
		}
		if(z) {			// Per veure si funciona treure vida a un monstre. 
			Scene.treuVida(0,1);
			z = false;
		}
	}

	Scene.turretLogic(inc);

	Scene.shotLogic(inc);

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
	//Scene.DrawLifePanel(&Data);

	glTranslatef(29.4f,0.0f,0.0f);

	Scene.DrawInfoPanel(&Data);

	printGameInfo();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void cGame::printGameInfo()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buffg[10], buffl[10];
	_itoa_s(gold,buffg,10 );
	_itoa_s(vidasP,buffl,10 );
	char *s[]={	"Gold: ", buffg,
				"Life: ", buffl
				};

	glColor3f(1.0f,1.0f,1.0f);

	glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(0.45f,-0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(0.60f,-0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);

			glRasterPos2f(0.45f,-0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(0.60f,-0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);
		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

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
	_itoa_s(xx,buffx,10 );
	_itoa_s(yy,buffy,10 );
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
	//int selected = Scene.getSelected();
	int selected = Scene.mouseOverTile;
	_itoa_s(selected,buff,10 );
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

void cGame::printTurretInfo()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buffg[10];
	_itoa_s(COST_TURRET_1,buffg,10 );
	char *s[]={	"Turret type 1",
				"Cost: ", buffg
				};

	glColor3f(1.0f,1.0f,1.0f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.85f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.95f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);
			glRasterPos2f(-0.80f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);

		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void cGame::printTurretBadPos()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char *s[]={	"I can't build here!"
				};

	glColor3f(1.0f,1.0f,1.0f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.85f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);

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

	glSelectBuffer(SELECT_BUF_SIZE,*buff);

	glGetIntegerv(GL_VIEWPORT,view);

	glRenderMode(GL_SELECT);

	glInitNames();

	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		gluPickMatrix(x,y,1.0,1.0,view);
		gluPerspective(45.0,(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1,200);

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
	
	//glTranslatef(-25.0f,-20.0f,-80.0f); // 16*16
	glTranslatef(-16.0f,-10.0f,-38.0f); // 8*8
	glRotatef(60,1.0f,0.0f,0.0f);

	Scene.Draw(&Data);
	//Monstre.Draw(&Data);
	Scene.DrawMonsters(&Data,numM,inc);
	Scene.DrawShots(&Data);

	if (debug) 
	{
		printCursorPosition();
		printSelectedTile();
	}

	if (showUI) 
	{
		printUI();
		if (Scene.getMouseOverTile() == SCENE_WIDTH*SCENE_DEPTH+1 || Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+1) printTurretInfo();
	}
	if( cdBadPos>0)
	{
		printTurretBadPos();
		--cdBadPos;
	}

	glutSwapBuffers();
}

int cGame::GetVida(){
	return vidasP;
}

bool cGame::hihaCami(int* map,int pi,int pf)
{
	bool b;
	int *dist =Monstre.BFS(map,pf,pi);
	int d = dist[pi];
	if(dist[pi]>= SCENE_WIDTH*SCENE_DEPTH) b=false;
	else b=true;
	return b;
}
bool cGame::TurretNextPosM(int p)
{
	bool b = true;
	std::map<int,cMonstre> monsters = Scene.GetMonsters();
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->second.GetNextPos() == p){	
			b = false;
		}
	}
	return b;
}