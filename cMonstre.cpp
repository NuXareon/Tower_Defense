#include "cMonstre.h"
#include "cScene.h"


cMonstre::cMonstre() {
	pos = 16;
	oldPos = 16;
	distAct = -1;
	distR = -1;
	distL = -1;
	distUp = -1;
	distDown = -1;
	id = -1;
	vida = 10;
	death = false;
	expAnim=0;
}
cMonstre::~cMonstre(){}

int cMonstre::Init()
{
	//return MakeMonstreDL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f);
	return MakeMonstre2DL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,1.0f,1.0f,1.0f,1.0f);
}

void cMonstre::SetPositionI(int p)
{
	pi = p;
	pos = p;
	NextPos = p+1;
}
void cMonstre::GetPositionI(int *p)
{
	*p = pi;
}
void cMonstre::SetPositionF(int p)
{
	pf = p;

}
int cMonstre::GetPositionF()
{
	return pf;
}
int cMonstre::GetPositionAct()
{
	return pos;
}
void cMonstre::SetID(int i)
{
	id = i;
}
float cMonstre::GetVida()
{
	return vida;
}
void cMonstre::SetVida(float i)
{
	vida = i;
}

void cMonstre::setMonsterDl(int dl)
{
	dl_monstre=dl;
}
void cMonstre::treuVida(int i){
	vida -= i;
}
void cMonstre::Draw(cData *Data,float inc,int *map, int img)
{	

	int i,j,x,z;

	glEnable(GL_TEXTURE_2D);
	i = pos/8;
	j = pos%8;
	x = j * TILE_SIZE;
	z = i * TILE_SIZE;
	glPushMatrix();
		//int dir = NextMov(map);
		int dir = Direction();
		if(dir==1)	glTranslatef(x+inc,0,-z);
		if(dir==2)	glTranslatef(x-inc,0,-z);
		if(dir==3)	glTranslatef(x,0,-z-inc);
		if(dir==4)	glTranslatef(x,0,-z+inc);
		//glTranslatef(x,0,-z);
		ColorVida();
		glBindTexture(GL_TEXTURE_2D,Data->GetID(img));
		animacio();
		glCallList(dl_monstre2);
	glPopMatrix();
}
void cMonstre::ColorVida()
{
	float fv = float(vida/10);
	glColor3f(1.0f,fv,fv);

}

void cMonstre::Draw2(cData *Data,float inc,int *map)
{
	int i,j,x,z;

	glEnable(GL_TEXTURE_2D);
	i = pos/8;
	j = pos%8;
	x = j * TILE_SIZE;
	z = i * TILE_SIZE;
	glPushMatrix();
		//int dir = NextMov(map);
		int dir = Direction();
		if(dir==1)	glTranslatef(x+inc,0,-z);
		if(dir==2)	glTranslatef(x-inc,0,-z);
		if(dir==3)	glTranslatef(x,0,-z-inc);
		if(dir==4)	glTranslatef(x,0,-z+inc);
		//glTranslatef(x,0,-z);
		ColorVida();
		glBindTexture(GL_TEXTURE_2D,Data->GetID(IMG_MONSTRE2));
		animacio();
		glCallList(dl_monstre2);
	glPopMatrix();
}
int cMonstre::Direction(){
	int dir;
	if(pos+1 == NextPos){
		dir = 1;
		SetState(STATE_WALKLEFT);
	}
	else if(pos-1 == NextPos){
		dir = 2;
		SetState(STATE_WALKRIGHT);
	}
	else if(pos+SCENE_WIDTH == NextPos){
		dir = 3;
		SetState(STATE_WALKUP);
	}
	else if(pos-SCENE_WIDTH == NextPos){
		dir = 4;
		SetState(STATE_WALKDOWN);
	}
	else dir=5;//error
	return dir;
}
int cMonstre::NextMov(int *map)
{
	dir;	// 1=R, 2=L, 3=Up, 4=Down
	int *dist = BFS(map,pf,pos);
	oldPos = pos;
	int aux = pos;
	int dmin = dist[pos];
	distAct = dist[pos];
	distR = dist[pos+1];
	distL = dist[pos-1];
	distUp = dist[pos+SCENE_WIDTH];
	distDown = dist[pos-SCENE_WIDTH];

	if(dmin > distR && ((aux+1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)+1 ) // R
	{
		dmin = dist[pos+1]; 
		aux = pos+1;
		dir = 1;
	} 	
	if(dmin > distL && ((aux-1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)-1 ) // L
	{
		dmin = dist[pos-1]; 
		aux = pos-1;
		dir = 2;
	} 
	if(dmin > distUp && aux+SCENE_WIDTH<64) //UP
	{
		dmin = dist[pos+SCENE_WIDTH]; 
		aux = pos+SCENE_WIDTH;
		dir = 3;
	}
	if(dmin > distDown && aux+SCENE_WIDTH>-1) //DOWN
	{
		dmin = dist[pos-SCENE_WIDTH]; 
		aux = pos-SCENE_WIDTH;
		dir = 4;
	}
	return aux;
}
int cMonstre::PosAdj(int *dist, int i)
{
	oldPos = pos;
	int aux = pos;
	int dmin = dist[pos];
	distAct = dist[pos];
	distR = dist[pos+1];
	distL = dist[pos-1];
	distUp = dist[pos+SCENE_WIDTH];
	distDown = dist[pos-SCENE_WIDTH];

	if(dmin > distR && ((aux+1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)+1 ) // R
	{
		dmin = dist[pos+1]; 
		aux = pos+1;
	} 	
	if(dmin > distL && ((aux-1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)-1 ) // L
	{
		dmin = dist[pos-1]; 
		aux = pos-1;
	} 
	if(dmin > distUp && aux+SCENE_WIDTH<64) //UP
	{
		dmin = dist[pos+SCENE_WIDTH]; 
		aux = pos+SCENE_WIDTH;
	}
	if(dmin > distDown && aux+SCENE_WIDTH>-1) //DOWN
	{
		dmin = dist[pos-SCENE_WIDTH]; 
		aux = pos-SCENE_WIDTH;
	}
	return aux;
}
void cMonstre::AI(int *map)
{
	int *dist = BFS(map,pf,pos);
	oldPos = pos;
	int aux = pos;
	int dmin = dist[pos];
	distAct = dist[pos];
	distR = dist[pos+1];
	distL = dist[pos-1];
	distUp = dist[pos+SCENE_WIDTH];
	distDown = dist[pos-SCENE_WIDTH];

	if(dmin > distR && ((aux+1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)+1 ) // R
	{
		dmin = dist[pos+1]; 
		aux = pos+1;
	} 	
	if(dmin > distL && ((aux-1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)-1 ) // L
	{
		dmin = dist[pos-1]; 
		aux = pos-1;
	} 
	if(dmin > distUp && aux+SCENE_WIDTH<64) //UP
	{
		dmin = dist[pos+SCENE_WIDTH]; 
		aux = pos+SCENE_WIDTH;
	}
	if(dmin > distDown && aux+SCENE_WIDTH>-1) //DOWN
	{
		dmin = dist[pos-SCENE_WIDTH]; 
		aux = pos-SCENE_WIDTH;
	}
	if(aux != NextPos){
		int fail=0;
		pos = NextPos;
	}else{
		pos = aux;
	}
	NextPos = NextMov(map);
}

int cMonstre::MakeMonstreDL(float w,float h,float d,float tw,float th,float td)
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
			// Top Face
			glTexCoord2f(0.0f,   td); glVertex3f(0, h, -d);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, h,  0);
			glTexCoord2f(  tw, 0.0f); glVertex3f(w, h,  0);
			glTexCoord2f(  tw,   td); glVertex3f(w, h, -d);
		glEnd();
	glEndList();
	return dl_monstre;
	
}

void cMonstre::animacio(){
	float xo,yo,xf,yf;
	int state = GetState();
	switch(state)
	{
		case STATE_WALKDOWN:	xo = 0.0f + (GetFrame()*0.25f);yo = 0.25f;
								NextFrame(4);
								break;
		case STATE_WALKRIGHT:	xo = 0.0f + (GetFrame()*0.25f);yo = 0.50f;
								NextFrame(4);
								break;
		case STATE_WALKLEFT:	xo = 0.0f + (GetFrame()*0.25f); yo = 0.75f;
								NextFrame(4);
								break;
		case STATE_WALKUP:	xo = 0.0f + (GetFrame()*0.25f);yo = 1.00f;
								NextFrame(4);
								break;
	}
	xf = xo + 0.25f;
	yf = yo - 0.25f;
	int ok = MakeMonstre2DL((float)TILE_SIZE,(float)TILE_SIZE,(float)TILE_SIZE,xo,yo,xf,yf);
}

int cMonstre::MakeMonstre2DL(float w,float h,float d,float xo,float yo,float xf,float yf)
{
	dl_monstre2 = glGenLists(1);
	glNewList(dl_monstre2,GL_COMPILE);
		glTranslatef(0.0f,0.2f,-1.0f);
		glRotatef(-30,1.0f,0.0f,0.0f);
		glBegin(GL_QUADS);			
			// Front Face
			glTexCoord2f(xo,yo); glVertex3f(0, 0,  0);
			glTexCoord2f(xf,yo); glVertex3f(w, 0,  0);
			glTexCoord2f(xf,yf); glVertex3f(w, h+1.5f,  0);
			glTexCoord2f(xo,yf); glVertex3f(0, h+1.5f,  0);
		glEnd();
	glEndList();
	return dl_monstre2;
	/*
	dl_monstre = glGenLists(1);
	glNewList(dl_monstre,GL_COMPILE);
			glPushMatrix();
				glTranslatef(w/2.0f,2.0f,-d/2.0f);
				//glRotatef(90,1.0f,0.0f,0.0f);
				GLUquadricObj *q = gluNewQuadric();
				gluQuadricTexture (q, GL_TRUE);
				//gluCylinder(q, 0.3,0.95,5,16,16);
				gluSphere(q, 2.0,16,16);
				glRotatef(-90,1.0f,0.0f,0.0f);
				//gluCylinder(q, 1.2,1.2,3.5,16,16);
				gluDeleteQuadric(q);
			glPopMatrix();
		glEndList();
		return dl_monstre;*/
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
		int dt=0;
		int dt2=0;
		if(GetType()==3){
			dt=9;
			dt2=8;
		}
		
		if((map[aux] == 0  || map[aux]==dt || map[aux]==dt2)&& mapbool[aux] < 4){
			mapbool[aux] += 1;
			if(map[45]==9){
				int zzz=0;
			}
			//en creu
			if((map[aux+1]==0 || map[aux+1]==dt || map[aux+1]==dt2) && ((aux+1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)+1 ) {
				q.push(aux+1); 
				if(dist[aux+1]>dist[aux]+1) dist[aux+1]=dist[aux]+1;
			}
			if((map[aux-1]==0 || map[aux-1]==dt || map[aux-1]==dt2) && ((aux-1)%SCENE_WIDTH)==((aux)%SCENE_WIDTH)-1) {
				q.push(aux-1);
				if(dist[aux-1]>dist[aux]+1)dist[aux-1]=dist[aux]+1;
			}
			if((map[aux+SCENE_WIDTH]==0 || map[aux+SCENE_WIDTH]==dt || map[aux+SCENE_WIDTH]==dt2) && aux+SCENE_WIDTH<64 ) {
				q.push(aux+SCENE_WIDTH); 
				if(dist[aux+SCENE_WIDTH]>dist[aux]+1)dist[aux+SCENE_WIDTH]=dist[aux]+1;
			}
			if((map[aux-SCENE_WIDTH]==0 || map[aux-SCENE_WIDTH]==dt || map[aux-SCENE_WIDTH]==dt2) && aux-SCENE_WIDTH>-1) {
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
	_itoa_s(pos,buffx,10 );
	_itoa_s(oldPos,buffy,10 );
	_itoa_s(distAct,buffa,10 );
	_itoa_s(distR,buffb,10 );
	_itoa_s(distL,buffb,10 );
	_itoa_s(distUp,buffb,10 );
	_itoa_s(distDown,buffb,10 );
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

void cMonstre::printVida()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	char buffx[10];
	_itoa_s(vida,buffx,10 );
	char *s[]={	"Vida: ", buffx
				};

	glColor3f(1.0f,1.0f,1.0f);

	glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
			//glRasterPos2f(-0.95f,0.80f);
			glTranslatef(10,0,40);
			render_string(GLUT_BITMAP_9_BY_15,s[0]);
			//glRasterPos2f(-0.80f,0.80f);
			render_string(GLUT_BITMAP_9_BY_15,s[1]);
		glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int cMonstre::GetDir()
{
	return dir;
}
int cMonstre::GetNextPos()
{
	return NextPos;
}

void cMonstre::incExpAnim()
{
	expAnim++;
	//erase = (expAnim>=25);
}
int cMonstre::getExpAnim()
{
	return expAnim;
}
bool cMonstre::getDeath()
{
	return death;
}
void cMonstre::setDeath()
{
	death=true;
}