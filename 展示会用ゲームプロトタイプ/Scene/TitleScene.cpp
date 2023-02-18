#include "TitleScene.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"

TitleScene::TitleScene(SceneManager& manager):SceneBase(manager),updateFunc_(&TitleScene::fadeInUpdate)
{
}

void TitleScene::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::draw()
{
	DrawString(0, 0, "TitleScene", 0xffffff);
	DrawString(0, 15, "press to enter", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	// 「次へ」ボタンが押された次シーンへ移行する
	if (input.isTriggered(InputType::next))
	{
		updateFunc_ = &TitleScene::fadeOutUpdate;
	}
	/*if (input.IsTriggered(InputType::prev))
	{
		manager_.changeScene(new OptionScene(manager_));
		return;
	}*/
}

void TitleScene::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new GameMain(manager_));
		return;
	}
}
