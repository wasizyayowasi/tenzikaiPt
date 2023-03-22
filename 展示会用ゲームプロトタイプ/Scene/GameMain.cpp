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

namespace {
	int graphSize = 16;;
}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc(&GameMain::fadeInUpdate)
{
	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	hpHandle = LoadGraph("data/objectGraph/heart.png");
	portionHandle = my::myLoadGraph("data/objectGraph/portion.png");
	macheteHandle = my::myLoadGraph("data/objectGraph/machete.png");
	guiHandle = my::myLoadGraph("data/GUIGraph/GUI.png");
	repairHandle = my::myLoadGraph("data/objectGraph/repair.png");
	coinHandle = my::myLoadGraph("data/objectGraph/CopperCoin.png");
	truckHandle = my::myLoadGraph("data/objectGraph/truck.png");
	rockHandle = my::myLoadGraph("data/objectGraph/rock.png");
	bottanHandle = my::myLoadGraph("data/GUIGraph/bottan.png");
	
	footstepSound = LoadSoundMem("data/soundEffect/small_explosion1.mp3");
	appearanceSound = LoadSoundMem("data/soundEffect/appearance.mp3");

	LPCSTR UIfontPath = "data/other/Silver.ttf";
	AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL);
	UIFontHandle = CreateFontToHandle("Silver", 48, 9, -1);

	player = new Player(0);
	hp = new ObjectHp;
	player->setHandle(portionHandle, macheteHandle,guiHandle,hpHandle,repairHandle,coinHandle);

	for (int i = 0; i < maxWave; i++) {
		space.push_back(std::make_shared<BugSpace>());
	}
	
	field = std::make_shared<Field>();

	int sw, sh, bit;
	GetScreenState(&sw, &sh, &bit);
	tempScreenH = MakeScreen(sw, sh);
	assert(tempScreenH >= 0);

	init();
}

GameMain::~GameMain()
{
	delete player;
	delete hp;
	DeleteGraph(enemyHandle);
	DeleteGraph(portionHandle);
	DeleteGraph(guiHandle);
	DeleteGraph(macheteHandle);
	DeleteGraph(hpHandle);
	DeleteGraph(rockHandle);
	DeleteGraph(repairHandle);
	DeleteGraph(coinHandle);
	DeleteGraph(truckHandle);
	DeleteGraph(tempScreenH);
	DeleteGraph(bottanHandle);

	DeleteFontToHandle(UIFontHandle);

	DeleteSoundMem(mainSound);
	DeleteSoundMem(footstepSound);
	DeleteSoundMem(appearanceSound);

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
		hp->setWaveHp(static_cast<int>(waveHp));
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

	if (quakeCount < 1) {
		hp->waveHpDraw({ Game::kScreenWidth / 4,100 });
		int waveFontSize = GetDrawFormatStringWidthToHandle(UIFontHandle, "%dウェーブ( %d / %d )", wave, wave, maxWave);
		DrawFormatStringToHandle(Game::kScreenWidth / 2 - waveFontSize / 2 + 2, 32, 0x000000, UIFontHandle, "%dウェーブ( %d / %d )", wave, wave, maxWave);
		DrawFormatStringToHandle(Game::kScreenWidth / 2 - waveFontSize / 2, 30, 0xffffff, UIFontHandle, "%dウェーブ( %d / %d )", wave, wave, maxWave);
	}

	//空間の描画
	if (startWave) {
		if (!EndOfRaid) {
			for (int i = 0; i < wave; i++) {
				if (space[i]->isEnable()) {
					space[i]->draw(offset);
				}
			}
		}
	}

	if (updateFunc != &GameMain::fadeInUpdate && !bossWave) {
		//プレイヤーの描画
		if (!hitRock) {
			player->draw(offset);
		}
		else {
			bossWave = player->updateSwoon(offset);
		}
	}

	if (updateFunc == &GameMain::fadeInUpdate) {
		DrawGraphF(truckPos, 643, truckHandle, true);
	}

	if (!hitRock) {
		if (updateFunc == &GameMain::gameClearIntroduction) {
			if (!player->rockCollision({ player->getPos().x,player->getPos().y - rockHeight })) {
				DrawRotaGraphF(player->getPos().x + offset.x, player->getPos().y - rockHeight, 2.0f, 0.0f, rockHandle, true, false);
				rockHeight -= 1.5f;
			}
			else {
				hitRock = true;
			}
		}
	}

	if (quakeCount >= 3 && quakeTimer < 0) {
		nextScene = true;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraphF(0, quakeY, tempScreenH, false);

}

void GameMain::gameClearIntroduction(const InputState& input)
{
	if (hitRock) {
		if (bossWave) {
			updateFunc = &GameMain::bossBattleSceneFadeOutUpdate;
		}
	}
}

void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	truckPos += 10.0f;
	if (--fadeTimer == 0) {
		updateFunc = &GameMain::normalUpdate;
		fadeValue = 0;
		PlayMusic("data/music/Dystopian.wav", DX_PLAYTYPE_LOOP);
		player->setPos({ truckPos + 20,720 });
	}
}

void GameMain::normalUpdate(const InputState& input)
{
	if (!startWave && !(clearCount >= 2)) {
		if (!(hp->chargeHp(wave))) {
			startWave = false;
		}
		else {
			startWave = true;
			ChangePanSoundMem(soundVolume, appearanceSound);
			PlaySoundMem(appearanceSound, DX_PLAYTYPE_BACK);
		}
	}

	waveHp = 0;

	if (musicVolume < 180) {
		musicVolume++;
		SetVolumeMusic(musicVolume);
	}

	if (!EndOfRaid) {
		for (int i = 0; i < wave; i++) {
			if (space[i]->isEnable()) {
				waveHp += space[i]->returnHp();
			}
		}
	}	

	if (startWave) {
		hp->setObjectHp(waveHp);
	}

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

	if (startWave) {
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
	}

	int x = (2130) / chipSize;
	int y = (780) / chipSize;

	if (player->shopCollision(x, y, offset)) {
		if (input.isTriggered(InputType::next))
		{
			manager_.pushScene(new Shop(manager_,input,player,portionHandle,guiHandle,hpHandle,repairHandle,coinHandle));
		}
	}
	
	waveCount = 0;

	if (!EndOfRaid) {
		for (int i = 0; i < wave; i++) {
			if (!space[i]->isEnable()) {
				waveCount++;
				if (waveCount > 3) {
					waveCount = 3;
				}
			}
		}
	}

	if (wave == waveCount) {
		wave++;
		startWave = false;
		init();
	}

	if (clearCount == 3) {
		clearCount--;
		quakeCount++;
		quakeTimer = 120;
		quakeY = 20.0f;
		footstepSoundVolume += 10;
	}

	if (quakeCount == 3 && quakeTimer <= 0) {
		quakeCount++;
	}

	if (nextScene) {
		updateFunc = &GameMain::gameClearIntroduction;
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
		if (targetOffset.x < -field->getWidth() + Game::kScreenWidth)
		{
			targetOffset.x = static_cast<float>( -field->getWidth() + Game::kScreenWidth);
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
	if (musicVolume > 1) {
		musicVolume--;
		SetVolumeMusic(musicVolume);
	}
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new Gameover(manager_,input,1));
		StopMusic();
		return;
	}
}

void GameMain::bossBattleSceneFadeOutUpdate(const InputState& input)
{
	if (musicVolume > 1) {
		musicVolume--;
		SetVolumeMusic(musicVolume);
	}
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new BossBattleScene(manager_));
		StopMusic();
		return;
	}
}
