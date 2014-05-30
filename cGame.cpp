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
	gold = 500;
	vidasP = 5;
	cdSpawnM = 3;
	numM = 0;
	inc = 0;
	pause = false;
	cdBadPos = cdNoGold = 0;
	lvl=1;
	grup = 1;	Scene.setGrup(grup);
	numgrups = 3;

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
	res = Data.LoadImage(IMG_FINISH,"Finish256.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_FLOOR,"floor.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ROOF,"roof.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTRE,"robot2iceaxe.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTRE2,"WarriorRobot.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MONSTRE3,"SimpleRobot.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_COR,"cor.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_EXPLOSION,"explosion512-2.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ARROW,"arrow.png",GL_RGBA);
	if(!res) return false;
	Scene.Init();
	int dl_mon = Monstre.Init();
	Scene.setDlMonstre(dl_mon);
	res = Scene.LoadLevel(lvl);
	if(!res) return false;
	res = Scene.LoadMonsters(grup);
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
	if (key == 'y' && press) Scene.setSelected(SCENE_WIDTH*SCENE_DEPTH+2);
	if (key == 'm' && press) Scene.pauseMusic();
	if (key == 's' && press) Scene.pauseSound();
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
					Scene.setSelected(buff[3]);
				}
				else{
					map[buff[3]]=0;
					cdBadPos = 15;	// temps de printar printTurretBadPos
				}
			} else if(hits != -1 && Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+2 && Scene.GetMap()[buff[3]] == 0 && gold >= COST_TURRET_2) 
			{
				int pi = Scene.getStart();
				int pf = Scene.getEnd();
				int *map = Scene.GetMap();
				map[buff[3]]=TOWER_ID_2;
				if(hihaCami(map,pi,pf) && TurretNextPosM(buff[3])){ //hi ha cami && no es on anira algun monstre
					Scene.updateMap(buff[3],TOWER_ID_2);
					Scene.addTurret(2,buff[3]);
					gold -= COST_TURRET_2;
					Scene.setSelected(buff[3]);
				}
				else{
					map[buff[3]]=0;
					cdBadPos = 15;	// temps de printar printTurretBadPos
				}
			}
			else if (gold < COST_TURRET_1 && Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+1) cdNoGold = 15;
			else if (gold < COST_TURRET_1 && Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+2) cdNoGold = 15;
			else if(Scene.GetMap()[Scene.getSelected()] == TOWER_ID_1 && buff[3] == SCENE_WIDTH*SCENE_DEPTH+10)
			{
				if (gold>= COST_UPGRADE_1+50*(Scene.getSelectedTurretLvl()-1)) 
				{					
					gold -= COST_UPGRADE_1+50*(Scene.getSelectedTurretLvl()-1);
					Scene.upgadeTurret();
				}
				else cdNoGold = 15;
			} else if(Scene.GetMap()[Scene.getSelected()] == TOWER_ID_2 && buff[3] == SCENE_WIDTH*SCENE_DEPTH+10)
			{
				if (gold>= COST_UPGRADE_1+50*(Scene.getSelectedTurretLvl()-1)) 
				{
					gold -= COST_UPGRADE_1+50*(Scene.getSelectedTurretLvl()-1);
					Scene.upgadeTurret();
				}
				else cdNoGold = 15;
			} 
			else {
				Scene.setSelected(buff[3]);
			}
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
			else if(Scene.GetMap()[buff[3]] == TOWER_ID_2)
			{
				Scene.updateMap(buff[3],0);
				Scene.destroyTurret(buff[3]);
				gold += COST_TURRET_2/2;
			}
			Scene.setSelected(-1);
		}
	}
}

void cGame::UpdateCursorPosition(int x, int y)
{
	xx = x;
	yy = y;
}

//Process
bool cGame::Process()
{
	bool res=true;
	bool z=false;
	bool saltalvl=false;
	//Process Input
	if(keys[27])	res=false;	
	if(keys['1'])	camera = 1;
	if(keys['2'])	camera = 2;
	if(keys['3'])	camera = 3;
	if(keys['4'])	camera = 4;
	if(keys['!'])   saltalvl = true;
	if(keys['z'])   z = true;
	// F1 to show debug info.
	if(keys[GLUT_KEY_F1] && releaseF1)	
	{
		debug=!debug;
		releaseF1 = false;
	}

	//Game Logic
	//...
	/*if(saltalvl){
			Scene.BorraAllMonster();
			saltalvl = false;
		}*/

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
	int *map = Scene.GetMap();
	std::map<int,cMonstre> monsters = Scene.GetMonsters();
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->second.GetVida() <= 0 && !iter->second.getDeath()){	// si el monstre no te vida borra
			gold += MONSTER_GOLD;
			//Scene.BorraMonstre(iter->first);
			Scene.DeathMonstre(iter->first);
		}
		if(iter->second.GetPositionF() == iter->second.GetPositionAct()){	//si arriba al final treu vidaPlayer i borra
			if(iter->second.GetType()==1) vidasP -=1;
			if(iter->second.GetType()==2) vidasP -=2;
			Scene.BorraMonstre(iter->first);
		}
		if(z) {			// Per veure si funciona treure vida a un monstre. 
			Scene.treuVida(0,1);
			z = false;
		}
		if((map[iter->second.GetPositionAct()]==9 || map[iter->second.GetPositionAct()]==8)&& iter->second.GetType()==3){ //destroy turret and monster
			//Scene.BorraMonstre(iter->first);
			Scene.DeathMonstre(iter->first);
			Scene.destroyTurret(iter->second.GetPositionAct());
			Scene.updateMap(iter->second.GetPositionAct(),0);
		}

		if(iter->second.getDeath()==true){	// amimacio explosio monstre
			if(iter->second.getExpAnim()==25){
				Scene.BorraMonstre(iter->first);
			}
			else Scene.IncExpMonstre(iter->first);
		}
	}
	//Segent grup
	if(Scene.GetMonsters().size() == 0){
		--numgrups;
		++grup;	Scene.setGrup(grup);
		Scene.LoadMonsters(grup);
	}

	//Panasr de nivell
	if(numgrups==0){	// per superar nivells automaticament		
		++lvl;
		if(grup<10){
			grup = 11;
		}
		else if(grup<20) grup =21;
			Scene.setGrup(grup);	
		numgrups = 3;
		Scene.LoadLevel(lvl);
		Scene.LoadMonsters(grup);
		if(gold<500) gold = 500;
		Scene.destroyAllTurret();
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

	Scene.DrawTurretPanel(&Data, 1);
	Scene.DrawTurretPanel(&Data, 2);
	//Scene.DrawLifePanel(&Data);

	glTranslatef(29.4f,0.0f,0.0f);

	Scene.DrawInfoPanel(&Data);

	printGameInfo();

	if (Scene.GetMap()[Scene.getSelected()] == 9 || Scene.GetMap()[Scene.getSelected()] == 8)
	{
		glTranslatef(-4.0f,0.0f,0.0f);
		Scene.DrawUpgradePanel(&Data);
	}
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

	char buffg[10], buffl[10], bufft[10], buffm[10], buffn[10];
	_itoa_s(gold,buffg,10 );
	_itoa_s(vidasP,buffl,10 );
	_itoa_s(Scene.GetTowers().size(),bufft,10 );
	_itoa_s(Scene.GetMonsters().size(),buffm,10 );
	_itoa_s(numgrups,buffn,10 );
	char *s[]={	"Gold: ", buffg,
				"Life: ", buffl,
				"Towers: ", bufft,
				"Monters: ", buffm,
				"Groups: ",buffn 
				};

	glColor3f(1.0f,1.0f,1.0f);

	glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(0.45f,-0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(0.65f,-0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);

			glRasterPos2f(0.45f,-0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(0.65f,-0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);

			glRasterPos2f(0.45f,-0.85f);
			render_string(GLUT_BITMAP_9_BY_15,s[4]);
			glRasterPos2f(0.65f,-0.85f);
			render_string(GLUT_BITMAP_9_BY_15,s[5]);

			glRasterPos2f(0.45f,-0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[6]);
			glRasterPos2f(0.65f,-0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[7]);

			glRasterPos2f(0.45f,-0.95f);
			render_string(GLUT_BITMAP_9_BY_15,s[8]);
			glRasterPos2f(0.65f,-0.95f);
			render_string(GLUT_BITMAP_9_BY_15,s[9]);
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

void cGame::printTurretInfo(int t)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buffg[10],buffd[10], buffas[10], buffr[10];
	char* aoe;
	if (t == 1) 
	{
		_itoa_s(COST_TURRET_1,buffg,10 );
		_itoa_s(3+1,buffd,10 );
		_gcvt(1.035+0.124,3,buffas);
		_itoa_s(3,buffr,10 );
		aoe = "Single-target";
	}
	else if (t == 2) 
	{
		_itoa_s(COST_TURRET_2,buffg,10 );
		_itoa_s(5+1,buffd,10 );
		_gcvt(0.652+0.124,4,buffas);
		_itoa_s(3,buffr,10 );
		aoe = "Multi-target";
	}
	char *s[]={	"Turret type 1",
				"Cost: ", buffg,
				aoe,
				"Range :", buffr,
				"Damage: ", buffd,
				"Attack Speed: ", buffas
				};

	glColor3f(1.0f,1.0f,1.0f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.95f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);
			glRasterPos2f(-0.78f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(-0.95f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);
			glRasterPos2f(-0.95f,0.70f);
			render_string(GLUT_BITMAP_9_BY_15,s[4]);
			glRasterPos2f(-0.78f,0.70f);
			render_string(GLUT_BITMAP_9_BY_15,s[5]);
			glRasterPos2f(-0.95f,0.65f);
			render_string(GLUT_BITMAP_9_BY_15,s[6]);
			glRasterPos2f(-0.78f,0.65f);
			render_string(GLUT_BITMAP_9_BY_15,s[7]);
			glRasterPos2f(-0.95f,0.60f);
			render_string(GLUT_BITMAP_9_BY_15,s[8]);
			glRasterPos2f(-0.95f,0.55f);
			render_string(GLUT_BITMAP_9_BY_15,s[9]);
		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void cGame::printTurretInfo2(int t)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	int lvl = Scene.getSelectedTurretLvl();
	char bufflvl[10],buffd[10],buffas[10];
	_itoa_s(lvl,bufflvl,10 );
	if (t == 1)
	{
		_itoa_s(3+lvl,buffd,10 );
		_gcvt(1.035+0.124*lvl,3,buffas);
	} else if (t == 2)
	{
		_itoa_s(5+lvl,buffd,10 );
		_gcvt(0.652+0.124*lvl,3,buffas);
	}
	char *s[]={	"Turret type 1",
				"Lvl: ", bufflvl,
				"Damage: ", buffd,
				"Attack Speed: ", buffas
				};

	glColor3f(1.0f,1.0f,1.0f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.95f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);
			glRasterPos2f(-0.78f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(-0.95f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);
			glRasterPos2f(-0.78f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[4]);
			glRasterPos2f(-0.95f,0.70f);
			render_string(GLUT_BITMAP_9_BY_15,s[5]);
			glRasterPos2f(-0.95f,0.65f);
			render_string(GLUT_BITMAP_9_BY_15,s[6]);

		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void cGame::printUpgradeInfo()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	int lvl = Scene.getSelectedTurretLvl();
	char bufflvl[10],bufflvlp[10],buffup[10];
	_itoa_s(lvl,bufflvl,10 );
	_itoa_s(lvl+1,bufflvlp,10 );
	_itoa_s(COST_UPGRADE_1+50*(lvl-1),buffup,10 );
	char *s[]={	"Upgrade Lvl",
				bufflvl, " -> ", bufflvlp,
				"Cost: ", buffup,
				"Damage: +1",
				"Attack Speed:", "+0.124"
				};

	glColor3f(1.0f,1.0f,1.0f);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.90f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.95f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);
			glRasterPos2f(-0.90f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(-0.80f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);
			glRasterPos2f(-0.95f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[4]);
			glRasterPos2f(-0.83f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[5]);
			glRasterPos2f(-0.95f,0.70f);
			render_string(GLUT_BITMAP_9_BY_15,s[6]);
			glRasterPos2f(-0.95f,0.65f);
			render_string(GLUT_BITMAP_9_BY_15,s[7]);
			glRasterPos2f(-0.95f,0.60f);
			render_string(GLUT_BITMAP_9_BY_15,s[8]);
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

void cGame::printTurretNoGold()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char *s[]={	"Not enought gold!"
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
		else if (posx >= -14.5 && posx <= -11.5 && posy >= -24.0 && posy <= -17.0){
			(*buff)[3] = SCENE_WIDTH*SCENE_DEPTH+2;
			hits = -1;
		}
		else if (posx >= 7.0 && posx <= 10.0 && posy >= -24.0 && posy <= -17.0){
			(*buff)[3] = SCENE_WIDTH*SCENE_DEPTH+10;
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
	
	glTranslatef(-25.0f,-20.0f,-80.0f); // 16*16
	//glTranslatef(-16.0f,-10.0f,-38.0f); // 8*8
	glRotatef(60,1.0f,0.0f,0.0f);

	
	// Pintem en ordre diferent depenent de si es el render final o el de clicking per fer que el clicking i les transparencies funcionin correctament.
	if (showUI)
	{
		Scene.DrawMonsters(&Data,numM,inc);
		Scene.DrawShots(&Data);
		Scene.Draw(&Data);
	}
	else {
		Scene.Draw(&Data);
		Scene.DrawMonsters(&Data,numM,inc);
		Scene.DrawShots(&Data);
	}

	if (debug) 
	{
		printCursorPosition();
		printSelectedTile();
	}

	if (showUI) 
	{
		printUI();
		if (cdNoGold>0)
		{
			printTurretNoGold();
			--cdNoGold;
		}
		else if(cdBadPos>0)
		{
			printTurretBadPos();
			--cdBadPos;
		} 
		else if (Scene.getMouseOverTile() == SCENE_WIDTH*SCENE_DEPTH+10 && (Scene.GetMap()[Scene.getSelected()] == TOWER_ID_1 || Scene.GetMap()[Scene.getSelected()] == TOWER_ID_2)) printUpgradeInfo();
		else if (Scene.getMouseOverTile() == SCENE_WIDTH*SCENE_DEPTH+1 || Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+1) printTurretInfo(1);
		else if (Scene.getMouseOverTile() == SCENE_WIDTH*SCENE_DEPTH+2 || Scene.getSelected() == SCENE_WIDTH*SCENE_DEPTH+2) printTurretInfo(2);
		else if (Scene.GetMap()[Scene.getSelected()] == 9) printTurretInfo2(1); 
		else if (Scene.GetMap()[Scene.getSelected()] == 8) printTurretInfo2(2); 
	}

	std::map<int,cMonstre> monsters = Scene.GetMonsters();
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->second.getDeath()) Scene.DrawExplosion(&Data,iter->first);
	}

	glutSwapBuffers();
}

int cGame::GetVida(){
	return vidasP;
}

bool cGame::hihaCami(int* map,int pi,int pf)
{
	bool b = true;
	std::map<int,cMonstre> monsters = Scene.GetMonsters();
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		int *dist =iter->second.BFS(map,pf,pi);
		int d = dist[pi];
		if(dist[pi]>= SCENE_WIDTH*SCENE_DEPTH){
			b=false;
		}
	}
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
