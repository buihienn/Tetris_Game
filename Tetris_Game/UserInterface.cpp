#include "UserInterface.h"

bool Trigger(double interval, double& lastUpdateTime) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void DrawMenu(Music &menu_music, const int screenWidth, const int screenHeight, Sound &mousePressed, Texture2D &texture2, GameState &gameState) {
    BeginDrawing();
    //InitAudioDevice();

    PlayMusicStream(menu_music);
    UpdateMusicStream(menu_music);
    ClearBackground(RAYWHITE);
    DrawTexture(texture2, 0, 0, WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        Rectangle startButtonBounds = { screenWidth / 2 - 115, screenHeight / 2 + 10, 200, 50 };
        Rectangle rulesButtonBounds = { screenWidth / 2 - 100, screenHeight / 2 + 90, 200, 50 };
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, startButtonBounds)) {
            PlaySound(mousePressed);
            gameState = GAMEPLAY;
        }
        if (CheckCollisionPointRec(mousePoint, rulesButtonBounds)) {
            PlaySound(mousePressed);
            gameState = RULES;
        }
    }
    EndDrawing();
}

void UserInterface(Game& game, double lastUpdateTime) {
    const int screenWidth = 820;
    const int screenHeight = 680;

    Sound mousePressed = LoadSound("Sound/mouse.wav");
    Music menu_music = LoadMusicStream("Sound/menu_music.mp3");
    
    InitWindow(screenWidth, screenHeight, "Menu");
    GameState gameState = MENU;
    Image image = LoadImage("Picture/nen.png");
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    
    Image image2 = LoadImage("Picture/nen_menu.png");
    Texture2D texture2 = LoadTextureFromImage(image2);
    UnloadImage(image2);

    Image image3 = LoadImage("Picture/rules.png");
    Texture2D texture3 = LoadTextureFromImage(image3);
    UnloadImage(image3);

    bool gameOverButtonPressed;

    while (!WindowShouldClose()) {

        switch (gameState) {
        case MENU:
            DrawMenu(menu_music, screenWidth, screenHeight, mousePressed, texture2, gameState);
            break;
        case RULES:
            BeginDrawing();
            DrawTexture(texture3, 0, 0, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) gameState = MENU;

            EndDrawing();
            break;
        case GAMEPLAY:
              
            
            //InitWindow(500, 710, "Tetris");
            SetTargetFPS(60);

            /* Color darkBlue = { 44, 44, 127, 255 };
             Color purple = { 128, 0, 128, 255 };*/
            gameOverButtonPressed = false;


            game.HandleInput();
            UpdateMusicStream(game.music);

            if (Trigger(0.75, lastUpdateTime)) {
                game.MoveDown();
            }

            BeginDrawing();
            //ClearBackground(darkBlue);
            DrawTexture(texture, 0, 0, WHITE);
            //ClearBackground(purple);

            DrawText(TextFormat("Score: %d", game.GetScore()), 320, 80, 20, RED);
            DrawText(TextFormat("NEXT "), 350, 150, 20, BLUE);
            
            
            
            game.Draw();
            if (game.GetGameOver() == true)
            {
                gameState = GAMEOVER;
            }
            EndDrawing();
            break;

        case GAMEOVER:
            
            // Render game-over screen
            BeginDrawing();
            //DrawTexture(texture2, 0, 0, WHITE);
            ClearBackground(WHITE);
            DrawText("GAME OVER", 320, 200, 30, RED);

            // Draw a button to return to the menu
            Rectangle returnButtonBounds = { screenWidth / 2 - 50, screenHeight / 2 + 50, 100, 40 };
            DrawRectangleRec(returnButtonBounds, LIGHTGRAY);

            if (CheckCollisionPointRec(GetMousePosition(), returnButtonBounds)) {
                DrawRectangleLinesEx(returnButtonBounds, 2, RED);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    gameState = GAMEPLAY; // Transition to MENU state if button is clicked
                    
                    game.Reset();
                }
            }
            else {
                DrawRectangleLinesEx(returnButtonBounds, 2, DARKGRAY);
            }

            DrawText("Return to Menu", returnButtonBounds.x + 5, returnButtonBounds.y + 10, 20, DARKGRAY);

            EndDrawing();
            
            break;
        }

        CloseWindow();
    }
}