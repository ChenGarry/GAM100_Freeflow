//---------------------------------------------------------
// file:	Map.h
// author:	Garry Chen
// brief:	Basic Collision engine organization
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "position.h"
#include "Collision.h"

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------
#define TILE_SIZE 30

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

enum TILE_TYPE {
	PLAYR_START = -1,
	EMPTY,
	WALL,
	FLOOR,
	TIME,
	GOAL,
	EXIT
};

typedef struct 
{
	int type;

	physics speed;
	collider collBox;

}TILE;

typedef struct
{
	int player_x;
	int player_y;
	int width;
	int height;
	float tile_width;
	float tile_height;
	void * bitmap;
}MAP;

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------



//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

void loadMap(char * level_dir, MAP *map, float tile_width, float tile_height);
void setLevel(int level);
