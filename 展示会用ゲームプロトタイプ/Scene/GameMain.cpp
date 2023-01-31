#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"

GameMain::GameMain()
{
	player = new Player;
	for (auto& space : space) {
		space = std::make_shared<BugSpace>();
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
		space->setPos({x, y});
		x += 100.0f;
		y += 100.0f;
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
		player->hiddenBlockTrue();
	}
	else {
		player->hiddenBlockFalse();
	}
}

void GameMain::draw()
{
	//’n–Ê
	DrawBox(0, 700, Game::kScreenWidth, Game::kScreenHeight, GetColor(255, 255, 0), true);

	//‰B‚êêŠ
	DrawBox(hiddenBlockX, hiddenBlockY, hiddenBlockX + 100, hiddenBlockY + 100, GetColor(100, 255, 0), true);

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