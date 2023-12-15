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

void GamePlay(Game& game, bool gameOverButtonPressed, double& lastUpdateTime, Texture2D texture, GameState& gameState, Font& font) {
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
    Vector2 tetrisPosition = { 120, 45 };
    Vector2 tetrisSize = { 300, 600 };
    Color borderColor = BLACK;
    // Draw border Grid
    DrawTetrisBox(tetrisPosition, tetrisSize, borderColor, 5);
    DrawTextEx(font, "Score", { 550,155 }, 38, 2, ORANGE);
    DrawTextEx(font, "Next", { 565,275 }, 38, 2, ORANGE);

    // Create Rectangle to show score and next brick
    DrawRectangleRounded({ 520,195,170,60 }, 0.3, 6, GRAY);
    DrawRectangleRounded({ 520,315,170,180 }, 0.3, 6, GRAY);

    // Solve level 
    if (game.GetScore() < 10) DrawTextEx(font, (to_string(game.GetScore()).c_str()), { 595,202 }, 50, 2, YELLOW);
    if (game.GetScore() >= 100 && game.GetScore() < 1000) DrawTextEx(font, (to_string(game.GetScore()).c_str()), { 572,202 }, 50, 2, YELLOW);
    if (game.GetScore() >= 1000) DrawTextEx(font, (to_string(game.GetScore()).c_str()), { 560,202 }, 50, 2, YELLOW);

    game.Draw();
    if (game.GetGameOver() == true)
    {
        gameState = GAMEOVER;
    }
    EndDrawing();
}

void DrawGameOver(Game& game, const float screenWidth, const float screenHeight, GameState& gameState, Font& font) {
    BeginDrawing();
    ClearBackground(WHITE);

    Image image4 = LoadImage("Picture/game_over.png");
    Texture2D texture4 = LoadTextureFromImage(image4);
    UnloadImage(image4);

    DrawTexture(texture4, 0, 0, WHITE);


    if (game.GetScore() < 10) DrawTextEx(font, (to_string(game.GetScore()).c_str()), { screenWidth / 2 - 20, screenHeight / 2 + 75 }, 100, 2, YELLOW);
    if (game.GetScore() >= 100 && game.GetScore() < 1000) DrawTextEx(font, (to_string(game.GetScore()).c_str()), { screenWidth / 2 - 63, screenHeight / 2 + 75 }, 100, 2, YELLOW);
    if (game.GetScore() >= 1000) DrawTextEx(font, (to_string(game.GetScore()).c_str()), { screenWidth / 2 - 90, screenHeight / 2 + 75 }, 100, 2, YELLOW);

    Rectangle retryButtonBounds = { screenWidth / 2 - 92, screenHeight / 2 + 225, 70, 70 };
    Rectangle exitButtonBounds = { screenWidth / 2 + 33, screenHeight / 2 + 225, 70, 70 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), retryButtonBounds)) {
            gameState = GAMEPLAY;
            game.Reset();
        }
        else if (CheckCollisionPointRec(GetMousePosition(), exitButtonBounds)) {
            EndDrawing();
            //CloseWindow();
            gameState = EXIT;
        }   
    }

    EndDrawing();
}

void DrawTetrisBox(Vector2 position, Vector2 size, Color borderColor, int borderWidth) {

    for (int i = 0; i < borderWidth; ++i) {
        DrawRectangleLines((int)position.x - i, (int)position.y - i, (int)size.x + i * 2, (int)size.y + i * 2, borderColor);
    }
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

        case EXIT:
            CloseWindow();
            return;
        }

        CloseWindow();
    }
}