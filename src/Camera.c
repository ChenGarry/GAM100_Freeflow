//---------------------------------------------------------
// file:	Camera.c
// author:	Garry Chen
// brief:	Basic camera system
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------
#include<stdio.h> 
#include<stdlib.h> 
#include "C_Processing.h"
#include "Physics.h"
#include "Map.h"
#include "Camera.h"
#include "Player.h"

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------


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
   Function: createCamera

Description: Create a new CAMERA sturct and initialize it with the 
			 parameterspassed by the caller.

     Inputs: window_left_edge   Camera's detection box's left edge's x 
								coordinate in the actual map.
		     window_right_edge  Camera's detection box's right edge's x 
								coordinate in the actual map.
			 screen_width		Width of the game's window screen. Will be
								use as camera's collideBox's width.
			 screen_height		Height of the game's window screen. Will be
								use as camera's collideBox's height.
			 map_left_edge		Actual map's left edge x coordinate.
			 map_right_edge		Actual map's right edge x coordinate.

    Outputs: Pointer to the new CAMERA sturct.
**************************************************************************/
CAMERA *createCamera(float window_left_edge,
					 float window_right_edge,
					 float screen_width,
					 float screen_height,
					 float map_left_edge,
					 float map_right_edge)
{
	CAMERA * new_c = (CAMERA *)malloc(sizeof(CAMERA));

	 /* Camera position start at (0, 0) */
	new_c->position.x = 0;
	new_c->position.y = 0;
	new_c->collBox.position.x = 0;
	new_c->collBox.position.y = 0;
	new_c->collBox.size.x = screen_width;
	new_c->collBox.size.y = screen_height;
	new_c->map_left_edge = map_left_edge;
	new_c->map_right_edge = map_right_edge;
	new_c->window_left_edge = window_left_edge;
	new_c->window_right_edge = window_right_edge;

	return new_c;
}

/**************************************************************************
   Function: setCameraPosition

Description: Change camera's position when player reach to its detection 
			 box.

	 Inputs: cam	Camera needed to change position.
			 p		Player's movement which may resualt camera change.

	Outputs: None.
**************************************************************************/
void setCameraPosition(CAMERA * cam, player * p)
{
	 /* If player reach at the left edge of the camera detection box and there is still room left for camera to move left. */
	if(p->collBox.position.x < cam->window_left_edge && cam->collBox.position.x > cam->map_left_edge)
	{
		 /* dist1 = player and camera's distance difference.	    */
		float dist1 = cam->window_left_edge - p->collBox.position.x;
		 /* dist2 = camera edge and map edge's distance difference. */
		float dist2 = cam->position.x - cam->map_left_edge;

		 /* Check if there's enough room for camera to move left, if not just move as close as it can. */
		if (dist2 < dist1)
		{
			dist1 = dist2;
		}
		if (dist1 < 0)
			dist1 = 0;
		 /* Set new camera data to new position. */
		cam->position.x -= dist1;
		cam->collBox.position.x -= dist1;
		cam->window_left_edge -= dist1;
		cam->window_right_edge -= dist1;
	}
	/* If player reach at the right edge of the camera detection box and there is still room left for camera to move right. */
	else if ((p->collBox.position.x + p->collBox.size.x) > cam->window_right_edge && (cam->collBox.position.x + cam->collBox.size.x) < cam->map_right_edge)
	{
		 /* dist1 = player and camera's distance difference.	    */
		float dist1 = (p->collBox.position.x + p->collBox.size.x) - cam->window_right_edge;
		 /* dist2 = camera edge and map edge's distance difference. */
		float dist2 = cam->map_right_edge - (cam->collBox.position.x + cam->collBox.size.x);

		 /* Check if there's enough room for camera to move left, if not just move as close as it can. */
		if (dist2 < dist1)
		{
			dist1 = dist2;
		}
		if (dist1 < 0)
			dist1 = 0;

		 /* Set new camera data to new position. */
		cam->position.x += dist1;
		cam->collBox.position.x += dist1;
		cam->window_left_edge += dist1;
		cam->window_right_edge += dist1;

	}
}

/**************************************************************************
   Function: drawObj

Description: Tanslate map coordinate of an object to screen coordinate and
			 draw them on the screen.

	 Inputs: cam	Camera use to translate the coordinate.
			 p		Player needed to be draw.
			 map	Map contain tiles needed to be draw.

	Outputs: None.
**************************************************************************/
void drawObj(CAMERA * cam, player * p, MAP *map)
{
	int i, j;
	
	 /* Draw all the tiles inside the camera range. */
	for (i = 0; i < map->height; i++)
	{
		for (j = 0; j < map->width; j++)
		{
			 /* Use collision to decide a tile is inside the camera screen. */
			if (isCollide(((TILE*)map->bitmap + j + i * map->width)->collBox, cam->collBox))
			{
				 /* Draw green rectangle for the floor and wall. */
				if (((TILE*)map->bitmap + j + i * map->width)->type == WALL || 
					((TILE*)map->bitmap + j + i * map->width)->type == FLOOR)
				{
					fill(0, 255, 0, 255);
					rect(j * map->tile_width - cam->position.x, i * map->tile_height - cam->position.y, map->tile_width, map->tile_height);
				}
				/* Draw orange circle for the goal. */
				if (((TILE*)map->bitmap + j + i * map->width)->type == GOAL)
				{
					fill(255, 140, 0, 255);
					circleMode(CORNER);
					circle(j * map->tile_width - cam->position.x, i * map->tile_height - cam->position.y, map->tile_width / 2);
				}
				/* Draw cyan rectangle for the program exit block. */
				if (((TILE*)map->bitmap + j + i * map->width)->type == EXIT)
				{
					fill(0, 255, 255, 255);
					rect(j * map->tile_width - cam->position.x, i * map->tile_height - cam->position.y, map->tile_width, map->tile_height);
				}
				/* Draw yellow rectangle for minus time collectibles. */
				if (((TILE*)map->bitmap + j + i * map->width)->type == TIME)
				{
					fill(255, 255, 0, 255);
					rect(j * map->tile_width - cam->position.x +10, i * map->tile_height - cam->position.y + 10, map->tile_width - 20, map->tile_height - 20);
				}
			}
		}
	}
	
	 /* Draw the player. */
	fill(255, 0, 0, 255);
	rect(p->collBox.position.x - cam->position.x, p->collBox.position.y - cam->position.y, p->collBox.size.x, p->collBox.size.y);
}

//---------------------------------------------------------

//---------------------------------------------------------
// Private Functions:
//---------------------------------------------------------
