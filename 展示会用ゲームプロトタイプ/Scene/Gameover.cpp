#include "Gameover.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "GameMain.h"

Gameover::Gameover(SceneManager& manager, const InputState& input):SceneBase(manager),updateFunc_(&Gameover::fadeInUpdate), inputState(input)
{
	gameOverChoiceTable[GameOverChoice::continueMain] = "continue";
	gameOverChoiceTable[GameOverChoice::end] = "end";
}

void Gameover::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Gameover::draw()
{
	DrawString(0, 0, "Gameover", 0xffffff);
	DrawString(0, 15, "press to enter", 0xffffff);


	constexpr int pw_width = Game::kScreenWidth / 2 - 200;						//ポーズ枠の幅
	constexpr int pw_height = Game::kScreenHeight / 2 - 200;						//ポーズ枠の高さ
	constexpr int pw_start_x = Game::kScreenWidth / 2 + 200;	//ポーズ枠に左
	constexpr int pw_start_y = Game::kScreenHeight / 2 + 200;	//ポーズ枠上

	auto y = Game::kScreenHeight / 2 + 100;
	int x = Game::kScreenWidth / 2;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : gameOverChoiceTable) {
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


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Gameover::fadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &Gameover::normalUpdate;
		fadeValue_ = 0;
	}
}

void Gameover::normalUpdate(const InputState& input)
{
	const int nameCount = gameOverChoiceTable.size();

	if (input.isTriggered(InputType::up)) {
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}


	if (currentInputIndex == 0) {
		if (input.isTriggered(InputType::next))
		{
			updateFunc_ = &Gameover::continueFadeOutUpdate;
		}
	}
	if (currentInputIndex == 1) {
		if (input.isTriggered(InputType::next))
		{
			updateFunc_ = &Gameover::fadeOutUpdate;
		}
	}
}

void Gameover::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new TitleScene(manager_));
		return;
	}
}

void Gameover::continueFadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new GameMain(manager_));
		return;
	}
}
