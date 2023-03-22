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
#include "Pause.h"
#include "DxLib.h"
#include <cassert>

namespace {
	constexpr int particleNum = 256;
}

BossBattleScene::BossBattleScene(SceneManager& manager) : SceneBase(manager),updateFunc(&BossBattleScene::fadeInUpdate)
{

	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	hpHandle = my::myLoadGraph("data/objectGraph/heart.png");
	portionHandle = my::myLoadGraph("data/objectGraph/portion.png");
	macheteHandle = my::myLoadGraph("data/objectGraph/machete.png");
	guiHandle = my::myLoadGraph("data/GUIGraph/GUI.png");
	repairHandle = my::myLoadGraph("data/objectGraph/repair.png");
	coinHandle = my::myLoadGraph("data/objectGraph/CopperCoin.png");
	signboardHandle = my::myLoadGraph("data/objectGraph/kb.png");


	cheerSound = LoadSoundMem("data/soundEffect/cheer.mp3");
	footstepSound = LoadSoundMem("data/soundEffect/small_explosion1.mp3");


	player = new Player(2);
	player->setHandle(portionHandle, macheteHandle, guiHandle, hpHandle, repairHandle, coinHandle);
	player->init();
	player->setPos({Game::kScreenWidth / 2,760});

	field = std::make_shared<Field>();
	bossEnemy = std::make_shared <Enemy>(3);
	
	bossEnemy->setPlayer(player, enemyHandle, coinHandle);
	bossEnemy->dispatch({ -700,400 });

	int sw, sh, bit;
	GetScreenState(&sw, &sh, &bit);
	tempScreenH = MakeScreen(sw, sh);
	assert(tempScreenH >= 0);

	effectScreen = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	pos[particleNum];
	vec[particleNum];

	for (int i = 0; i < particleNum; i++)
	{
		pos[i].x = static_cast<float>(GetRand(Game::kScreenWidth));
		pos[i].y = static_cast<float>(- GetRand(Game::kScreenHeight));

		vec[i].x = 0.0f;
		vec[i].y = static_cast<float>(GetRand(20) + 20);
		vec[i].y /= 10.0f;
	}
}

BossBattleScene::~BossBattleScene()
{
	delete player;
	DeleteGraph(enemyHandle);
	DeleteGraph(hpHandle);
	DeleteGraph(portionHandle);
	DeleteGraph(guiHandle);
	DeleteGraph(macheteHandle);
	DeleteGraph(repairHandle);
	DeleteGraph(coinHandle);
	DeleteGraph(tempScreenH);
	DeleteGraph(signboardHandle);
	DeleteGraph(tempScreenH);
	DeleteGraph(effectScreen);

	DeleteSoundMem(cheerSound);
	DeleteSoundMem(footstepSound);
}

void BossBattleScene::update(const InputState& input)
{
	(this->*updateFunc)(input);

	if (--quakeTimer > 0) {
		quakeY = -quakeY;
		quakeY *= 0.95f;
		--quakeTimer;
	}
	else {
		quakeY = 0.0f;
	}
}

void BossBattleScene::draw()
{

	//加工用スクリーンハンドルをセット
	SetDrawScreen(tempScreenH);
	ClearDrawScreen();

	field->draw(offset,2);

	for (int x = 0; x < bossNumX; x++) {

		int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

		if (posX > Game::kScreenWidth + chipSize) {
			break;
		}
		if (posX < chipSize * 14) {
			continue;
		}

		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = groundData::bossGround[y][x];

			if (chipNo == 1) {
				DrawRotaGraph(static_cast<int>(x * chipSize + offset.x), (y - 1) * chipSize + 22,1.5f,0.0f, signboardHandle, true);
			}
		}
	}

	if (!player->updateResuscitation(offset)) {

	}
	else {
		bossMove = true;
		player->draw(offset);
	}

	if (bossEnemy->isEnable()) {
		bossEnemy->BossDraw(offset);
	}

	if (--ImgTime == 0) {
		imgX++;
		ImgTime = 4;
		if (imgX > 3) {
			imgX = 0;
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, static_cast<int>(quakeY), tempScreenH, false);

	if (!bossEnemy->isEnable()) {
		particleDraw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BossBattleScene::bossAppearanceUpdate(const InputState& input)
{
	
	if (bossMove) {
		if (bossEnemy->getPos().x < -1) {
			if (bossEnemy->isEnable()) {
				bossEnemy->update(offset);
				if (--soundCount == 0) {
					quakeTimer = 100;
					quakeY = 20.0f;
					ChangeVolumeSoundMem(130, footstepSound);
					PlaySoundMem(footstepSound, DX_PLAYTYPE_BACK, true);
					soundCount = 100;
				}
			}
		}
		else {
			if (--startTime == 0) {
				PlayMusic("data/music/Battle-ricercare.mp3",DX_PLAYTYPE_LOOP);
				updateFunc = &BossBattleScene::normalUpdate;
			}
		}
	}
}


void BossBattleScene::normalUpdate(const InputState& input)
{

	if (musicVolume < 120) {
		musicVolume++;
		SetVolumeMusic(musicVolume);
	}

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
		targetOffset.x = static_cast<float>( -field->getBossWidth() + Game::kScreenWidth);
	}

	offset = targetOffset * 0.5f + offset * 0.5f;

	if (!bossEnemy->isEnable()) {
		updateFunc = &BossBattleScene::clearUpdate;
	}

	if (bossEnemy->returHp()) {
		if (--soundCount == 0) {
			quakeTimer = 100;
			quakeY = 10.0f;
			ChangeVolumeSoundMem(130, footstepSound);
			PlaySoundMem(footstepSound, DX_PLAYTYPE_BACK, true);
			soundCount = 100;
		}
	}

	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new Pause(manager_, input));
	}

}

void BossBattleScene::particleUpdate()
{
	for (int i = 0; i < particleNum; i++)
	{
		pos[i] += vec[i];
		if (pos[i].y > Game::kScreenHeight)
		{
			pos[i].x = static_cast<float>(GetRand(Game::kScreenWidth));
			pos[i].y = 0.0f;

			vec[i].x = 0.0f;
			vec[i].y = static_cast<float>(GetRand(20) + 20);
			vec[i].y /= 10.0f;
		}
	}
	sinRate += 0.05f;
}

void BossBattleScene::particleDraw()
{
	// draw
	SetDrawScreen(effectScreen);
	ClearDrawScreen();
	BeginAADraw();
	for (int i = 0; i < particleNum; i++)
	{
		float rate = (vec[i].length() / 4.0f);
		//	DrawCircleAA(pos[i].x, pos[i].y, 4 * rate, 32, 0x808020, true);
			// 遠くのもの(落下速度が遅いもの)は小さく、暗く描画する
		DrawCircleAA(pos[i].x, pos[i].y, 4 * rate, 32, GetColor(static_cast < int>(0 * rate), static_cast < int>(79 * rate), static_cast < int>(128 * rate)), true);
	}
	EndAADraw();
	SetDrawScreen(DX_SCREEN_BACK);
	
	//加算合成する
	SetDrawBlendMode(DX_BLENDMODE_ADD, 192);

	DrawGraph(0, 0, effectScreen, true);

	//画面をぼかす
	GraphFilter(effectScreen, DX_GRAPH_FILTER_GAUSS, 16, 800);
	for (int i = 0; i < 8; i++)
	{
		DrawGraph(GetRand(4) - 2, GetRand(4) - 2, effectScreen, false);
	}

	GraphFilter(effectScreen, DX_GRAPH_FILTER_GAUSS, 32, 2400);
	for (int i = 0; i < 8; i++)
	{
		DrawGraph(GetRand(8) - 4, GetRand(8) - 4, effectScreen, false);
	}
	//元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
}

void BossBattleScene::fadeInUpdate(const InputState& input)
{
	fadeValue = static_cast <int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (--fadeTimer == 0) {
		updateFunc = &BossBattleScene::bossAppearanceUpdate;
		fadeValue = 0;
	}
}

void BossBattleScene::gameoverFadeOutUpdate(const InputState& input)
{
	if (musicVolume > 1) {
		musicVolume--;
		SetVolumeMusic(musicVolume);
	}
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new Gameover(manager_,input,2));
		StopMusic();
		return;
	}
}

void BossBattleScene::clearFadeOutUpdate(const InputState& input)
{
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new GameClear(manager_));
		StopMusic();
		return;
	}
}

void BossBattleScene::clearUpdate(const InputState& input)
{
	if (musicVolume > 1) {
		musicVolume--;
		SetVolumeMusic(musicVolume);
	}

	if (--cheerTime == 0) {
		PlaySoundMem(cheerSound, DX_PLAYTYPE_BACK);
		cheerTime = 160;
	}
	
	if (--cheerCount == 0) {
		updateFunc = &BossBattleScene::clearFadeOutUpdate;
	}

	if (!bossEnemy->isEnable()) {
		particleUpdate();
	}
}

