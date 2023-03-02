#include "BossBattleScene.h"
#include "SceneManager.h"
#include "GameMain.h"
#include "GameClear.h"
#include "Gameover.h"
#include "../Object/Player.h"
#include "../InputState.h"
#include "../game.h"
#include "../field.h"
#include "../Object/Enemy.h"
#include "../DrawFunctions.h"
#include "DxLib.h"

BossBattleScene::BossBattleScene(SceneManager& manager) : SceneBase(manager),updateFunc(&BossBattleScene::fadeInUpdate)
{

	enemyHandle = my::myLoadGraph("data/enemy.png");
	hpHandle = my::myLoadGraph("data/heart.png");
	portionHandle = my::myLoadGraph("data/portion.png");
	hacheteHandle = my::myLoadGraph("data/machete.png");
	guiHandle = my::myLoadGraph("data/GUI.png");
	repairHandle = my::myLoadGraph("data/repair.png");
	bugHandle = my::myLoadGraph("data/Bag.png");
	bottanHandle = my::myLoadGraph("data/bottan1.png");
	bottanHandle2 = my::myLoadGraph("data/bottan2.png");
	bottanHandle3 = my::myLoadGraph("data/bottan3.png");
	coinHandle = my::myLoadGraph("data/CopperCoin.png");

	player = new Player(2);
	player->setHandle(portionHandle, hacheteHandle, guiHandle, hpHandle, repairHandle, bugHandle);
	player->init();
	player->setPos({Game::kScreenWidth / 2,600});

	field = std::make_shared<Field>();
	bossEnemy = std::make_shared <Enemy>(3);
	
	bossEnemy->setPlayer(player, enemyHandle, coinHandle);
	bossEnemy->dispatch({ 0,0 });

}

BossBattleScene::~BossBattleScene()
{
	delete player;
	DeleteGraph(enemyHandle);
	DeleteGraph(portionHandle);
	DeleteGraph(guiHandle);
	DeleteGraph(hacheteHandle);
	DeleteGraph(hpHandle);
	DeleteGraph(repairHandle);
	DeleteGraph(bugHandle);
	DeleteGraph(bottanHandle);
	DeleteGraph(bottanHandle2);
	DeleteGraph(bottanHandle3);
	DeleteGraph(coinHandle);
}

void BossBattleScene::init()
{
	player->init();
}

void BossBattleScene::update(const InputState& input)
{
	(this->*updateFunc)(input);
}

void BossBattleScene::draw()
{
	field->draw(offset,2);
	player->draw(offset);
	bossEnemy->BossDraw(offset);
	
	if (--time == 0) {
		imgX++;
		time = 4;
		if (imgX > 3) {
			imgX = 0;
		}
	}

	for (int x = 0; x < bossNumX; x++) {

		int posX = x * chipSize + static_cast<int>(offset.x) + graphChipSize;

		if (posX > Game::kScreenWidth + chipSize) {
			break;
		}
		if (posX < -chipSize) {
			continue;
		}

		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = groundData::bossGround[y][x];

			if (chipNo == 1) {
				my::myDrawRectRotaGraph(x * 32 + offset.x, y * 32 + 10, imgX * 40, 200, 40, 40, 3.0f, 0.0f, enemyHandle, true, true);
			}
		}
	}
}


void BossBattleScene::normalUpdate(const InputState& input)
{

	if (player->isEnable()) {
		player->update(offset, input);
		bossEnemy->update(offset);
	}
	else {
		player->updateDeath(offset,input);
		if (player->nextScene()) {
			updateFunc = &BossBattleScene::gameoverFadeOutUpdate;
		}
	}

	Vec2 targetOffset{};

	targetOffset.x = 0 - bossEnemy->getPos().x;
	if (targetOffset.x > 0)
	{
		targetOffset.x = 0;
	}
	if (targetOffset.x < -field->getBossWidth() + Game::kScreenWidth)
	{
		targetOffset.x = -field->getBossWidth() + Game::kScreenWidth;
	}

	offset = targetOffset * 0.5f + offset * 0.5f;

}

void BossBattleScene::fadeInUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (--fadeTimer == 0) {
		updateFunc = &BossBattleScene::normalUpdate;
		fadeValue = 0;
	}
}

void BossBattleScene::gameoverFadeOutUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new Gameover(manager_,input,2));
		return;
	}
}

void BossBattleScene::clearFadeOutUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new GameClear(manager_));
		return;
	}
}

