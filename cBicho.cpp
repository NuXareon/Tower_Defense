#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"

cBicho::cBicho(void)
{
	seq=0;
	delay=0;

	jumping = false;
	caure = false;

	shot_cd = 0;
	roll_count = 0;

	buff_speed = false;
	buff_power = false;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx,int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}
void cBicho::SetType(int i)
{
	type = i;
}
int cBicho::GetType()
{
	return type;
}
/*
void cBicho::SetShotCd(int cd)
{
	shot_cd = cd;
}
void cBicho::GetShotCd(int *cd)
{
	*cd = shot_cd;
}
int cBicho::GetRollCount()
{
	return roll_count;
}
void cBicho::DisableBuff(int id)
{
	if (id == POWER_SHOT_BUFF_ID) buff_power = false;
	if (id == SPEED_BUFF_ID) buff_speed = false;
}
void cBicho::EnableBuff(int id)
{
	if (id == POWER_SHOT_BUFF_ID) buff_power = true;
	if (id == SPEED_BUFF_ID) buff_speed = true;
}
bool cBicho::GetBuffStatus(int id)
{
	if (id == POWER_SHOT_BUFF_ID) return buff_power;
	if (id == SPEED_BUFF_ID) return buff_speed;
	return false;
}
*/
bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
}
bool cBicho::CollidesMapWall(int *map,bool right)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if(right)	tile_x += width_tiles;
	
	for(j=0;j<height_tiles;j++)
	{
		if(map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ] != 0)	return true;
	}
	
	return false;
}
bool cBicho::CollidesMapFloor(int *map)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if( (x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if( (y % TILE_SIZE) == 0 )
		{
			if(map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ] == 1 ||
				map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ] == 2)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] == 1 ||
				map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] == 2)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}
void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}
/*
void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::MoveLeft(int *map)
{
	int xaux;
	
	//Whats next tile?
	if( (x % TILE_SIZE) == 0 || x < 2*TILE_SIZE)
	{
		xaux = x;
		if (buff_speed) x -= 2*STEP_LENGTH;
		else x -= STEP_LENGTH;

		if(CollidesMapWall(map,false) )
		{
			x = xaux;
			if(!caure && !jumping)state = STATE_LOOKLEFT;
			else if(caure) state = STATE_CAUREL;
		}
	}
	//Advance, no problem
	else
	{
		if (buff_speed) x -= 2*STEP_LENGTH;
		else x -= STEP_LENGTH;
		if(caure && !jumping) state = STATE_CAUREL;
		else if(state != STATE_WALKLEFT && !jumping)
		{
			state = STATE_WALKLEFT;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::MoveRight(int *map)
{
	int xaux;

	//Whats next tile?
	if( (x % TILE_SIZE) == 0 || x > (SCENE_WIDTH-4)*TILE_SIZE)
	{
		xaux = x;
		if (buff_speed) x += 2*STEP_LENGTH;
		else x += STEP_LENGTH;
		if(CollidesMapWall(map,true))
		{
			x = xaux;
			if(!caure && !jumping)state = STATE_LOOKRIGHT;
			else if(caure) state = STATE_CAURER;
		}
	}
	//Advance, no problem
	else
	{
		if (buff_speed) x += 2*STEP_LENGTH;
		else x += STEP_LENGTH;

		if(caure && !jumping) state = STATE_CAURER;
		else if(state != STATE_WALKRIGHT && !jumping)
		{
			state = STATE_WALKRIGHT;
			seq = 0;
			delay = 0;
		}
		
	}
}
void cBicho::RollLeft(int *map)
{
	int xaux;
	
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x -= ROLL_STEP;

		if(CollidesMapWall(map,false) )
		{
			x = xaux;
			state = STATE_ROLLINGR;
			++roll_count;
		}
	}
	//Advance, no problem
	else
	{
		x -= STEP_LENGTH;
	}
}
void cBicho::RollRight(int *map)
{
	int xaux;

	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x += ROLL_STEP;
		if(CollidesMapWall(map,true))
		{
			x = xaux;
			state = STATE_ROLLINGL;
			++roll_count;
		}
	}
	//Advance, no problem
	else
	{
		x += STEP_LENGTH;
	}
}
void cBicho::Stop(int *map)
{
	if(caure){ //caure
		switch(state)
		{
			case STATE_WALKLEFT:	state = STATE_CAUREL;	break;
			case STATE_WALKRIGHT:	state = STATE_CAURER;	break;
			case STATE_LOOKLEFT:	state = STATE_CAUREL;	break;
			case STATE_LOOKRIGHT:	state = STATE_CAURER;	break;
			case STATE_JUMPINGL:	state = STATE_CAUREL;	break;
			case STATE_JUMPINGR:	state = STATE_CAURER;	break;
		}
	}
	else if(jumping && !CollidesMapFloor(map)){
		switch(state)
		{
			case STATE_WALKLEFT:	state = STATE_JUMPINGL;	break;
			case STATE_WALKRIGHT:	state = STATE_JUMPINGR;	break;
			case STATE_LOOKLEFT:	state = STATE_JUMPINGL;	break;
			case STATE_LOOKRIGHT:	state = STATE_JUMPINGR;	break;
		}
	}
	else{
		switch(state)
		{
			case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
			case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
			case STATE_JUMPINGL:	state = STATE_LOOKLEFT;		break;
			case STATE_JUMPINGR:	state = STATE_LOOKRIGHT;	break;
			case STATE_CAUREL:		state = STATE_LOOKLEFT;		break;
			case STATE_CAURER:		state = STATE_LOOKRIGHT;	break;
		}
	}
	
}
void cBicho::Jump(int *map)
{
	if(!jumping)
	{
		if(CollidesMapFloor(map))
		{
			jumping = true;
			jump_alfa = 0;
			jump_y = y;
			switch(state)
			{
				case STATE_WALKLEFT:	state = STATE_JUMPINGL;	break;
				case STATE_WALKRIGHT:	state = STATE_JUMPINGR;	break;
				case STATE_LOOKLEFT:	state = STATE_JUMPINGL;	break;
				case STATE_LOOKRIGHT:	state = STATE_JUMPINGR;	break;
				case STATE_CAUREL:		state = STATE_JUMPINGL;	break;
				case STATE_CAURER:		state = STATE_JUMPINGR;	break;
			}
		}		
	}
}*/
void cBicho::Logic(int *map)
{
	float alfa;

	if(jumping)
	{
		jump_alfa += JUMP_STEP;
		
		if(jump_alfa == 180)
		{
			jumping = false;
			y = jump_y;
		}
		else
		{
			alfa = ((float)jump_alfa) * 0.017453f;
			y = jump_y + (int)( ((float)JUMP_HEIGHT) * sin(alfa) );
		
			if(jump_alfa > 90)
			{
				//Over floor?
				jumping = !CollidesMapFloor(map);
			}
		}
	}
	else
	{
		//Over floor?
		if(!CollidesMapFloor(map)){
			y -= (2*STEP_LENGTH);
			caure = true;
		}else caure = false;
	}

	if (shot_cd > 0) shot_cd -= 1;
}
void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == FRAME_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cBicho::GetFrame()
{
	return seq;
}
int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}
void cBicho::ResetFrame()
{
	seq = 0;
}
