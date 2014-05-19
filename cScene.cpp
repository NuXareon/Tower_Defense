#include "cScene.h"
#include "Globals.h"

cScene::cScene(void) {}
cScene::~cScene(void){}

void cScene::Init()
{
	selected = -1;
	mouseOverTile = -1;
	MakeCubeDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f);
	MakeFloorDL((float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f);
	MakeTurretDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE);
	MakeShotDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE);
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
	int pi,pf;
	int i;
	int tex;

	res=true;

	if(level<10) sprintf_s(file,"%s0%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf_s(file,"%s%d%s",(char *)MONSTER_FILENAME,level,(char *)FILENAME_EXT);

	err=fopen_s(&fd,file,"r");
	if(fd==NULL) return false;

	while(fscanf_s(fd,"%d",&tex) > 0) // read texture (type of monster)
	{ 
		int a1 = fscanf_s(fd,"%d",&numMonstres);	// read num monstres
		int b1 = fscanf_s(fd,"%d",&pi); // read position inicial
		int c1 = fscanf_s(fd,"%d",&pf); // read y position final
		
		start = pi;
		end = pf;

		cMonstre* b = new cMonstre();
		cMonstre bb = *b;
		//bb.SetType(tex);
		bb.SetPositionI(pi);
		bb.SetPositionF(pf);
		bb.setMonsterDl(dl_monstre);
		for(i=0;i<numMonstres;++i){
			bb.SetID(i);
			monsters[i]= bb;
		}
	}

	return res;
}

void cScene::printTurret(int r)
{
	glPushMatrix();
		glTranslatef(TILE_SIZE/2.0f,1.0f+TILE_SIZE,-TILE_SIZE/2.0f);
		glRotatef(r,0.0f,1.0f,0.0f);
		glTranslatef(-TILE_SIZE/2.0f,-1.0f-TILE_SIZE,+TILE_SIZE/2.0f);
		glCallList(dl_turret);
	glPopMatrix();
}


void cScene::Draw(cData *Data)
{
	int i,j,x,z;

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
				// Print Turret Preview
				if (i*SCENE_WIDTH+j == mouseOverTile && selected == SCENE_WIDTH*SCENE_DEPTH+1 && map[i*SCENE_WIDTH+j] == 0)
				{
					glColor3f(0.5f,0.50f,1.5f);
					glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
					glCallList(dl_turret);
				}
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
							glCallList(dl_cube);
							break;
					case 2: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL2));
							glCallList(dl_cube);
							break;
					case 3: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							glCallList(dl_cube);
							break;
					// Turret
					case 9: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							int rotate = turrets[i*SCENE_WIDTH+j].getRotationY();
							printTurret(rotate);
							glColor3f(1.0f,0.2f,0.2f);
							glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
							glCallList(dl_floor);
							glColor3f(1.0f,1.0f,1.0f);
				}
			glPopMatrix();
			x += TILE_SIZE;
		}
	}
	glDisable(GL_TEXTURE_2D);
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
			glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
			glCallList(dl_shot);
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawMonsters(cData *Data, int n,float inc){
	std::map<int,cMonstre>::iterator iter;
	for(iter=monsters.begin(); iter != monsters.end(); ++iter){
		if(iter->first <=n)	iter->second.Draw2(Data,inc,map);
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
void cScene::DrawTurretPanel(cData *Data)
{
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
		glColor3f(0.8f,0.8f,0.8f);
		glCallList(dl_turret);
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
		if(selected == SCENE_WIDTH*SCENE_DEPTH+1) glColor3f(0.3f,1.0f,0.3f);
		else glColor3f(1.0f,0.2f,0.2f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,   0.0f); glVertex3f(0.5, -0.5, -5);
			glTexCoord2f(0.0f,   1.0f); glVertex3f(0.5, 6.5, -5);
			glTexCoord2f(1.0f,   1.0f); glVertex3f(3.5, 6.5, -5);
			glTexCoord2f(1.0f,   0.0f); glVertex3f(3.5, -0.5, -5);
		glEnd();
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
			// Bottom Face
			/*glTexCoord2f(  tw,   td); glVertex3f(0, 0, -d);
			glTexCoord2f(0.0f,   td); glVertex3f(w, 0, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(0, 0,  0);*/
			// Top Face
			glTexCoord2f(0.0f,   td); glVertex3f(0, h, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, h,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(w, h,  0);
			glTexCoord2f(  tw,   td); glVertex3f(w, h, -d);
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
		if(iter->first <=n) iter->second.AI(map);
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
			int target = iter2->second.getTarget();
			addShot(x,y,z,target);
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
			if (x == xm && y == ym && z == zm) monsters[target].treuVida(2);
		} else
		{
			shots[i].setErase();
		}
	}
	shots.erase(std::remove_if(shots.begin(), shots.end(), RemoveShotCondition),shots.end());
}
void cScene::addShot(float x, float y, float z, int target)
{
	cShot* s = new cShot();
	cShot ss = *s;
	ss.Init(x,y,z,target);
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
	turrets.erase(pos);
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
int cScene::getStart()
{
	return start;
}
int cScene::getEnd()
{
	return end;
}