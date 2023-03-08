#include "GameClear.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "../DrawFunctions.h"

GameClear::GameClear(SceneManager& manager) : SceneBase(manager),updateFunc(&GameClear::fadeInUpdate)
{
	clearHandle = my::myLoadGraph("data/fieldGraph/endGraph.png");
}

GameClear::~GameClear()
{
	DeleteGraph(clearHandle);
	
}

void GameClear::update(const InputState& input)
{
	(this->*updateFunc)(input);
}

void GameClear::draw()
{
	DrawString(0, 0, "Gameclear", 0xffffff);
	DrawString(0, 15, "press to enter", 0xffffff);

	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0f, 0.0f, clearHandle, true, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameClear::fadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0) {
		updateFunc = &GameClear::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameClear::normalUpdate(const InputState& input)
{
	if (input.isTriggered(InputType::next))
	{
		updateFunc = &GameClear::fadeOutUpdate;
	}
}

void GameClear::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new TitleScene(manager_));
		return;
	}
}
