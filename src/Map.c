//---------------------------------------------------------
// file:	Map.c
// author:	Garry Chen
// brief:	Basic Map manage system
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------
#include<stdio.h> 
#include<stdlib.h> 
#include "C_Processing.h"
#include "Collision.h"
#include "Map.h"


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
   Function: loadMap

Description: Load the tile base text edit map file's data to MAP struct

     Inputs: level_dir		Directory of the map file.
		     map			Struct to store map data.
			 tile_width		Width of one tile in pixel
			 tile_height	Height of one tile in pixel

    Outputs: None.
**************************************************************************/
void loadMap(char * level_dir, MAP *map, float tile_width, float tile_height)
{
	FILE * map_file;
	int tmp_c;
	int i, j;

	 /*
	  * Open the file and read in two integer. First one is map's width in tile
	  * Second is map's height in tile.
	  */
	fopen_s(&map_file, level_dir , "r");
	fscanf_s(map_file, "%d%d", &(map->width), &(map->height));
	 /* Allocate memory for the 2d tile map. */
	map->bitmap = malloc(sizeof(TILE) * (map->width) * (map->height));
	map->tile_height = tile_height;
	map->tile_width = tile_width;
	

	for (i = 0; (tmp_c = fgetc(map_file)) != EOF;)
	{
		 /* Flag use to initialize collision box. */
		int flag = 0; 

		 /* Check tile type and store the data. */
		if (tmp_c == '1')
		{
			vec2_t size;
			((TILE*)map->bitmap + i)->type = WALL;
			i++;
			flag = 1;
		}
		else if (tmp_c == '2')
		{
			((TILE*)map->bitmap + i)->type = FLOOR;
			i++;
			flag = 1;
		}
		else if (tmp_c == '.')
		{
			((TILE*)map->bitmap + i)->type = EMPTY;
			i++;
			flag = 1;
		}
		else if(tmp_c == '0')
		{
			((TILE*)map->bitmap + i)->type = PLAYR_START;
			map->player_x = i % map->width;
			map->player_y = i / map->width;
			i++;
			flag = 1;
		}
		else if (tmp_c == 'X')
		{
			((TILE*)map->bitmap + i)->type = GOAL;
			i++;
			flag = 1;
		}
		 /* Type Time is collectible with smaller collBox.  */
		else if (tmp_c == 'T')
		{
			((TILE*)map->bitmap + i)->type = TIME;
			((TILE*)map->bitmap + i)->collBox.position.x = (i % map->width) * tile_width + 10;
			((TILE*)map->bitmap + i)->collBox.position.y = (i / map->width) * tile_height + 10;
			((TILE*)map->bitmap + i)->collBox.size.x = tile_width - 20;
			((TILE*)map->bitmap + i)->collBox.size.y = tile_height - 20;
			i++;
		}
		else if (tmp_c == 'E')
		{
			((TILE*)map->bitmap + i)->type = EXIT;
			i++;
			flag = 1;
		}
		 /* Initialize collBox when tile is not EMPTY. */
		if (flag)
		{
			i--;
			((TILE*)map->bitmap + i)->collBox.position.x = (i % map->width) * tile_width;
			((TILE*)map->bitmap + i)->collBox.position.y = (i / map->width) * tile_height;
			((TILE*)map->bitmap + i)->collBox.size.x = tile_width;
			((TILE*)map->bitmap + i)->collBox.size.y = tile_height;
			i++;
		}
	}

	fclose(map_file);
}


//---------------------------------------------------------

//---------------------------------------------------------
// Private Functions:
//---------------------------------------------------------
