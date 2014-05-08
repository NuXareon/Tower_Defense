#include "cMonstre.h"
#include "cScene.h"


cMonstre::cMonstre() {
	pos = 16;
	oldPos = 16;
	posFi = 61;
	distAct = -1;
	distR = -1;
	distL = -1;
	distUp = -1;
	distDown = -1;
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
				if (i*SCENE_WIDTH+j == pos) // Posici� d'on surten els monstres
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
	//printPos();
	glDisable(GL_TEXTURE_2D);
}

void cMonstre::AI(int *map)
{
	int *dist = BFS(map,posFi,pos);
	//pos adjacent mes propera
	oldPos = pos;
	int aux = pos+1;
	int dmin = dist[pos+1];
	distAct = dist[pos];
	distR = dist[pos+1];
	distL = dist[pos-1];
	distUp = dist[pos+SCENE_WIDTH];
	distDown = dist[pos-SCENE_WIDTH];
	if(dmin > dist[pos+1]&& ((aux+1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)+1 ) {dmin = dist[pos+1]; aux = pos+1;}
	if(dmin > dist[pos-1]&& ((aux-1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)-1 ) {dmin = dist[pos-1]; aux = pos-1;}
	if(dmin > dist[pos+SCENE_WIDTH] && aux+SCENE_WIDTH<64) {dmin = dist[pos+SCENE_WIDTH]; aux = pos+SCENE_WIDTH;}
	if(dmin > dist[pos-SCENE_WIDTH] && aux+SCENE_WIDTH<-1) {dmin = dist[pos-SCENE_WIDTH]; aux = pos-SCENE_WIDTH;}

	pos = aux;

	/*if(map[pos+1] == 0) pos = pos+1;
	else if(map[pos+8] == 0) pos = pos+8;
	else{}*/

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

int* cMonstre::BFS(int *map, int pF,int pI){
	int dist[SCENE_WIDTH * SCENE_DEPTH];
	for(int i=0;i< SCENE_WIDTH * SCENE_DEPTH;++i){
		dist[i]=SCENE_WIDTH * SCENE_DEPTH+1;
	}
	int mapbool[SCENE_WIDTH * SCENE_DEPTH];
	for(int i=0;i< SCENE_WIDTH * SCENE_DEPTH;++i){
		mapbool[i]=-1;
	}

	queue<int> q;
	q.push(pF);
	dist[pF] = 0;
	while(!q.empty()){
		int aux = q.front();
		q.pop();

		//if(aux == pI) return dist;
		if(map[aux] == 0 && mapbool[aux] < 3){
			mapbool[aux] += 1;
			//en creu
			if(map[aux+1]==0 && ((aux+1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)+1 ) {
				q.push(aux+1); 
				if(dist[aux+1]>dist[aux]+1) dist[aux+1]=dist[aux]+1;
			}
			if(map[aux-1]==0 && ((aux-1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)-1) {
				q.push(aux-1);
				if(dist[aux-1]>dist[aux]+1)dist[aux-1]=dist[aux]+1;
			}
			if(map[aux+SCENE_WIDTH]==0 && aux+SCENE_WIDTH<64 ) {
				q.push(aux+SCENE_WIDTH); 
				if(dist[aux+SCENE_WIDTH]>dist[aux]+1)dist[aux+SCENE_WIDTH]=dist[aux]+1;
			}
			if(map[aux-SCENE_WIDTH]==0 && aux-SCENE_WIDTH>-1) {
				q.push(aux-SCENE_WIDTH);
				if(dist[aux-SCENE_WIDTH]>dist[aux]+1) dist[aux-SCENE_WIDTH]=dist[aux]+1;
			}
		}
	}
	return dist;
}

void cMonstre::render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}
void cMonstre::printPos()
{	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buffx[10], buffy[10];
	char buffa[10], buffb[10],buffc[10], buffd[10],buffe[10];
	itoa(pos,buffx,10 );
	itoa(oldPos,buffy,10 );
	itoa(distAct,buffa,10 );
	itoa(distR,buffb,10 );
	itoa(distL,buffb,10 );
	itoa(distUp,buffb,10 );
	itoa(distDown,buffb,10 );
	char *s[]={	"PosN: ", buffx,
				"PosO: ", buffy,
				buffa,buffb,buffc,buffd,buffe
				};

	glColor3f(1.0f,1.0f,1.0f);

	glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			glRasterPos2f(-0.95f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			glRasterPos2f(-0.80f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);

			glRasterPos2f(-0.95f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[2]);
			glRasterPos2f(-0.80f,0.75f);
			render_string(GLUT_BITMAP_9_BY_15,s[3]);

			glRasterPos2f(-0.95f,0.70f);
			render_string(GLUT_BITMAP_9_BY_15,s[4]);
			glRasterPos2f(-0.80f,0.70f);
			render_string(GLUT_BITMAP_9_BY_15,s[7]);
			glRasterPos2f(-0.95f,0.65f);
			render_string(GLUT_BITMAP_9_BY_15,s[6]);
			glRasterPos2f(-0.80f,0.65f);
			render_string(GLUT_BITMAP_9_BY_15,s[5]);
			glRasterPos2f(-0.95f,0.60f);
			render_string(GLUT_BITMAP_9_BY_15,s[8]);
		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}