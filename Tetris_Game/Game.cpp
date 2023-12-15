#include "Game.h"

Game::Game() {
	grid = Grid ();
	bricks = { LBrick(), JBrick(), IBrick(), OBrick(), SBrick(), TBrick(), ZBrick() };
	curBrick = RandomBrick();
	nextBrick = RandomBrick();
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
	curBrick.Draw(11, 11);
	nextBrick.Draw(270, 270);
	if (gameOver) {
		DrawOver();
	}
}


void Game::MoveLeft() {
	if (gameOver == false) {
		curBrick.Move(0, -1);
		if (checkOutSide() || checkFit() == false) {
			curBrick.Move(0, 1);
		}
	}
}

void Game::MoveRight() {
	if (gameOver == false) {
		curBrick.Move(0, 1);
		if (checkOutSide() || checkFit() == false) {
			curBrick.Move(0, -1);
		}
	}
}

void Game::MoveDown() {
	if (gameOver == false) {
		curBrick.Move(1, 0);
		if (checkOutSide() || checkFit() == false) {
			curBrick.Move(-1, 0);
			LockBrick();
		}
	}
}

void Game::MoveDownInstant() {
	if (gameOver == false) {
		while (checkFit()) {
			curBrick.Move(1, 0);
		}
		if (checkOutSide() || checkFit() == false) {
			curBrick.Move(-1, 0);
			LockBrick();
		}
	}
}

void Game::HandleInput() {
	int keyPress = GetKeyPressed();
	/*if (gameOver && keyPress != 0) {
		gameOver = false;
		Reset();
	}*/
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

	//if (IsKeyDown(KEY_DOWN)) {
	//	MoveDown();
	//}
}

bool Game::checkOutSide() {
	vector <Position> temp = curBrick.GetCellPos();
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
		if (checkOutSide() ||checkFit () == false) {
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
	if (checkFit() == false) {
		gameOver = true;
	}
	nextBrick = RandomBrick();
	int numRowCleared = grid.ClearFullRow();
	UpdateScore(numRowCleared);
}


bool Game::GetGameOver() {
	return gameOver;
}

bool Game::checkFit() {
	vector <Position> temp = curBrick.GetCellPos();
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