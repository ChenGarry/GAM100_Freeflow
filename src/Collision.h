//---------------------------------------------------------
// file:	Collision.h
// author:	Gary Chen, Owen Reeves
// brief:	Collision detection system
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "Physics.h"

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

typedef struct coll
{
	/* position of top left corner of rectangle */
	vec2_t position;
	vec2_t size;

} collider;

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------


int isCollide(collider obj1, collider obj2);
collider perfectCollision(physics obj, collider obj1, collider obj2);
int getPosition(collider obj1, collider obj2);

collider newCollider(float positionX, float positionY, float sizeX, float sizeY);
vec2_t rayCast(physics obj, collider obj1, collider obj2);