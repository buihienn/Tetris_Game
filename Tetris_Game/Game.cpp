#include "Game.h"

Game::Game() {
	grid = Grid ();
	bricks = { LBrick(), JBrick(), IBrick(), OBrick(), SBrick(), TBrick(), ZBrick() };
	curBrick = RandomBrick();
	nextBrick = RandomBrick();
	ghostBrick = curBrick;
	ghostBrick.TurnToGhost();
	gameOver = false;
	score = 0;
	InitAudioDevice();
	music = LoadMusicStream("Sound/music.mp3");
	PlayMusicStream(music);
	rotateSound = LoadSound("Sound/rotate.mp3");
	clearSound = LoadSound("Sound/clear.mp3");
	moveSound = LoadSound("Sound/menumove.wav");
}

Game::~Game() {
	UnloadMusicStream(music);
	CloseAudioDevice();
}

Brick Game::GetCurBrick() {
	return curBrick;
}

Grid Game::GetGrid() {
	return grid;
}

Brick Game::RandomBrick() {
	if (bricks.empty()) {
		bricks = { LBrick(), JBrick(), IBrick(), OBrick(), SBrick(), TBrick(), ZBrick() };
	}
	int randomIndex = rand() % bricks.size();
	Brick brick = bricks[randomIndex];
	bricks.erase(bricks.begin() + randomIndex);
	return brick;
}

void Game::Draw() {
	grid.DrawGrid();
	curBrick.Draw(120, 45);
	DrawNextBrick(nextBrick.GetID());
	ghostBrick = curBrick;
	GhostMove();
	if (gameOver) {
		DrawOver();
	}
}


void Game::MoveLeft() {
	if (gameOver == false) {
		curBrick.Move(0, -1);
		if (checkOutSide(curBrick) || checkFit(curBrick) == false) {
			curBrick.Move(0, 1);
		}
	}
}

void Game::MoveRight() {
	if (gameOver == false) {
		curBrick.Move(0, 1);
		if (checkOutSide(curBrick) || checkFit(curBrick) == false) {
			curBrick.Move(0, -1);
		}
	}
}

void Game::MoveDown() {
	if (gameOver == false) {
		curBrick.Move(1, 0);
		if (checkOutSide(curBrick) || checkFit(curBrick) == false) {
			curBrick.Move(-1, 0);
			LockBrick();
		}
	}
}

void Game::MoveDownInstant() {
	if (gameOver == false) {
		while (checkFit(curBrick)) {
			curBrick.Move(1, 0);
		}
		if (checkOutSide(curBrick) || checkFit(curBrick) == false) {
			curBrick.Move(-1, 0);
			LockBrick();
		}
	}
}

void Game::HandleInput() {
	int keyPress = GetKeyPressed();
	if (keyPress == KEY_LEFT) {
		MoveLeft();
		PlaySound(moveSound);
	}
	else if (keyPress == KEY_RIGHT) {
		MoveRight();
		PlaySound(moveSound);
	}
	else if (keyPress == KEY_DOWN) {
		MoveDown();
		PlaySound(moveSound);
	}
	else if (keyPress == KEY_UP) {
		RotateBrick();
		PlaySound(rotateSound);
	}
	else if (keyPress == KEY_SPACE) {
		MoveDownInstant();
	}

}

bool Game::checkOutSide(Brick brick) {
	vector <Position> temp = brick.GetCellPos();
	for (int i = 0; i < temp.size(); i++) {
		if (grid.checkOutSide(temp[i].getRow(), temp[i].getCol())) {
			return true;
		}
	}
	return false;
}

void Game::RotateBrick() {
	if (gameOver == false) {
		curBrick.Rotate();
		if (checkOutSide(curBrick) ||checkFit (curBrick) == false) {
			curBrick.Unrotate();
		}
	}
}


void Game::LockBrick() {
	vector <Position> temp = curBrick.GetCellPos();
	for (int i = 0; i < temp.size(); i++) {
		grid.grid[temp[i].getRow()][temp[i].getCol()] = curBrick.GetID();
	}
	curBrick = nextBrick;
	if (checkFit(curBrick) == false) {
		gameOver = true;
	}
	nextBrick = RandomBrick();
	int numRowCleared = grid.ClearFullRow();
	UpdateScore(numRowCleared);
}


bool Game::GetGameOver() {
	return gameOver;
}

bool Game::checkFit(Brick brick) {
	vector <Position> temp = brick.GetCellPos();
	for (int i = 0; i < temp.size(); i++) {
		if (grid.checkEmpty(temp[i].getRow(), temp[i].getCol()) == false) {
			return false;
		}
	}
	return true;
}


void Game::Reset() {
	grid = Grid();
	bricks = { LBrick(), JBrick(), IBrick(), OBrick(), SBrick(), TBrick(), ZBrick() };
	curBrick = RandomBrick();
	nextBrick = RandomBrick();
	score = 0;
	gameOver = false;
}

void Game::UpdateScore(int numLineCleared) {
	if (numLineCleared == 1)
		score += 100;
	else if (numLineCleared == 2)
		score += 200;
	else if (numLineCleared == 3)
		score += 400;
	else if (numLineCleared == 4)
		score += 1000;
	else {

	}
}

int Game::GetScore() {
	return score;
}

void Game::DrawOver() {
	grid.DrawWhite();
}

void Game::SetGameOver(bool temp) {
	gameOver = temp;
}

void Game::GhostMove() {
	while (checkFit(ghostBrick) && !checkOutSide(ghostBrick)) {
		ghostBrick.Move(1, 0);
	}

	if (checkOutSide(ghostBrick) || checkFit(ghostBrick) == false) {
		ghostBrick.Move(-1, 0);
	}
	ghostBrick.Draw(120, 45);
}

void Game::DrawNextBrick(int ID) {
	if (ID == 1) { // L
		nextBrick.Draw(475, 375);
	}
	else if (ID == 2) { // J
		nextBrick.Draw(475, 375);
	}
	else if (ID == 3) { // I
		nextBrick.Draw(457, 395);
	}
	else if (ID == 4) { // O
		nextBrick.Draw(458, 380);
	}
	else if (ID == 5) { // S
		nextBrick.Draw(473, 375);
	}
	else if (ID == 6) { // T
		nextBrick.Draw(473, 375);
	}
	else if (ID == 7) { // Z
		nextBrick.Draw(470, 375);
	}
}

