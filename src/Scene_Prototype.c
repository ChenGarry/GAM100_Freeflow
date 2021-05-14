//---------------------------------------------------------
// file:	Scene_Prototype.c
// author:	Team
// brief:	DigiPen Logo scene functions
// course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "C_Processing.h"
#include "Scene_Prototype.h"
#include "SceneManager.h"
#include "movement.h"
#include "Map.h"
#include "Speedrun.h"
#include "Camera.h"

#define startingLevel 2

//---------------------------------------------------------
// Private Consts:
//---------------------------------------------------------

//---------------------------------------------------------
// Private Structures:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Variables:
//---------------------------------------------------------

player playerHolder;
player * currentPlayer = &playerHolder;
float screensizeX = 1600;
float screensizeY = 800;
int playerHeight = 40;
int playerWidth = 40;
int levelInc = startingLevel;
int inputs[5];
PSound jump_sound;
PSound win_sound;
PSound Cowbell;
PImage credit;
//---------------------------------------------------------
// Private Variables:
//---------------------------------------------------------
MAP map1;
CAMERA *cam;



//---------------------------------------------------------
// Private Function Declarations:
//---------------------------------------------------------

//---------------------------------------------------------
// Public Functions:
//---------------------------------------------------------

void initPlayer(MAP * map)
{
	//currentPlayer->collBox.position.x = 500; currentPlayer->collBox.position.y = 500;
	currentPlayer->tile_in_x = map->player_x;
	currentPlayer->tile_in_y = map->player_y;

	currentPlayer->collBox.position = ((TILE*)map->bitmap + map->player_x + map->width * map->player_y)->collBox.position;
	
	currentPlayer->tile_position = currentPlayer->collBox.position;
	currentPlayer->collBox.size.x = (float)playerWidth; currentPlayer->collBox.size.y = (float)playerHeight;
	currentPlayer->width_in_tile = (int)(playerWidth / map->tile_width) + 1;
	currentPlayer->height_in_tile = (int)(playerHeight / map->tile_height) + 1;

	//currentPlayer->onGround = 0;
	currentPlayer->onWall = 0;

	//currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;

}

//---------------------------------------------------------
// Initialize the DigiPen Logo scene.
void inirutine(char * level_dir)
{
	loadMap(level_dir, &map1, (float)screensizeX / 40, (float)screensizeY / 20);
	initPlayer(&map1);
	 /* Initialize camera with new map data. */
	cam = createCamera(screensizeX * 0.25, screensizeX * 0.75, screensizeX, screensizeY, 0, map1.width * map1.tile_width);
}

void ScenePrototypeInit()
{
	setSpeedInc(startingLevel - 1);
	levelInc = startingLevel;
	setFrameRate(30);
	timer = 0;
	file_Load();
	int i;
	jump_sound = loadSound("./Assets/Cartoons-MouthHarp-CU-Hig_6.wav", 0);
	win_sound = loadSound("./Assets/Crowds-Medium-ApplauseCheer.wav", 0);
	Cowbell = loadSound("./Assets/Cowbell.wav", 0);
	credit = loadImage("./Assets/credits.png");
	for (i = 0; i < 5; i++)
	{
		inputs[i] = 0;
	}
	setLevel(levelInc);
	fill(255, 0, 0, 255);
	size((int)screensizeX, (int)screensizeY);

	noStroke();
}

void collisions()
{
	int i, j;
	int needChange = 0;

	/* flag to show if there were any collisions */
	int collisionflag = 0;

	/* resets Wall Jumping Capability */
	currentPlayer->onWall = 0;
	
	for (i = currentPlayer->tile_in_y - 1 - 1; i < currentPlayer->tile_in_y + currentPlayer->height_in_tile + 1; i++)
	{
		for (j = currentPlayer->tile_in_x - 1; j < currentPlayer->tile_in_x + currentPlayer->width_in_tile; j++)
		{
			TILE *tmp_t = (TILE*)map1.bitmap + j + (i *+ map1.width);

			if (tmp_t->type == WALL || tmp_t->type == FLOOR)
			{
				if (isCollide(tmp_t->collBox, currentPlayer->collBox))
				{
					++collisionflag; 
					int collide_side;
					currentPlayer->collBox = perfectCollision(currentPlayer->speed, currentPlayer->collBox, tmp_t->collBox);
					collide_side = getPosition(currentPlayer->collBox, tmp_t->collBox);

					if (collide_side == 0)
					{
						resetAcceleration(&(currentPlayer->speed.direction.y));
						currentPlayer->onGround = 1;
					}
					else if (collide_side == 1 && tmp_t->type != FLOOR)
					{
						resetAcceleration(&(currentPlayer->speed.direction.x));
						currentPlayer->onWall = -1;
					}
					else if (collide_side == 2 && tmp_t->type != FLOOR)
					{
						resetAcceleration(&(currentPlayer->speed.direction.x));
						currentPlayer->onWall = 1;
					}
					else if (collide_side == 3)
					{
						resetAcceleration(&(currentPlayer->speed.direction.y));
					}
					changePos(currentPlayer, &map1);
				}
			}
			else if (tmp_t->type == GOAL)
			{
				if (isCollide(tmp_t->collBox, currentPlayer->collBox))
				{
					if (levelInc <= levelCount)
					{
						levelInc++;
						nextLevel(levelInc);
						setLevel(levelInc);
					}
				}
			}
			else if (tmp_t->type == EXIT)
			{
				if (isCollide(tmp_t->collBox, currentPlayer->collBox))
				{
					free(map1.bitmap);
					Terminate();
				}
			}
			else if (tmp_t->type == TIME)
			{
				if (isCollide(tmp_t->collBox, currentPlayer->collBox))
				{
					sound(Cowbell);
					tmp_t->type = EMPTY;
					timer -= 1;
				}
			}
		}
	}

	/* if no collisions occured at all(in the air), change onGround to 0*/
	if (!collisionflag)
	{
		currentPlayer->onGround = 0;
	}

}

void drawScreen()
{
	fill(0, 0, 0, 255);
	rect(0, 0, screensizeX, screensizeY);

	setCameraPosition(cam, currentPlayer);
	drawObj(cam, currentPlayer, &map1);
}


/**************************************************************************
   Function: consoleCommands
Description: does special actions beased on specific input
     Inputs: Special key presses
    Outputs: game changes
**************************************************************************/
void consoleCommands()
{
	/* set player to level 1 */
	if (keyIsDown(KEY_1))
	{
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		levelInc = 1;
		setLevel(levelInc);
		setSpeedInc(0);
	}

	/* set player to level 2 */
	else if (keyIsDown(KEY_2))
	{
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		levelInc = 2;
		setLevel(levelInc);
		setSpeedInc(1);
	}

	/* set player to level 3 */
	else if (keyIsDown(KEY_3))
	{
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		levelInc = 3;
		setLevel(levelInc);
		setSpeedInc(2);
	}

	/* set player to level 4 */
	else if (keyIsDown(KEY_4))
	{
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		levelInc = 4;
		setLevel(levelInc);
		setSpeedInc(3);
	}

	/* set player to level 5 */
	else if (keyIsDown(KEY_5))
	{
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		levelInc = 5;
		setLevel(levelInc);
		setSpeedInc(4);
	}

	/* set player to level 6 */
	else if (keyPressed(KEY_6))
	{
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		levelInc = 6;
		setLevel(levelInc);
		setSpeedInc(5);
	}

	/* set player to beginning of game */
	else if (keyIsDown(KEY_R))
	{
		levelInc = startingLevel;
		setSpeedInc(levelInc - 1);
		currentPlayer->onGround = 0;
		currentPlayer->speed.direction.x = 0; currentPlayer->speed.direction.y = 0;
		ScenePrototypeInit();
	}
}

//---------------------------------------------------------
// Update the DigiPen Logo scene.
void ScenePrototypeUpdate()
{
	consoleCommands(); /* check for debug input */
	input(inputs);     /* get player input      */

	/* change player movement based on player input */
	changeSpeed(currentPlayer, inputs);
	changePos(currentPlayer, &map1);

	collisions(); /* check for collisions */
	drawScreen(); /* draw the new screen  */
	
	/* if outside the main menu */
	if (levelInc > 2)
	{
		speedrunUI(); /* start the speedrun text */
	}
}

/**************************************************************************
   Function: setLevel
Description: sets the game to a certain level
     Inputs: int level, desired level
    Outputs: the game is now on that level
**************************************************************************/
void setLevel(int level)
{
	if (level <= levelCount) /* if it is a created level last level */
	{
		/* get the level file */
		char buffer[21];
		sprintf_s(buffer, 21, "./Assets/level%i.txt", level);

		free(map1.bitmap); /* free memory for previous level */
		inirutine(buffer); /* create the new level           */

		if (level == 6) /* if it is the last level */
		{
			sound(win_sound); /* play the win sound */
		}
	}
	else /* if it is the last level */
	{
		/* get the first level file */
		char buffer[21];
		levelInc = startingLevel;
		sprintf_s(buffer, 21, "./Assets/level%i.txt", levelInc);

		/* reset speedrun */
		setSpeedInc(startingLevel - 1);
		timer = 0;

		file_Save();       /* save the current speedrun record           */
		file_Load();       /* replace the old record with the new record */
		free(map1.bitmap); /* free memory of last level                  */
		inirutine(buffer); /* create new level                           */
	}
}

//---------------------------------------------------------
// Private Functions:
//---------------------------------------------------------

