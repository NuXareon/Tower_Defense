#include "cMonstre.h"
#include "cScene.h"

cMonstre::cMonstre() {
	pos = 16;
	oldPos = 16;
}
cMonstre::~cMonstre(){}

void cMonstre::Init()
{
	MakeMonstreDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f);
}

void cMonstre::Draw(cData *Data)
{	

	int i,j,x,z;
	float w,d,tw,td;

	glEnable(GL_TEXTURE_2D);
	
	//Monster
	for(i=0;i<SCENE_DEPTH;i++)
	{
		x = 0;
		z = i*TILE_SIZE;

		for(j=0;j<SCENE_WIDTH;j++)
		{
			glPushMatrix();
				glTranslatef(x,0,-z);
				if (i*SCENE_WIDTH+j == pos) // Posició d'on surten els monstres
				{
					glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_ROOF));
					glCallList(dl_monstre);
					glColor3f(1.0f,0.0f,0.0f);
				}
				else glColor3f(1.0f,1.0f,1.0f);
			glPopMatrix();
			x += TILE_SIZE;
		}
	}

	glDisable(GL_TEXTURE_2D);
}

void cMonstre::AI(int *map)
{
	if(map[pos+1] == 0) pos = pos+1;
	else if(map[pos+8] == 0) pos = pos+8;
	else{}

	int x,y;
	GetPosition(&x,&y);

	int v=0;
	//SetPosition(3,3);
}

void cMonstre::MakeMonstreDL(float w,float h,float d,float tw,float th,float td)
{
	dl_monstre = glGenLists(1);
	glNewList(dl_monstre,GL_COMPILE);
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