#include "TitleScene.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "TutorialScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "../Object/Enemy.h"
#include "../DrawFunctions.h"
#include "../field.h"

TitleScene::TitleScene(SceneManager& manager):SceneBase(manager),updateFunc_(&TitleScene::fadeInUpdate)
{
	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	soundHandle = LoadSoundMem("data/soundEffect/Wind-Synthetic_Ambi02-1.mp3");

	sceneTable[choice::main] = "本番";
	sceneTable[choice::tutorial] = "チュートリアル";
	sceneTable[choice::option] = "オプション";
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
	DeleteSoundMem(soundHandle);
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
	field->draw({ 0.0f,0.0f },0);

	if (enemy->isEnable()) {
		enemy->draw({ 0.0f,0.0f });
	}

	for (auto& enemy : enemies) {
		if (enemy->isEnable()) {
			enemy->draw({ 0.0f,0.0f });
		}
	}

	DrawString(0, 0, "TitleScene", 0xffffff);
	DrawString(0, 15, "press to enter", 0xffffff);

	choiceSceneDraw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	
}

void TitleScene::choiceScene(const InputState& input)
{
	const int nameCount = sceneTable.size();

	if (input.isTriggered(InputType::up)) {
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}

	if (input.isTriggered(InputType::next)) {
		if (currentInputIndex == 0) {
			updateFunc_ = &TitleScene::fadeOutUpdateGameMain;
			return;
		}
		if (currentInputIndex == 1) {
			updateFunc_ = &TitleScene::fadeOutUpdateTutorial;
			return;
		}
		if (currentInputIndex == 2) {

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

	auto y = Game::kScreenHeight / 2 + 100;
	int x = Game::kScreenWidth / 2;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : sceneTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		int font = strlen(name.second.c_str());
		fontSize = GetDrawStringWidth(name.second.c_str(), font);

		//選択された時の処理
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			DrawString(x - fontSize / 2 - 20, y, "⇒", 0xff0000);
		}

		//各キーの表示
		if (currentInputIndex == idx) {
			x += offset;
		}
		DrawString(x - fontSize / 2, y, name.second.c_str(), color);
		DrawFormatString(x + 200, y, 0xffffff, "%d", fontSize);

		if (currentInputIndex == idx) {
			x -= offset;
		}

		y += 50;
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
