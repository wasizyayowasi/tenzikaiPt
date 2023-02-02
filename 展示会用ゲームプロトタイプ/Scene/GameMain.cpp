#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"

GameMain::GameMain()
{
	player = new Player;
	for (int i = 0; i < 3;i++) {
		space[i] = std::make_shared<BugSpace>(i);
	}
	for (auto& space : space) {
		space->setPlayer(player);
	}
	for (auto& space : space) {
		space->enemySetPlayer();
	}
}

void GameMain::init()
{
	for (auto& space : space) {
		space->setPos({static_cast<float>(GetRand(Game::kScreenWidth - 50)), 600.0f});
		
	}
}

void GameMain::update()
{
	Pad::update();

	if (Pad::isPress(PAD_INPUT_RIGHT)) {
		fieldX -= 10;
	}
	if (Pad::isPress(PAD_INPUT_LEFT)) {
		fieldX += 10;
	}

	player->update();

	for (auto& space : space) {
		space->update();
	}

	if (hiddenPlayer()) {
		player->setHidden(true);
	}
	else {
		player->setHidden(false);
	}

	if (ladderCollision()) {
		player->setLadder(true);
	}
	else {
		player->setLadder(false);
	}
}

void GameMain::draw()
{
	//’n–Ê
	DrawBox(0, 700, Game::kScreenWidth, Game::kScreenHeight, GetColor(255, 255, 0), true);
	DrawString(0, 700, "’n–Ê", 0x000000);

	//‰B‚êêŠ
	DrawBox(hiddenBlockX, hiddenBlockY, hiddenBlockX + 100, hiddenBlockY + 100, GetColor(100, 255, 0), true);
	DrawString(hiddenBlockX, hiddenBlockY, "‰B‚êêŠ", 0x000000);

	//’òŽq
	DrawBox(ladderBlockX, ladderBlockY, ladderBlockX + 70, ladderBlockY + 500, GetColor(100, 100, 100), true);
	DrawString(ladderBlockX, ladderBlockY, "’òŽq", 0xffffff);

	for (auto& space : space) {
		space->draw();
	}

	player->draw();

}

struct ColData {
	int left;
	int top;
	int right;
	int bottom;
};

ColData colData[] = {
	{600,600,700,700}
};

ColData ladderColData[] = {
	{300,200,370,700}
};

bool GameMain::hiddenPlayer() {

	float playerLeft = player->getPos().x;
	float playerRight = player->getPos().x + 50;
	float playerTop = player->getPos().y;
	float playerBottom = player->getPos().y + 64;

	for (ColData data : colData) {
		if (playerRight < data.left)continue;
		if (playerLeft > data.right)continue;
		if (playerBottom < data.top)continue;
		if (playerTop > data.bottom)continue;
		return true;
	}
	return false;
}

bool GameMain::ladderCollision()
{
	float playerLeft = player->getPos().x + 25;
	float playerRight = player->getPos().x + 26;
	float playerTop = player->getPos().y;
	float playerBottom = player->getPos().y + 64;

	for (ColData data : ladderColData) {
		if (playerRight < data.left)continue;
		DrawString(300, 0, "aiu1", 0xffffff);
		if (playerLeft > data.right)continue;
		DrawString(300, 15, "aiu1", 0xffffff);
		if (playerBottom < data.top)continue;
		DrawString(300, 30, "aiu1", 0xffffff);
		if (playerTop > data.bottom)continue;
		DrawString(300, 45, "aiu1", 0xffffff);
		return true;
	}
	return false;
}
