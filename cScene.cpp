#include "cScene.h"
#include "Globals.h"

cScene::cScene(void) {}
cScene::~cScene(void){}

void cScene::Init()
{
	selected = -1;
	mouseOverTile = -1;
	numMonstres = 0;
	soundPaused = false;
	MakeCubeDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f);
	MakeCubeDL2((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f);
	MakeFloorDL((float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f);
	MakeTurretDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE);
	MakeTurretDL2((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE);
	MakeShotDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE);
	
	InitSoundSystem();
	soundSystem->playSound(Background,0,true,&channelBackground);
	channelBackground->setVolume(0.75f);
	channelBackground->setPaused(false);
	soundSystem->update();

}
void cScene::InitSoundSystem()
{
	// FMOD sound API init
	FMOD::System_Create(&soundSystem);

	soundSystem->init(512, FMOD_INIT_NORMAL, 0);

	soundSystem->createSound("audio\\Tower_Shot1.wav",FMOD_DEFAULT,0,&TowerShot1);
	soundSystem->createSound("audio\\Tower_Shot2.wav",FMOD_DEFAULT,0,&TowerShot2);
	soundSystem->createSound("audio\\Monster_Death1.wav",FMOD_DEFAULT,0,&MonsterDeath1);
	soundSystem->createSound("audio\\Monster_Death2.wav",FMOD_DEFAULT,0,&MonsterDeath2);
	soundSystem->createSound("audio\\Monster_Death3.wav",FMOD_DEFAULT,0,&MonsterDeath3);
	soundSystem->createSound("audio\\Explosio1.wav",FMOD_DEFAULT,0,&TurretExplosion);
	soundSystem->createSound("audio\\Shot_Explosion.wav",FMOD_DEFAULT,0,&ShotExplosion);
	soundSystem->createSound("audio\\Background.wav",FMOD_LOOP_NORMAL,0,&Background);
}
void cScene::updateMap(int pos, int value)
{
	map[pos] = value;
}
bool cScene::LoadLevel(int level)
{
	FILE *fd;
	char file[16],tile;
	int i,j;

	if(level<10) sprintf_s(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf_s(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fopen_s(&fd,file,"r");
	if(fd==NULL) return false;

	for(i=SCENE_DEPTH-1;i>=0;i--)
	{
		for(j=0;j<SCENE_WIDTH;j++)
		{
			fscanf_s(fd,"%c",&tile);
			if(tile==' ')
			{
				//Tiles must be != 0 !!!
				map[(i*SCENE_WIDTH)+j]=0;
			}
			else
			{
				//Tiles = 1,2,3,...
				map[(i*SCENE_WIDTH)+j] = tile-48;
			}
		}
		fscanf_s(fd,"%c",&tile); //pass enter
	}

	fclose(fd);

	return true;
}
bool cScene::LoadMonsters(int level) {
	errno_t err;
	bool res;
	FILE *fd;
	char file[16];
	int pi,pf,vida;
	int i;
	int tex;

	res=true;

	if(level<10) sprintf_s(file,"%s0%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf_s(file,"%s%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);

	err=fopen_s(&fd,file,"r");
	if(fd==NULL) return false;

	while(fscanf_s(fd,"%d",&tex) > 0) // read texture (type of monster)
	{ 
		int a1 = fscanf_s(fd,"%d",&vida);	// read vida monstres
		int b1 = fscanf_s(fd,"%d",&pi); // read position inicial
		int c1 = fscanf_s(fd,"%d",&pf); // read y position final
		
		start = pi;
		end = pf;

		cMonstre* b = new cMonstre();
		cMonstre bb = *b;
		bb.SetType(tex);
		bb.SetVida(vida + (grup%10)*3);
		bb.SetPositionI(pi);
		bb.SetPositionF(pf);
		bb.setMonsterDl(dl_monstre);
		bb.SetID(numMonstres);
		bb.setOn(false);
		monsters[numMonstres]= bb;
		numMonstres++;
	
	}

	return res;
}

void cScene::printTurret(int r, int t)
{
	glPushMatrix();
		glTranslatef(TILE_SIZE/2.0f,1.0f+TILE_SIZE,-TILE_SIZE/2.0f);
		glRotatef(r,0.0f,1.0f,0.0f);
		glTranslatef(-TILE_SIZE/2.0f,-1.0f-TILE_SIZE,+TILE_SIZE/2.0f);
		if (t == 1) glCallList(dl_turret);
		else if (t == 2) glCallList(dl_turret2);
	glPopMatrix();
}


void cScene::Draw(cData *Data)
{
	int i,j,x,z;
	int tLvl;

	glEnable(GL_TEXTURE_2D);
	
	//Walls
	for(i=0;i<SCENE_DEPTH;i++)
	{
		x = 0;
		z = i*TILE_SIZE;

		for(j=0;j<SCENE_WIDTH;j++)
		{
			glPushMatrix();
				glTranslatef(x,0,-z);
				glLoadName(i*SCENE_WIDTH+j);	// Name for render_mode(GL_SELECT) = position in map array. (clicking)
				// Set color for selected tile
				if (i*SCENE_WIDTH+j == selected) glColor3f(0.5f,1.0f,0.5f);
				else glColor3f(1.0f,1.0f,1.0f);

				// Pintar tile
				switch(map[(i*SCENE_WIDTH)+j])
				{
					// Terra
					case 0:	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
							glCallList(dl_floor);
							break;
					
					// Walls
					case 1: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL1));
							glCallList(dl_cube2);
							break;
					case 2: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL2));
							glCallList(dl_cube2);
							break;
					case 3: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							glCallList(dl_cube2);
							break;
					case 5:	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FINISH));
							glCallList(dl_floor);
							break;
					// Turret
					case 8: tLvl = turrets[i*SCENE_WIDTH+j].getLvl();
							if (i*SCENE_WIDTH+j != selected) glColor3f(1.0f,1.0f,1.0f-0.33*(tLvl-1));
							glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							printTurret(0,2);
							glColor3f(1.0f,0.2f,0.2f);
							glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
							glCallList(dl_floor);
							glColor3f(1.0f,1.0f,1.0f);
							break;
					case 9: tLvl = turrets[i*SCENE_WIDTH+j].getLvl();
							if (i*SCENE_WIDTH+j != selected) glColor3f(1.0f,1.0f,1.0f-0.33*(tLvl-1));
							glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							int rotate = turrets[i*SCENE_WIDTH+j].getRotationY();
							printTurret(rotate,1);
							glColor3f(1.0f,0.2f,0.2f);
							glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
							glCallList(dl_floor);
							glColor3f(1.0f,1.0f,1.0f);
							break;
				}

			glPopMatrix();
			x += TILE_SIZE;
		}
	}
	glPushMatrix();
	// Print Turret Preview
	if (selected == SCENE_WIDTH*SCENE_DEPTH+1 && mouseOverTile <= SCENE_WIDTH*SCENE_DEPTH && mouseOverTile >= 0 && map[mouseOverTile] == 0)
	{
		x = (mouseOverTile%SCENE_WIDTH)*TILE_SIZE;
		z = (mouseOverTile/SCENE_WIDTH)*TILE_SIZE;
		glTranslatef(x,0,-z);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f,0.50f,1.5f,0.7f);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
		glCallList(dl_turret);
		glDisable(GL_BLEND);
	}
	else if (selected == SCENE_WIDTH*SCENE_DEPTH+2 && mouseOverTile <= SCENE_WIDTH*SCENE_DEPTH && mouseOverTile >= 0 && map[mouseOverTile] == 0)
	{
		x = (mouseOverTile%SCENE_WIDTH)*TILE_SIZE;
		z = (mouseOverTile/SCENE_WIDTH)*TILE_SIZE;
		glTranslatef(x,0,-z);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f,0.50f,1.5f,0.7f);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
		glCallList(dl_turret2);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawExplosion(cData *Data,int id){
	int expAnim = monsters[id].getExpAnim();
	int expAnimDiv = expAnim/5+1;
	int expAnimMod = expAnim%5;
	float xo,xf,yo,yf;
	xo = expAnimMod*0.2;
	yo = expAnimDiv*0.2;
	xf = xo+0.2;
	yf = yo-0.2;
	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_EXPLOSION));
	int pos = monsters[id].GetPositionAct();
	int dir = monsters[id].GetDir();
	int inc = monsters[id].GetInc();
	int i,j,x,z;
	i = pos/SCENE_WIDTH;
	j = pos%SCENE_DEPTH;
	x = j * TILE_SIZE;
	z = i * TILE_SIZE;
	if(dir==1)	glTranslatef(x+inc,0,-z);
	if(dir==2)	glTranslatef(x-inc,0,-z);
	if(dir==3)	glTranslatef(x,0,-z-inc);
	if(dir==4)	glTranslatef(x,0,-z+inc);
	glBegin(GL_QUADS);
		glTexCoord2f(xo,yo); glVertex3f(0, 0,  0);
		glTexCoord2f(xf,yo); glVertex3f(TILE_SIZE, 0,  0);
		glTexCoord2f(xf,yf); glVertex3f(TILE_SIZE, TILE_SIZE,  0);
		glTexCoord2f(xo,yf); glVertex3f(0, TILE_SIZE,  0);
	glEnd();
}
void cScene::DrawShots(cData *Data)
{
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < shots.size(); ++i)
	{
		float x,y,z;
		glPushMatrix();
			shots[i].getCoord(x,y,z);
			glTranslatef(x,y,z);
			if (shots[i].getExplosion()){
				int expAnim = shots[i].getExpAnim();
				int expAnimDiv = expAnim/5+1;
				int expAnimMod = expAnim%5;
				float xo,xf,yo,yf;
				xo = expAnimMod*0.2;
				yo = expAnimDiv*0.2;
				xf = xo+0.2;
				yf = yo-0.2;
				glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_EXPLOSION));
				glTranslatef(0.0f,1.4f,0.2f);
				glRotatef(-45,1.0f,0.0f,0.0f);
				glTranslatef(TILE_SIZE-2,TILE_SIZE-1,0.2f);
				glBegin(GL_QUADS);
					glTexCoord2f(xo,yo); glVertex3f(0, 0,  0);
					glTexCoord2f(xf,yo); glVertex3f(TILE_SIZE/4, 0,  0);
					glTexCoord2f(xf,yf); glVertex3f(TILE_SIZE/4, TILE_SIZE/4,  0);
					glTexCoord2f(xo,yf); glVertex3f(0, TILE_SIZE/4,  0);
				glEnd();
			} else
			{
				glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
				glCallList(dl_shot);
			}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawMonsters(cData *Data, int n,float inc){
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->first <=n && !iter->second.getDeath())	{
			iter->second.setOn(true);
			if(iter->second.GetType()==1) iter->second.Draw(Data,inc,map,IMG_MONSTRE);
			if(iter->second.GetType()==2) iter->second.Draw(Data,inc,map,IMG_MONSTRE2);
			if(iter->second.GetType()==3) iter->second.Draw(Data,inc,map,IMG_MONSTRE3);
		}
	}
}
void cScene::DrawContainer(cData *Data)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL2));
	glColor3f(0.4f,0.4f,0.4f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,   0.0f); glVertex3f(-20, -24.5, -6);
		glTexCoord2f(0.0f,   1.0f); glVertex3f(-20, -16.5, -6);
		glTexCoord2f(1.0f,   1.0f); glVertex3f(20, -16.5, -6);
		glTexCoord2f(1.0f,   0.0f); glVertex3f(20, -24.5, -6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawTurretPanel(cData *Data, int n)
{
	glEnable(GL_TEXTURE_2D);
		glTranslatef(4.0f*(n-1),0.0f,0.0f);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
		glColor3f(0.8f,0.8f,0.8f);
		if (n == 1) glCallList(dl_turret);
		else if (n == 2) glCallList(dl_turret2);

		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
		if(selected == SCENE_WIDTH*SCENE_DEPTH+n) glColor3f(0.3f,1.0f,0.3f);
		else glColor3f(1.0f,0.2f,0.2f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,   0.0f); glVertex3f(0.5, -0.5, -5);
			glTexCoord2f(0.0f,   1.0f); glVertex3f(0.5, 6.5, -5);
			glTexCoord2f(1.0f,   1.0f); glVertex3f(3.5, 6.5, -5);
			glTexCoord2f(1.0f,   0.0f); glVertex3f(3.5, -0.5, -5);
		glEnd();
		glTranslatef(-4.0f*(n-1),0.0f,0.0f);
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawUpgradePanel(cData *Data)
{
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
		glColor3f(0.8f,0.8f,0.8f);
		glCallList(dl_turret);

		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
		glColor3f(1.0f,1.0f,0.6f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,   0.0f); glVertex3f(0.5, -0.5, -5);
			glTexCoord2f(0.0f,   1.0f); glVertex3f(0.5, 6.5, -5);
			glTexCoord2f(1.0f,   1.0f); glVertex3f(3.5, 6.5, -5);
			glTexCoord2f(1.0f,   0.0f); glVertex3f(3.5, -0.5, -5);
		glEnd();

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ARROW));
			glColor4f(1.0f,1.0f,1.0f,0.5f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,   0.0f); glVertex3f(2.5, 6.5, 0);
				glTexCoord2f(0.0f,   1.0f); glVertex3f(2.5, 4.5, 0);
				glTexCoord2f(1.0f,   1.0f); glVertex3f(3.5, 4.5, 0);
				glTexCoord2f(1.0f,   0.0f); glVertex3f(3.5, 6.5, 0);
			glEnd();
		glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawLifePanel(cData *Data){
	glEnable(GL_TEXTURE_2D);
		glTranslatef(30.0f,3.0f,0.0f); 	
		glColor3f(1.0f,1.0f,1.0f);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_COR));
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,   0.0f); glVertex3f(0.5, -0.5, -5);
			glTexCoord2f(0.0f,   1.0f); glVertex3f(0.5, 3.0, -5);
			glTexCoord2f(1.0f,   1.0f); glVertex3f(3.0, 3.0, -5);
			glTexCoord2f(1.0f,   0.0f); glVertex3f(3.0, -0.5, -5);
		glEnd();
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawInfoPanel(cData* Data)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL1));
	glColor3f(0.1f,0.1f,0.1f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,   0.0f); glVertex3f(0.5, -0.5, -5);
		glTexCoord2f(0.0f,   1.0f); glVertex3f(0.5, 6.5, -5);
		glTexCoord2f(1.0f,   1.0f); glVertex3f(9.5, 6.5, -5);
		glTexCoord2f(1.0f,   0.0f); glVertex3f(9.5, -0.5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}
int* cScene::GetMap2()
{
	return map2;
}
void cScene::setDlMonstre(int mons)
{
	dl_monstre = mons;
}
void cScene::MakeCubeDL(float w,float h,float d,float tw,float th,float td)
{
	dl_cube = glGenLists(1);
	glNewList(dl_cube,GL_COMPILE);
		glBegin(GL_QUADS);
			// Front Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(w, 0,  0);
			glTexCoord2f(  tw,   th); glVertex3f(w, h,  0);
			glTexCoord2f(0.0f,   th); glVertex3f(0, h,  0);
			// Back Face
			glTexCoord2f(  tw, 0.0f); glVertex3f(0, 0, -d);
			glTexCoord2f(  tw,   th); glVertex3f(0, h, -d);
			glTexCoord2f(0.0f,   th); glVertex3f(w, h, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0, -d);
			// Right face
			glTexCoord2f(  td, 0.0f); glVertex3f(w, 0, -d);
			glTexCoord2f(  td,   th); glVertex3f(w, h, -d);
			glTexCoord2f(0.0f,   th); glVertex3f(w, h,  0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0,  0);
			// Left Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -d);
			glTexCoord2f(  td, 0.0f); glVertex3f(0, 0,  0);
			glTexCoord2f(  td,   th); glVertex3f(0, h,  0);
			glTexCoord2f(0.0f,   th); glVertex3f(0, h, -d);
			// Top Face
			glTexCoord2f(0.0f,   td); glVertex3f(0, h, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, h,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(w, h,  0);
			glTexCoord2f(  tw,   td); glVertex3f(w, h, -d);
		glEnd();
	glEndList();
}
void cScene::MakeCubeDL2(float w,float h,float d,float tw,float th,float td)
{
	dl_cube2 = glGenLists(1);
	glNewList(dl_cube2,GL_COMPILE);
		glBegin(GL_QUADS);
			// Front Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(w, 0,  0);
			glTexCoord2f(  tw,   th); glVertex3f(w, h/2,  0);
			glTexCoord2f(0.0f,   th); glVertex3f(0, h/2,  0);
			// Back Face
			glTexCoord2f(  tw, 0.0f); glVertex3f(0, 0, -d);
			glTexCoord2f(  tw,   th); glVertex3f(0, h/2, -d);
			glTexCoord2f(0.0f,   th); glVertex3f(w, h/2, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0, -d);
			// Right face
			glTexCoord2f(  td, 0.0f); glVertex3f(w, 0, -d);
			glTexCoord2f(  td,   th); glVertex3f(w, h/2, -d);
			glTexCoord2f(0.0f,   th); glVertex3f(w, h/2,  0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0,  0);
			// Left Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -d);
			glTexCoord2f(  td, 0.0f); glVertex3f(0, 0,  0);
			glTexCoord2f(  td,   th); glVertex3f(0, h/2,  0);
			glTexCoord2f(0.0f,   th); glVertex3f(0, h/2, -d);
			// Top Face
			glTexCoord2f(0.0f,   td); glVertex3f(0, h/2, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, h/2,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(w, h/2,  0);
			glTexCoord2f(  tw,   td); glVertex3f(w, h/2, -d);
		glEnd();
	glEndList();
}

void cScene::MakeFloorDL(float w,float d,float tw,float td)
{
	dl_floor = glGenLists(1);
	glNewList(dl_floor,GL_COMPILE);
		glBegin(GL_QUADS);
			glTexCoord2f(  tw,   td); glVertex3f(0, 0, 0);
			glTexCoord2f(0.0f,   td); glVertex3f(w, 0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0,-d);
			glTexCoord2f(  tw, 0.0f); glVertex3f(0, 0,-d);
		glEnd();
	glEndList();
}

void cScene::MakeTurretDL(float w,float h,float d)
{
	dl_turret = glGenLists(1);
	glNewList(dl_turret,GL_COMPILE);
		glPushMatrix();
			glTranslatef(w/2.0f,1.0f+h,-d/2.0f);
			glRotatef(90,1.0f,0.0f,0.0f);
			GLUquadricObj *q = gluNewQuadric();
			gluQuadricTexture (q, GL_TRUE);
			gluCylinder(q, 0.3,0.95,5,16,16);
			gluSphere(q, 0.8,16,16);
			glRotatef(-90,1.0f,0.0f,0.0f);
			gluCylinder(q, 0.2,0.2,1.5,16,16);
			gluDeleteQuadric(q);
		glPopMatrix();
	glEndList();
}

void cScene::MakeTurretDL2(float w,float h,float d)
{
	dl_turret2 = glGenLists(1);
	glNewList(dl_turret2,GL_COMPILE);
		glPushMatrix();
			glTranslatef(w/2.0f,1.0f+h,-d/2.0f);
			glRotatef(90,1.0f,0.0f,0.0f);
			GLUquadricObj *q = gluNewQuadric();
			gluQuadricTexture (q, GL_TRUE);
			gluCylinder(q, 0.3,0.95,5,16,16);
			gluSphere(q, 0.8,16,16);
			glRotatef(-90,1.0f,0.0f,0.0f);
			gluCylinder(q, 0.2,0.2,1.5,16,16);
			glRotatef(90,0.0f,1.0f,0.0f);
			gluCylinder(q, 0.2,0.2,1.5,16,16);
			glRotatef(90,0.0f,1.0f,0.0f);
			gluCylinder(q, 0.2,0.2,1.5,16,16);
			glRotatef(90,0.0f,1.0f,0.0f);
			gluCylinder(q, 0.2,0.2,1.5,16,16);
			gluDeleteQuadric(q);
		glPopMatrix();
	glEndList();
}

void cScene::MakeShotDL(float w, float h, float d)
{
	dl_shot = glGenLists(1);
	glNewList(dl_shot,GL_COMPILE);
		glPushMatrix();
			glTranslatef(w/2.0f,h,-d/2.0f);
			GLUquadricObj *q = gluNewQuadric();
			gluQuadricTexture (q, GL_TRUE);
			gluSphere(q, 0.4,16,16);
			gluDeleteQuadric(q);
		glPopMatrix();
	glEndList();
}

void cScene::setSelected(int s) 
{
	selected = s;
}
int cScene::getSelected()
{
	return selected;
} 
void cScene::setMoseOverTile(int s)
{
	mouseOverTile = s;
}
int cScene::getMouseOverTile()
{
	return mouseOverTile;
}
void cScene::AI(int *map, int n)
{
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->first <=n && !iter->second.getDeath()){
			if(iter->second.GetType()==1) iter->second.AI(map);
			if(iter->second.GetType()==2) iter->second.AI(map);
			if(iter->second.GetType()==3) iter->second.AI(map);
		}
	}
}
void cScene::turretLogic(float inc)
{
	std::map<int,cTurret>::iterator iter2;
	for(iter2=turrets.begin(); iter2 != turrets.end(); ++iter2){
		bool shoot = iter2->second.AI(monsters, iter2->first, SCENE_WIDTH, TILE_SIZE, inc);	// Assigna un target i la variable Rotation = angle amb el target, per cada torreta, retorna true si pot disparar.
		if (shoot) 
		{
			float x = (iter2->first%SCENE_WIDTH)*TILE_SIZE;
			float y = 0.3*TILE_SIZE;
			float z = -(iter2->first/SCENE_WIDTH*TILE_SIZE);
			vector<int> target = iter2->second.getTarget();
			int damage = iter2->second.getDamage();
			for (unsigned int i = 0; i < target.size(); ++i)
			{
				addShot(x,y,z,target[i],damage);
			}
			if (!target.empty())
			{
				if(!soundPaused)
				{
					if (iter2->second.getType() == 1) 
					{
						soundSystem->playSound(TowerShot2,0,true,&channel);
						channel->setVolume(0.7f);
						channel->setPaused(false);
					}
					else if (iter2->second.getType() == 2) 
					{
						soundSystem->playSound(TowerShot1,0,true,&channel);
						channel->setVolume(0.7f);
						channel->setPaused(false);
					}
					soundSystem->update();
				}
			}
		}
	}
}
bool RemoveShotCondition(cShot s)
{
	return s.getErase();
}
void cScene::shotLogic(float inc)
{
	for(int i = 0; i < shots.size(); ++i)
	{
		if (shots[i].getExplosion())
		{
			shots[i].incExpAnim();
		} else 
		{
			int target = shots[i].getTarget();
			if (monsters.count(target) > 0)
			{
				int mpos = monsters[target].GetPositionAct();
				int dir = monsters[target].GetDir();
				shots[i].IA(mpos, SCENE_WIDTH, TILE_SIZE, dir, inc);
				float x,y,z;
				shots[i].getCoord(x,y,z);
				float xm = (mpos%SCENE_WIDTH)*TILE_SIZE;
				float ym = -0.2*TILE_SIZE;
				float zm = -(mpos/SCENE_WIDTH)*TILE_SIZE;

				if (dir == 1) xm += inc;
				else if (dir == 2) xm -= inc;
				else if (dir == 3) zm -= inc;
				else if (dir == 4) zm += inc;

				if (x == xm && y == ym && z == zm) 
				{
					int dmg = shots[i].getDamage();
					monsters[target].treuVida(dmg);
					shots[i].setExplosion();
					if (!soundPaused)
					{
						soundSystem->playSound(ShotExplosion,0,true,&channel);
						channel->setVolume(0.4f);
						channel->setPaused(false);
					}
				}
			} else
			{
				shots[i].setErase();
			}
		}
	}
	shots.erase(std::remove_if(shots.begin(), shots.end(), RemoveShotCondition),shots.end());
}
void cScene::addShot(float x, float y, float z, int target, int damage)
{
	cShot* s = new cShot();
	cShot ss = *s;
	ss.Init(x,y,z,target, damage);
	shots.push_back(ss);
}
void cScene::addTurret(int type, int pos)
{
	cTurret* t = new cTurret();
	cTurret tt = *t;

	tt.Init(type);
	turrets[pos] = tt;
}
void cScene::destroyTurret(int pos)
{
	if (!soundPaused)
	{
		soundSystem->playSound(TurretExplosion,0,true,&channel);
		channel->setVolume(0.7f);
		channel->setPaused(false);
		soundSystem->update();
	}
	turrets.erase(pos);
}
void cScene::destroyAllTurret()
{
	std::map<int,int> vec;
	std::map<int,cTurret>::iterator iter;
	std::map<int,int>::iterator iter2;
	int i=0;
	for(iter=turrets.begin(); iter != turrets.end(); ++iter){	
		vec[i]=iter->first;
		++i;
	}
	for(iter2=vec.begin(); iter2 != vec.end(); ++iter2) turrets.erase(iter2->second);

}

std::map<int,cMonstre> cScene::GetMonsters()
{
	return monsters;
}
void cScene::BorraMonstre(int id){
	monsters.erase(id);
}
void cScene::treuVida(int id, int v)
{
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){	
		if(iter->first ==id) iter->second.treuVida(v);
	}
}
void cScene::DeathMonstre(int id)
{
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){	
		if(iter->first ==id) 
		{
			if (!soundPaused)
			{
				if (iter->second.GetType() == 1) 
				{
					soundSystem->playSound(MonsterDeath1,0,true,&channel);
					channel->setVolume(0.7f);
					channel->setPaused(false);
				}
				else if (iter->second.GetType() == 2)
				{
					soundSystem->playSound(MonsterDeath2,0,true,&channel);
					channel->setVolume(0.7f);
					channel->setPaused(false);
				}
				else if (iter->second.GetType() == 3) 
				{
					soundSystem->playSound(MonsterDeath3,0,true,&channel);
					channel->setVolume(0.7f);
					channel->setPaused(false);
				}
				soundSystem->update();
			}
			iter->second.setDeath();
			iter->second.setOn(false);
		}
	}
}
void cScene::IncExpMonstre(int id)
{
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){	
		if(iter->first ==id) iter->second.incExpAnim();
	}
}
int cScene::getStart()
{
	return start;
}
int cScene::getEnd()
{
	return end;
}
int cScene::getNumMonstres()
{
	return numMonstres;
}
void cScene::upgadeTurret()
{
	turrets[selected].upgrade();
}
int cScene::getSelectedTurretLvl()
{
	return turrets[selected].getLvl();
}
map<int,cTurret> cScene::GetTowers()
{
	return turrets;
}
void cScene::pauseMusic()
{
	bool paused;
	channelBackground->getPaused(&paused);
	channelBackground->setPaused(!paused);
	soundSystem->update();
}
void cScene::pauseSound()
{
	channel->getMute(&soundPaused);
	soundPaused=!soundPaused;
	channel->setMute(soundPaused);
	soundSystem->update();
}
void cScene::setGrup(int i){
	grup = i;
}
void cScene::BorraAllMonster()
{
	monsters.clear();
}