//---------------------------------------------------------
// file:	Collision.c
// author:	Gary Chen, Owen Reeves
// brief:	Collision detection system
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "C_Processing.h"
#include "Collision.h"
#include "Physics.h"


//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------
#define NO_INTERSECTION -100000
//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

/* rectangular collider to be put on objects */
typedef struct coll collider;
/* To store which side of the AABB retangle hit. */
static int last_collide_positon = 0;

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Variables:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Function Declarations:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

//---------------------------------------------------------

/**************************************************************************
   Function: getPosition
Description: checks the position of one object reletive to another
     Inputs: obj1, the collider of the first object
		     obj2, the collider of the second object

    Outputs: Top(1), Left(2), Bottom(3), Right(4), Error(-1). 
**************************************************************************/
/* Must be called AFTER perfectCollision() to work */
int getPosition(collider obj1, collider obj2)
{
	return last_collide_positon;
}

/**************************************************************************
   Function: isCollide

Description: checks if two colliders have collided

     Inputs: obj1, the collider of the first object
			 obj2, the collider of the second object
		
    Outputs: boolean for if a collision has happened (1) or not (0)
**************************************************************************/
int isCollide(collider obj1, collider obj2)
{
	return (obj1.position.x < obj2.position.x + obj2.size.x &&
		obj1.position.x + obj1.size.x > obj2.position.x &&
		obj1.position.y < obj2.position.y + obj2.size.y &&
		obj1.position.y + obj1.size.y > obj2.position.y);
}

/**************************************************************************
   Function: perfectCollision

Description: creates a collision with no overlap

     Inputs: obj,  the physics for obj1
			 obj1, the collider of the first object
		     obj2, the collider of the second object

    Outputs: a modified collider to replace obj1
**************************************************************************/
collider perfectCollision(physics obj, collider obj1, collider obj2)
{
	vec2_t tmp;
	 /* set position of obj1 to before collsion */
	obj1.position = vec2_sub(obj1.position, obj.direction);
	 /* rayCast return the collider object with perfect collision coordinate */
	tmp = rayCast(obj, obj1,  obj2);
	 /* Set obj1 to new value(tmp) if there is a perfect collision found. */
	if (tmp.x != NO_INTERSECTION)
		obj1.position = tmp;

	return obj1;
}

/**************************************************************************
   Function: newCollider

Description: creates a newCollider for testing purpose.

	 Inputs: positionX	X coordinate of this new collider
			 positionY	Y coordinate of this new collider
			 sizeX		Width of this new collider
			 size		Height of this new collider

	Outputs: A newly initialized collider
**************************************************************************/
collider newCollider(float positionX, float positionY, float sizeX, float sizeY)
{
	collider new_c;
	new_c.position.x = positionX;
	new_c.position.y = positionY;
	new_c.size.x = sizeX;
	new_c.size.y = sizeY;
	return new_c;
}

/**************************************************************************
   Function: lineIntersection

Description: Check if two line segments has intersection.

	 Inputs: a	Coordinate of point 1 of line 1
			 b	Coordinate of point 2 of line 1
			 c	Coordinate of point 1 of line 2
			 d	Coordinate of point 2 of line 2

	Outputs: Coordniate of intersection if there's one.
**************************************************************************/
vec2_t lineIntersection(vec2_t a, vec2_t b, vec2_t c, vec2_t d)
{
	 /* Make vector of two line using their points.  */
	vec2_t r = vec2_sub(b, a);
	vec2_t s = vec2_sub(d, c);
	
	 /* Math function to find the intersection. */
	float denominator = r.x * s.y - s.x * r.y;
	float u = ((c.x - a.x) * r.y - (c.y - a.y) * r.x) / denominator;
	float t = ((c.x - a.x) * s.y - (c.y - a.y) * s.x) / denominator;

	return vec2_add(a, vec2_scale(r, t));
}

/**************************************************************************
   Function: rayCast

Description: Use ray-cast plus Minkowski difference to find the collide
			 side and its perfect collision's coordinate.

	 Inputs: obj	the physics for obj1
			 obj1	the collider of the first object
			 obj2	the collider of the second object

	Outputs: The coordinate of the perfect coolistion.
**************************************************************************/
vec2_t rayCast(physics obj, collider obj1, collider obj2)
{
	/* 
	 * Reduce obj to single point and span obj2 to a bigger one useing Minkowski
	 * difference. New Minkowski_obj2 = newCollider(obj2.position.x - obj1.size.x, 
	 * obj2.position.y - obj1.size.y, obj2.size.x + obj1.size.x, obj2.size.y + 
	 * obj1.size.y); 
	 */
	int i;
	float dist;
	 /* Set new obj2's four corner coordinates. */
	vec2_t top_left_coner = vec2_set(obj2.position.x - obj1.size.x, obj2.position.y - obj1.size.y);
	vec2_t top_right_coner = vec2_set(obj2.position.x + obj2.size.x, obj2.position.y - obj1.size.y);
	vec2_t bottom_left_coner = vec2_set(obj2.position.x - obj1.size.x, obj2.position.y + obj2.size.y);
	vec2_t bottom_right_coner = vec2_set(obj2.position.x + obj2.size.x, obj2.position.y + obj2.size.y);
	 /* tmp store intersction of obj1 to each side of new obj2. */
	vec2_t tmp[4];
	 /* Store the coordinate of closest intersection. Remain NO_INTERSECTION mean no intersection found. */
	vec2_t intersection = vec2_set(NO_INTERSECTION, NO_INTERSECTION);

	 /* Find intersection of obj1 + speed vector and four side of obj2. */
	tmp[0] = lineIntersection(obj1.position, vec2_add(obj1.position, obj.direction), top_left_coner, top_right_coner);
	tmp[1] = lineIntersection(obj1.position, vec2_add(obj1.position, obj.direction), top_left_coner, bottom_left_coner);
	tmp[2] = lineIntersection(obj1.position, vec2_add(obj1.position, obj.direction), bottom_right_coner, top_right_coner);
	tmp[3] = lineIntersection(obj1.position, vec2_add(obj1.position, obj.direction), bottom_right_coner, bottom_left_coner);
	
	 /* Find the closet point of intersection among the four side. */
	for (i = 0, dist = 100000; i < 4; i++)
	{
		if(tmp[i].x != NO_INTERSECTION)
			if (vec2_distance(tmp[i], obj1.position) < dist)
			{
				dist = vec2_distance(tmp[i], obj1.position);
				intersection = tmp[i];
				last_collide_positon = i;
			}
	}
	return intersection;
}

//---------------------------------------------------------

//---------------------------------------------------------
// Private Functions:
//---------------------------------------------------------