//---------------------------------------------------------
// file:	Camera.h
// author:	Garry Chen
// brief:	Camera fuctions
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "position.h"
#include "Physics.h"
#include "Collision.h"
#include "Player.h"

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------


typedef struct 
{
	vec2_t position;
	collider collBox;
	float window_left_edge;
	float window_right_edge;
	float map_left_edge;
	float map_right_edge;

}CAMERA;


//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------



//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------
CAMERA *createCamera(float window_left_edge,
	float window_right_edge,
	float screen_width,
	float screen_height,
	float map_left_edge,
	float map_right_edge);
void setCameraPosition(CAMERA * cam, player * p);
void drawObj(CAMERA * cam, player * p, MAP *map);
