#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"
#include"../field.h"



GameMain::GameMain()
{
	enemyHandle = LoadGraph("data/enemy.png");
	playerHandle = LoadGraph("data/player.png");

	player = new Player;
	//空間のデータを作る
	for (int i = 0; i < 3;i++) {
		space[i] = std::make_shared<BugSpace>(i);
	}
	//プレイヤーのデータを送る
	for (auto& space : space) {
		space->setPlayer(player);
	}
	for (auto& space : space) {
		space->enemySetPlayer(enemyHandle);
	}
}

GameMain::~GameMain()
{
	delete player;
	DeleteGraph(enemyHandle);
}


void GameMain::init()
{
	//空間の場所をランダムで設定
	for (auto& space : space) {
		//space->setPos({static_cast<float>(GetRand(Game::kScreenWidth - 50)), 600.0f});
		space->init();
	}
}

void GameMain::update()
{
	//入力装置の情報を読み取る
	Pad::update();

	//プレイヤーの更新
	player->update();

	//空間の更新
	for (auto& space : space) {
		if (space->isEnable()) {
			space->update();
		}
	}

	//身を隠せる状態にいるかの正否を返す
	if (hiddenPlayer()) {
		player->setHidden(true);
	}
	else {
		player->setHidden(false);
	}

	//梯子に登れる状態なのか正否を返す
	if (ladderCollision()) {
		player->setLadder(true);
	}
	else {
		player->setLadder(false);
	}
}

void GameMain::draw()
{
	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 1) {
				DrawBox(x * Field::chipSize, y * Field::chipSize, x * Field::chipSize + Field::chipSize, y * Field::chipSize + Field::chipSize, 0xffff00, true);
			}
		}
	}

	//地面
	//DrawBox(0, 700, Game::kScreenWidth, Game::kScreenHeight, GetColor(255, 255, 0), true);
	DrawString(0, 700, "地面", 0x000000);

	DrawBox(100, ladderBlockY, ladderBlockX, ladderBlockY + 50, GetColor(255, 255, 0), true);
	DrawString(100, ladderBlockY, "地面", 0x000000);

	//隠れ場所
	DrawBox(hiddenBlockX, hiddenBlockY, hiddenBlockX + 100, hiddenBlockY + 100, GetColor(100, 255, 0), true);
	DrawString(hiddenBlockX, hiddenBlockY, "隠れ場所", 0x000000);

	//梯子
	DrawBox(ladderBlockX, ladderBlockY, ladderBlockX + 70, ladderBlockY + 500, GetColor(100, 100, 100), true);
	DrawString(ladderBlockX, ladderBlockY, "梯子", 0xffffff);

	//空間の描画
	for (auto& space : space) {
		if (space->isEnable()) {
			space->draw();
		}
	}

	//プレイヤーの描画
	player->draw(playerHandle);

}

struct ColData {
	int left;
	int top;
	int right;
	int bottom;
};

//隠れ場所のデータ
ColData colData[] = {
	{600,600,700,700}
};

//梯子のデータ
ColData ladderColData[] = {
	{300,200,370,700}
};

//隠れ場所とプレイヤーの当たり判定
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

//梯子とプレイヤーの当たり判定
bool GameMain::ladderCollision()
{
	float playerLeft = player->getPos().x + 25;
	float playerRight = player->getPos().x + 26;
	float playerTop = player->getPos().y;
	float playerBottom = player->getPos().y + 74;

	for (ColData data : ladderColData) {
		if (playerRight < data.left)continue;
		if (playerLeft > data.right)continue;
		if (playerBottom < data.top)continue;
		if (playerTop > data.bottom)continue;

		return true;
	}
	return false;
}
