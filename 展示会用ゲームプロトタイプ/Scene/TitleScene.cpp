#include "TitleScene.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "TutorialScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"

TitleScene::TitleScene(SceneManager& manager):SceneBase(manager),updateFunc_(&TitleScene::fadeInUpdate)
{
	sceneTable[choice::main] = "本番";
	sceneTable[choice::tutorial] = "チュートリアル";
	sceneTable[choice::option] = "オプション";
	sceneTable[choice::end] = "終了";
}

void TitleScene::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::draw()
{
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

		//選択された時の処理
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			DrawString(x - 20, y, "⇒", 0xff0000);
		}

		//各キーの表示
		if (currentInputIndex == idx) {
			x += offset;
		}
		DrawString(x, y, name.second.c_str(), color);

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
	}
}

void TitleScene::normalUpdate(const InputState& input)
{
	choiceScene(input);
}

void TitleScene::fadeOutUpdateGameMain(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new GameMain(manager_));
		return;
	}
}

void TitleScene::fadeOutUpdateTutorial(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new TutorialScene(manager_));
		return;
	}
}
