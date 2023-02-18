#include "Gameover.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"

Gameover::Gameover(SceneManager& manager):SceneBase(manager),updateFunc_(&Gameover::fadeInUpdate)
{
}

void Gameover::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void Gameover::draw()
{
	DrawString(0, 0, "Gameover", 0xffffff);
	DrawString(0, 15, "press to enter", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//âÊñ ëSëÃÇê^Ç¡çïÇ…ìhÇËÇ¬Ç‘Ç∑
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
	if (input.isTriggered(InputType::next))
	{
		updateFunc_ = &Gameover::fadeOutUpdate;
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
