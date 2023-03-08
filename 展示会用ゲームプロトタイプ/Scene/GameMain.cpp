#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"
#include"../field.h"
#include "SceneManager.h"
#include "Gameover.h"
#include "../InputState.h"
#include "GameClear.h"
#include "Pause.h"
#include "Shop.h"
#include "BossBattleScene.h"
#include "../DrawFunctions.h"
#include "../Object/ObjectHp.h"
#include <cassert>


GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc(&GameMain::fadeInUpdate)
{
	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	hpHandle = LoadGraph("data/objectGraph/heart.png");
	portionHandle = my::myLoadGraph("data/objectGraph/portion.png");
	hacheteHandle = my::myLoadGraph("data/objectGraph/machete.png");
	guiHandle = my::myLoadGraph("data/GUIGraph/GUI.png");
	repairHandle = my::myLoadGraph("data/objectGraph/repair.png");
	coinHandle = my::myLoadGraph("data/objectGraph/CopperCoin.png");
	truckHandle = my::myLoadGraph("data/objectGraph/truck.png");
	
	mainSound = LoadSoundMem("data/music/Dystopian.wav");
	footstepSound = LoadSoundMem("data/soundEffect/small_explosion1.mp3");

	player = new Player(0);
	hp = new ObjectHp;
	player->setHandle(portionHandle, hacheteHandle,guiHandle,hpHandle,repairHandle,coinHandle);

	for (int i = 0; i < maxWave; i++) {
		space.push_back(std::make_shared<BugSpace>());
	}
	
	field = std::make_shared<Field>();

	int sw, sh, bit;
	GetScreenState(&sw, &sh, &bit);
	tempScreenH = MakeScreen(sw, sh);
	assert(tempScreenH >= 0);

	init();
	
	ChangeVolumeSoundMem(soundVolume, mainSound);

	PlaySoundMem(mainSound, DX_PLAYTYPE_LOOP, true);
}

GameMain::~GameMain()
{
	delete player;
	delete hp;
	DeleteGraph(enemyHandle);
	DeleteGraph(portionHandle);
	DeleteGraph(guiHandle);
	DeleteGraph(hacheteHandle);
	DeleteGraph(hpHandle);
	DeleteGraph(repairHandle);
	DeleteGraph(coinHandle);
	DeleteGraph(truckHandle);
	DeleteGraph(tempScreenH);

	DeleteSoundMem(mainSound);
	DeleteSoundMem(footstepSound);

	for (int i = 0; i < maxWave; i++) {
		space[i]->init(0,0);
	}
}


void GameMain::init()
{

	player->init();

	int noLongerNum = 0;
	
	for (int x = 0; x < bgNumX; x++) {
		for (int y = 0; y < bgNumY; y++) {
			const int chipNo = FieldData::tempField[y][x];
			if (chipNo == 4) {
				noLongerUsedX[noLongerNum] = x;
				noLongerUsedY[noLongerNum] = y;
				noLongerNum++;
			}
		}
	}

	for (int i = 0; i < wave; i++) {
		spacePosNum[i] = GetRand(5);
		for (int j = 0; j < wave; j++) {
			if (i != j) {
				if (spacePosNum[i] == spacePosNum[j]) {
					i--;
				}
			}
		}
	}

	//空間の場所をランダムで設定
	if (wave <= maxWave) {
		for (int i = 0; i < wave; i++) {
			space[i]->init(noLongerUsedX[spacePosNum[i]], noLongerUsedY[spacePosNum[i]]);
			space[i]->setPlayer(player);
			space[i]->enemySetPlayer(enemyHandle, coinHandle);
			waveHp += space[i]->returnHp();
		}
		hp->setObjectMaxHp(waveHp);
	}
	else {
		for (int i = 0; i < wave - 1; i++) {
			if (!space[i]->isEnable()) {
				clearCount++;
			}
		}
		EndOfRaid = true;
	}
}

void GameMain::update(const InputState& input)
{
	(this->*updateFunc)(input);
}

void GameMain::draw()
{
	//加工用スクリーンハンドルをセット
	SetDrawScreen(tempScreenH);

	//フィールドの描画
	field->draw(offset,1);

	hp->waveHpDraw({Game::kScreenWidth / 4,100});

	//空間の描画
	if (!EndOfRaid) {
		for (int i = 0; i < wave; i++) {
			if (space[i]->isEnable()) {
				space[i]->draw(offset);
			}
		}
	}

	if (updateFunc != &GameMain::fadeInUpdate) {
		//プレイヤーの描画
		if (quakeCount > 3) {
			if (quakeTimer > -120) {
				nextScene = player->updateSwoon(offset);
			}
		}
		else {
			player->draw(offset);
		}
	}

	if (updateFunc == &GameMain::fadeInUpdate) {
		DrawGraph(truckPos, 643, truckHandle, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, quakeY, tempScreenH, false);

}

void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	truckPos += 10.0f;
	if (--fadeTimer == 0) {
		updateFunc = &GameMain::normalUpdate;
		fadeValue = 0;
		player->setPos({ truckPos + 20,720 });
	}
}

void GameMain::normalUpdate(const InputState& input)
{
	waveHp = 0;

	if (!EndOfRaid) {
		for (int i = 0; i < wave; i++) {
			if (space[i]->isEnable()) {
				waveHp += space[i]->returnHp();
			}
		}
	}
	
	hp->setObjectHp(waveHp);

	//プレイヤーの更新
	if (player->isEnable()) {
		if (quakeCount < 1) {
			player->update(offset, input);
		}
	}
	else {
		player->updateDeath(offset, input);
		if (player->nextScene()) {
			updateFunc = &GameMain::gameoverFadeOutUpdate;
		}
	}

	if (player->isEnable()) {
		//空間の更新
		if (!EndOfRaid) {
			for (int i = 0; i < wave; i++) {
				if (space[i]->isEnable()) {
					space[i]->update(offset);
				}
			}
		}
	}

	int x = (2130) / chipSize;
	int y = (780) / chipSize;

	if (player->shopCollision(x, y, offset)) {
		if (input.isTriggered(InputType::next))
		{
			manager_.pushScene(new Shop(manager_,input,player,portionHandle,guiHandle,hpHandle,repairHandle));
		}
	}
	
	waveCount = 0;

	if (!EndOfRaid) {
		for (int i = 0; i < wave; i++) {
			if (!space[i]->isEnable()) {
				waveCount++;
			}
		}
	}

	if (wave == waveCount) {
		wave++;
		init();
	}

	if (clearCount == 3) {
		clearCount--;
		quakeCount++;
		quakeTimer = 120;
		quakeY = 20.0f;
		ChangeVolumeSoundMem(footstepSoundVolume, footstepSound);
		PlaySoundMem(footstepSound, DX_PLAYTYPE_BACK, true);
		footstepSoundVolume += 10;
	}

	if (quakeCount == 3 && quakeTimer <= 0) {
		quakeCount++;
	}

	if (nextScene) {
		updateFunc = &GameMain::bossBattleSceneFadeOutUpdate;
	}

	if (quakeCount == 1) {
		if (quakeTimer < -80) {
			quakeCount++;
			quakeTimer = 120;
			quakeY = 40.0f;
			ChangeVolumeSoundMem(footstepSoundVolume, footstepSound);
			PlaySoundMem(footstepSound, DX_PLAYTYPE_BACK, true);
			footstepSoundVolume += 10;
		}
	}
	else if (quakeCount == 2) {
		if (quakeTimer < -80) {
			quakeCount++;
			quakeTimer = 120;
			quakeY = 60.0f;
			ChangeVolumeSoundMem(footstepSoundVolume, footstepSound);
			PlaySoundMem(footstepSound, DX_PLAYTYPE_BACK, true);
		}
	}

	{
		Vec2 targetOffset{};

		targetOffset.x = (Game::kScreenWidth / 2) - player->getPos().x;
		if (targetOffset.x > 0)
		{
			targetOffset.x = 0;
		}
		if (targetOffset.x < -field->getWidth() + Game::kScreenWidth + 16)
		{
			targetOffset.x = -field->getWidth() + Game::kScreenWidth + 16;
		}
		offset = targetOffset;
	}


	
	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new Pause(manager_,input));
	}

	if (--quakeTimer > 0) {
		quakeY = -quakeY;
		quakeY *= 0.95f;
		--quakeTimer;
	}
	else {
		quakeY = 0.0f;
	}

}

void GameMain::gameoverFadeOutUpdate(const InputState& input)
{
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, mainSound);
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new Gameover(manager_,input,1));
		return;
	}
}

void GameMain::bossBattleSceneFadeOutUpdate(const InputState& input)
{
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, mainSound);
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new BossBattleScene(manager_));
		return;
	}
}

