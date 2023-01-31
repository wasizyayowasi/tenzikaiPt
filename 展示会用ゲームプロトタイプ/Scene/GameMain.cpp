#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"

GameMain::GameMain()
{
	player = new Player;
	space = new BugSpace;
	
	space->setPlayer(player);
	space->enemySetPlayer();
}

void GameMain::update()
{
	player->update();
	space->update();
	if (hiddenPlayer()) {
		DrawString(0, 135, "hidden", GetColor(255, 255, 255));
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

	DrawFormatString(0, 0, GetColor(255, 255, 255), "%f:%f", player->getPos().x, player->getPos().y);
	player->draw();

	space->draw();
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