#include "UserInterface.h"

bool Trigger(double interval, double& lastUpdateTime) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void DrawMenu(Music& menu_music, const float screenWidth, const float screenHeight, Sound& mousePressed, Texture2D& texture2, GameState& gameState) {
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

void GamePlay(Game& game, bool gameOverButtonPressed, double &lastUpdateTime, Texture2D texture, GameState& gameState, Font &font) {
    SetTargetFPS(60);

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
}


//void DrawGameOver(Game& game, const float screenWidth, const float screenHeight, GameState& gameState) {
//    // Render game-over screen
//    BeginDrawing();
//
//    ClearBackground(WHITE);
//    DrawText("GAME OVER", 320, 200, 30, RED);
//
//    // Draw a button to return to the menu
//    Rectangle returnButtonBounds = { screenWidth / 2 - 50, screenHeight / 2 + 50, 100, 40 };
//    DrawRectangleRec(returnButtonBounds, LIGHTGRAY);
//
//    if (CheckCollisionPointRec(GetMousePosition(), returnButtonBounds)) {
//        DrawRectangleLinesEx(returnButtonBounds, 2, RED);
//        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
//            gameState = GAMEPLAY; // Transition to MENU state if button is clicked
//
//            game.Reset();
//        }
//    }
//    else {
//        DrawRectangleLinesEx(returnButtonBounds, 2, DARKGRAY);
//    }
//
//    DrawText("Return to Menu", returnButtonBounds.x + 5, returnButtonBounds.y + 10, 20, DARKGRAY);
//
//    EndDrawing();
//}

void DrawGameOver(Game& game, const float screenWidth, const float screenHeight, GameState& gameState, Font &font) {
    BeginDrawing();
    ClearBackground(WHITE);

    Image image4 = LoadImage("Picture/game_over.png");
    Texture2D texture4 = LoadTextureFromImage(image4);
    UnloadImage(image4);

    DrawTexture(texture4, 0, 0, WHITE);
    DrawTextEx(font, (to_string(game.GetScore()).c_str()), { screenWidth / 2 - 15, screenHeight / 2 + 60 }, 100, 2, YELLOW);

    Rectangle retryButtonBounds = { screenWidth / 2 - 92, screenHeight / 2 + 225, 70, 70 };
    Rectangle exitButtonBounds = { screenWidth / 2 + 33, screenHeight / 2 + 225, 70, 70 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), retryButtonBounds)) {
            gameState = GAMEPLAY;
            game.Reset();
        }
        else if (CheckCollisionPointRec(GetMousePosition(), exitButtonBounds)) {
            EndDrawing();
            CloseWindow();
            return;
        }
    }
    EndDrawing();
}



void UserInterface(Game& game, double lastUpdateTime) {
    const float screenWidth = 820;
    const float screenHeight = 680;

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


    bool gameOverButtonPressed = false;
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    while (!WindowShouldClose()) {

        switch (gameState) {
        case MENU:
            DrawMenu(menu_music, screenWidth, screenHeight, mousePressed, texture2, gameState);
            break;
        case RULES:
            BeginDrawing();

            DrawTexture(texture3, 0, 0, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                gameState = MENU;

            EndDrawing();
            break;
        case GAMEPLAY:

            GamePlay(game, gameOverButtonPressed, lastUpdateTime, texture, gameState, font);
            break;

        case GAMEOVER:
            DrawGameOver(game, screenWidth, screenHeight, gameState, font);

            break;
        }

        CloseWindow();
    }
}