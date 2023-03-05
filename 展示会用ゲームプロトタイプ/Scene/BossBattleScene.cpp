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

	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	hpHandle = my::myLoadGraph("data/objectGraph/heart.png");
	portionHandle = my::myLoadGraph("data/objectGraph/portion.png");
	hacheteHandle = my::myLoadGraph("data/objectGraph/machete.png");
	guiHandle = my::myLoadGraph("data/GUIGraph/GUI.png");
	repairHandle = my::myLoadGraph("data/objectGraph/repair.png");
	coinHandle = my::myLoadGraph("data/objectGraph/CopperCoin.png");

	mainSound = LoadSoundMem("data/music/Battle-Dawn.mp3");
	footstepSound = LoadSoundMem("data/soundEffect/small_explosion1.mp3");

	player = new Player(2);
	player->setHandle(portionHandle, hacheteHandle, guiHandle, hpHandle, repairHandle, coinHandle);
	player->init();
	player->setPos({Game::kScreenWidth / 2,760});

	field = std::make_shared<Field>();
	bossEnemy = std::make_shared <Enemy>(3);
	
	bossEnemy->setPlayer(player, enemyHandle, coinHandle);
	bossEnemy->dispatch({ 0,400 });

	init();
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
	DeleteGraph(coinHandle);

	DeleteSoundMem(mainSound); 
	DeleteSoundMem(footstepSound);
}

void BossBattleScene::init()
{
	ChangeVolumeSoundMem(soundVolume, mainSound);
	PlaySoundMem(mainSound, DX_PLAYTYPE_LOOP, true);
}

void BossBattleScene::update(const InputState& input)
{
	(this->*updateFunc)(input);
}

void BossBattleScene::draw()
{
	field->draw(offset,2);

	for (int x = 0; x < bossNumX; x++) {

		int posX = x * chipSize + static_cast<int>(offset.x) + graphChipSize;

		if (posX > Game::kScreenWidth + chipSize) {
			break;
		}
		if (posX < chipSize * 14) {
			continue;
		}

		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = groundData::bossGround[y][x];

			if (chipNo == 1) {
				my::myDrawRectRotaGraph(x * 32 + offset.x, y * 32 + 10, imgX * 40, 200, 40, 40, 3.0f, 0.0f, enemyHandle, true, true);
			}
		}
	}

	player->draw(offset);

	DrawFormatString(1100, 1000, 0xffffff, "%f", offset.x);

	if (bossEnemy->isEnable()) {
		bossEnemy->BossDraw(offset);
	}
	
	if (--time == 0) {
		imgX++;
		time = 4;
		if (imgX > 3) {
			imgX = 0;
		}
	}
}


void BossBattleScene::normalUpdate(const InputState& input)
{

	if (player->isEnable()) {
		player->update(offset, input);
		if (bossEnemy->isEnable()) {
			bossEnemy->update(offset);
		}
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

	if (!bossEnemy->isEnable()) {
		updateFunc = &BossBattleScene::clearFadeOutUpdate;
	}

	if (bossEnemy->isEnable()) {
		if (--soundCount == 0) {
			ChangeVolumeSoundMem(130, footstepSound);
			PlaySoundMem(footstepSound, DX_PLAYTYPE_BACK, true);
			soundCount = 100;
		}
	}
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
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, mainSound);
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new Gameover(manager_,input,2));
		return;
	}
}

void BossBattleScene::clearFadeOutUpdate(const InputState& input)
{
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, mainSound);
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new GameClear(manager_));
		return;
	}
}

