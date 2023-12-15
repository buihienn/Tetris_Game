#pragma once
#include <raylib.h>
#include "game.h"
#include "time.h"
#include <string>
bool Trigger(double interval, double& lastUpdateTime);
void UserInterface(Game &game, double lastUpdateTime);
void DrawGameOver(Game& game, const float screenWidth, const float screenHeight, GameState& gameState, Font& font);
void GamePlay(Game& game, bool gameOverButtonPressed, double& lastUpdateTime, Texture2D texture, GameState& gameState, Font& font);
void DrawMenu(Music& menu_music, const float screenWidth, const float screenHeight, Sound& mousePressed, Texture2D& texture2, GameState& gameState);
void DrawTetrisBox(Vector2 position, Vector2 size, Color borderColor, int borderWidth);