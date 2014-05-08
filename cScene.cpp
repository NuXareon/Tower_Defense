#include "cScene.h"
#include "Globals.h"

cScene::cScene(void) {}
cScene::~cScene(void){}

void cScene::Init()
{
	selected = -1;
	MakeCubeDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f);
	MakeFloorDL((float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f);
	MakeTurretDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE);
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

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	for(i=SCENE_DEPTH-1;i>=0;i--)
	{
		for(j=0;j<SCENE_WIDTH;j++)
		{
			fscanf(fd,"%c",&tile);
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
		fscanf(fd,"%c",&tile); //pass enter
	}

	fclose(fd);

	return true;
}
void cScene::Draw(cData *Data)
{
	int i,j,x,z;
	float w,d,tw,td;

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
				if (i*SCENE_WIDTH+j == selected) 
				{
					if (map[(i*SCENE_WIDTH)+j] == 0)
					{
						glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
						glCallList(dl_turret);
						map[(i*SCENE_WIDTH)+j] = 5;
					}
					glColor3f(0.5f,1.0f,0.5f);
				}
				else glColor3f(1.0f,1.0f,1.0f);

				glLoadName(i*SCENE_WIDTH+j);	// Name for render_mode(GL_SELECT) = position in map array. (clicking)
				switch(map[(i*SCENE_WIDTH)+j])
				{
					case 0:	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
							glCallList(dl_floor);
							break;
					case 1: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL1));
							glCallList(dl_cube);
							break;
					case 2: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL2));
							glCallList(dl_cube);
							break;
					case 3: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							glCallList(dl_cube);
							break;
					case 9: glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL3));
							glCallList(dl_turret);
							glColor3f(1.0f,0.2f,0.2f);
							glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
							glCallList(dl_floor);
							glColor3f(1.0f,1.0f,1.0f);
				}
			glPopMatrix();
			x += TILE_SIZE;
		}
	}
	/*
	tw = (float)SCENE_WIDTH;
	td = (float)SCENE_DEPTH;
	w  = (float)SCENE_WIDTH*TILE_SIZE;
	d  = (float)SCENE_DEPTH*TILE_SIZE;
	*/
	//Floor
	/*
	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_FLOOR));
	glBegin(GL_QUADS);
		// Bottom Face
		glTexCoord2f(  tw,   td); glVertex3f(0, 0, 0);
		glTexCoord2f(0.0f,   td); glVertex3f(w, 0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(w, 0,-d);
		glTexCoord2f(  tw, 0.0f); glVertex3f(0, 0,-d);
	glEnd();
	*/
	//Roof
	/*glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
	glBegin(GL_QUADS);
		// Top Face
		glTexCoord2f(0.0f,   td); glVertex3f(0, TILE_SIZE, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0, TILE_SIZE,-d);
		glTexCoord2f(  tw, 0.0f); glVertex3f(w, TILE_SIZE,-d);
		glTexCoord2f(  tw,   td); glVertex3f(w, TILE_SIZE, 0);
	glEnd();*/

	glDisable(GL_TEXTURE_2D);
}
void cScene::DrawContainer(cData *Data)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_WALL1));
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
	glColor3f(1.0f,0.2f,0.2f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,   0.0f); glVertex3f(0.5, -0.5, -5);
		glTexCoord2f(0.0f,   1.0f); glVertex3f(0.5, 6.5, -5);
		glTexCoord2f(1.0f,   1.0f); glVertex3f(3.5, 6.5, -5);
		glTexCoord2f(1.0f,   0.0f); glVertex3f(3.5, -0.5, -5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
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

void cScene::setSelected(int s) 
{
	selected = s;
}
int cScene::getSelected()
{
	return selected;
} 
