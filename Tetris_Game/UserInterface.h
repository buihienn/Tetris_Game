#pragma once
#include <raylib.h>
#include "game.h"
#include "time.h"
#include <string>
bool Trigger(double interval, double& lastUpdateTime);
void UserInterface(Game &game, double lastUpdateTime);

