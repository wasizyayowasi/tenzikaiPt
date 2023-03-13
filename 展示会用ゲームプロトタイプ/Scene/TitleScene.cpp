#include "TitleScene.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "TutorialScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "../Object/Enemy.h"
#include "../DrawFunctions.h"
#include "../Scene/BossBattleScene.h"
#include "../field.h"

TitleScene::TitleScene(SceneManager& manager):SceneBase(manager),updateFunc_(&TitleScene::fadeInUpdate)
{
	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	soundHandle = LoadSoundMem("data/soundEffect/Wind-Synthetic_Ambi02-1.mp3");
	bottanHandle = my::myLoadGraph("data/GUIGraph/bottan1.png");
	uiSound = LoadSoundMem("data/soundEffect/ui3.mp3");
	uiSound2 = LoadSoundMem("data/soundEffect/ui4.mp3");

	LPCSTR fontPath = "data/other/CompassPro.ttf";
	LPCSTR UIfontPath = "data/other/Silver.ttf";

	AddFontResourceEx(fontPath, FR_PRIVATE, NULL);
	AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL);
	
	fontHandle = CreateFontToHandle("CompassPro", 64, -1, -1);
	fontHandle2 = CreateFontToHandle("CompassPro", 200, -1, -1);
	UIfontHandle = CreateFontToHandle("Silver", 48, 9, -1);
	UIfontHandle2 = CreateFontToHandle("Silver", 64, 9, -1);

	titleWidth = GetDrawStringWidthToHandle("P  R  O  J  E  C  T", strlen("P  R  O  J  E  C  T"), fontHandle);
	titleWidth2 = GetDrawStringWidthToHandle("VIKING", strlen("VIKING"), fontHandle2);

	sceneTable[choice::tutorial] = "チュートリアル";
	sceneTable[choice::main] = "メイン";
	sceneTable[choice::boss] = "ボス";
	sceneTable[choice::end] = "終了";

	field = std::make_shared<Field>();

	enemy = std::make_shared<Enemy>(4);
	enemy->setHandle(enemyHandle);
	enemy->dispatch({ Game::kScreenWidth + 120 , 800 });

	for (auto& enemy : enemies) {
		enemy = std::make_shared<Enemy>(4);
	}
}

TitleScene::~TitleScene()
{
	DeleteGraph(enemyHandle);
	DeleteGraph(bottanHandle);
	DeleteSoundMem(soundHandle);
	DeleteSoundMem(uiSound);
	DeleteSoundMem(uiSound2);
	DeleteFontToHandle(fontHandle);
	DeleteFontToHandle(fontHandle2);
	DeleteFontToHandle(UIfontHandle);
	DeleteFontToHandle(UIfontHandle2);
}

void TitleScene::update(const InputState& input)
{

	if (!enemy->isEnable()) {
		enemyGo = true;
	}
	else {
		enemyGo = false;
	}

	for (auto& enemy : enemies) {
		if (enemy->isEnable()) {
			enemiesGo = false;
		}
		else {
			enemiesGo = true;
		}
	}

	if (enemyGo == enemiesGo) {
		luck = GetRand(100);
		luck = (luck / 10) % 10;
		if (luck > 0 && luck < 10) {
			if (--collTime == 0) {
				enemy->setHandle(enemyHandle);
				enemy->dispatch({ Game::kScreenWidth + 120 , 800 });
				collTime = 300;
			}
		}
		else if (luck == 10) {
			if (--collTime == 0) {
				for (auto& enemy : enemies) {
					enemy->setHandle(enemyHandle);
					enemy->dispatch({ Game::kScreenWidth + shift.x , shift.y });
					shift += {50.0f, 0.0f};
				}
				collTime = 300;
			}
		}
	}

	for (auto& enemy : enemies) {
		if (enemy->isEnable()) {
			enemy->update({ 0.0f,0.0f });
		}
	}

	if (enemy->isEnable()) {
		enemy->update({ 0.0f,0.0f });
	}
	(this->*updateFunc_)(input);
}

void TitleScene::draw()
{
	field->draw({ 0.0f,0.0f },3);

	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth / 2 + 5, Game::kScreenHeight / 5 + 5, "P  R  O  J  E  C  T", 0x888833, fontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth2 / 2 + 5, Game::kScreenHeight / 5 + 37, "VIKING", 0x888833, fontHandle2);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth / 2, Game::kScreenHeight / 5, "P  R  O  J  E  C  T", 0xffffff, fontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth2 / 2, Game::kScreenHeight / 5 + 32, "VIKING", 0xffffff, fontHandle2);

	if (enemy->isEnable()) {
		enemy->draw({ 0.0f,0.0f });
	}

	for (auto& enemy : enemies) {
		if (enemy->isEnable()) {
			enemy->draw({ 0.0f,0.0f });
		}
	}

	choiceSceneDraw();

	if (--time < 0) {
		imgX++;
		time = 8;
	}

	if (imgX > 5) {
		imgX = 2;
	}

	DrawRectRotaGraph(Game::kScreenWidth / 2 - 32, 950, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
	DrawStringToHandle(Game::kScreenWidth / 2, 940, "決定", 0xff0000, UIfontHandle);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	
}

void TitleScene::choiceScene(const InputState& input)
{
	const int nameCount = sceneTable.size();

	if (input.isTriggered(InputType::up)) {
		ChangeVolumeSoundMem(160, uiSound);
		PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		ChangeVolumeSoundMem(160, uiSound);
		PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}

	if (input.isTriggered(InputType::next)) {
		ChangeVolumeSoundMem(180, uiSound2);
		PlaySoundMem(uiSound2, DX_PLAYTYPE_BACK);
		if (currentInputIndex == 0) {
			updateFunc_ = &TitleScene::fadeOutUpdateTutorial;
			return;
		}
		if (currentInputIndex == 1) {
			updateFunc_ = &TitleScene::fadeOutUpdateGameMain;
			return;
		}
		if (currentInputIndex == 2) {
			updateFunc_ = &TitleScene::fadeOutUpdateBoss;
			return;
		}
		if (currentInputIndex == 3) {
			DxLib_End();
			return;
		}
	}

}

void TitleScene::choiceSceneDraw()
{
	constexpr int pw_width = Game::kScreenWidth / 2 - 200;						//ポーズ枠の幅
	constexpr int pw_height = Game::kScreenHeight / 2 - 200;						//ポーズ枠の高さ
	constexpr int pw_start_x = Game::kScreenWidth / 2 + 200;	//ポーズ枠に左
	constexpr int pw_start_y = Game::kScreenHeight / 2 + 200;	//ポーズ枠上

	auto y = Game::kScreenHeight / 2 + 150;
	int x = Game::kScreenWidth / 2;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : sceneTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		int font = strlen(name.second.c_str());
		fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font,UIfontHandle);

		//選択された時の処理
		if (currentInputIndex == idx) {
			fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, UIfontHandle2);
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			if (currentInputIndex == 3) {
				color = 0x44ff44;
			}
			DrawStringToHandle(x - fontSize / 2, y, name.second.c_str(), color, UIfontHandle2);
		}
		else {
			DrawStringToHandle(x - fontSize / 2, y, name.second.c_str(), color, UIfontHandle);
		}

		y += 50;
		if (idx == 2) {
			y += 20;
		}
		++idx;
	}

}

void TitleScene::fadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &TitleScene::normalUpdate;
		fadeValue_ = 0;
		ChangeVolumeSoundMem(soundVolume, soundHandle);
		PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP, true);
	}
}

void TitleScene::normalUpdate(const InputState& input)
{
	choiceScene(input);
}

void TitleScene::fadeOutUpdateGameMain(const InputState& input)
{
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, soundHandle);
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new GameMain(manager_));
		return;
	}
}

void TitleScene::fadeOutUpdateTutorial(const InputState& input)
{
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, soundHandle);
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new TutorialScene(manager_));
		return;
	}
}

void TitleScene::fadeOutUpdateBoss(const InputState& input)
{
	soundVolume--;
	ChangeVolumeSoundMem(soundVolume, soundHandle);
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new BossBattleScene(manager_));
		return;
	}
}
