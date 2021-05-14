//---------------------------------------------------------
// file:	Scene_Prototype.h
// author:	Team
// brief:	DigiPen Logo scene functions
// Course:  GAM100F18
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "Physics.h"
#include "Collision.h"
#include "platform.h"
#include "position.h"
#include "Player.h"
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
// Public Functions:
//---------------------------------------------------------

void ScenePrototypeInit();
void ScenePrototypeUpdate();

void initPlayer(MAP * map);