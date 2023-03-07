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
	player->setHandle(portionHandle, hacheteHandle,guiHandle,hpHandle,repairHandle,coinHandle);

	//空間のデータを作る
	for (auto& space : space) {
		space = std::make_shared<BugSpace>();
	}
	//プレイヤーのデータを送る
	for (auto& space : space) {
		space->setPlayer(player);
	}
	for (auto& space : space) {
		space->enemySetPlayer(enemyHandle,coinHandle);
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
}


void GameMain::init()
{

	player->init();

	int i = 0;
	
	for (int x = 0; x < bgNumX; x++) {
		for (int y = 0; y < bgNumY; y++) {
			const int chipNo = FieldData::tempField[y][x];
			if (chipNo == 4) {
				noLongerUsedX[i] = x;
				noLongerUsedY[i] = y;
				i++;
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

	i = 0;

	//空間の場所をランダムで設定
	for (auto& space : space) {
		space->init(noLongerUsedX[spacePosNum[i]], noLongerUsedY[spacePosNum[i]]);
		i++;
	}

	wave = 1;
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

	//空間の描画
	for (auto& space : space) {
		if (space->isEnable()) {
			space->draw(offset);
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

	DrawFormatString(600, 400, 0xffffff, "%d", quakeCount);
	DrawFormatString(600, 420, 0xffffff, "%d", clearCount);
	DrawFormatString(600, 440, 0xffffff, "%d", quakeTimer);

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
		for (auto& space : space) {
			if (space->isEnable()) {
				space->update(offset);
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
	

	clearCount = 0;

	if (quakeCount < 1) {
		for (auto& space : space) {
			if (!space->isEnable()) {
				clearCount++;
			}
		}
	}

	if (clearCount == 3) {
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

