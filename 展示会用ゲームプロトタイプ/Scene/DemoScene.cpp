#include "DemoScene.h"
#include "DxLib.h"
#include "../game.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "../game.h"
#include <algorithm>

DemoScene::DemoScene(SceneManager& manager) : SceneBase(manager),updateFunc(&DemoScene::fadeInUpdate)
{
	demoMovieHandle = my::myLoadGraph("data/movie/project Viking mainDemo.mp4");
	PlayMovieToGraph(demoMovieHandle);

	LPCSTR UIfontPath = "data/other/Silver.ttf";
	AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL);
	UIFontHandle = CreateFontToHandle("Silver", 96, 9, -1);
}

DemoScene::~DemoScene()
{
	DeleteGraph(demoMovieHandle);
	DeleteFontToHandle(UIFontHandle);
}

void DemoScene::update(const InputState& input)
{
	(this->*updateFunc)(input);
}

void DemoScene::draw()
{
	
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, demoMovieHandle, false);

	if (ultimateTimer > 0) {
		if ((ultimateTimer / 10) % 10 <= 5) {
			return;
		}
	}

	DrawStringToHandle(1450, 950, "DEMO MOVIE", 0xff4444, UIFontHandle);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue);
	//‰æ–Ê‘S‘Ì‚ð^‚Á•‚É“h‚è‚Â‚Ô‚·
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DemoScene::normalUpdate(const InputState& input)
{
	if (input.isTriggered(InputType::attack) || input.isTriggered(InputType::down) || input.isTriggered(InputType::left) || input.isTriggered(InputType::next) || input.isTriggered(InputType::nextItem) ||
		input.isTriggered(InputType::pause) || input.isTriggered(InputType::prev) || input.isTriggered(InputType::prevItem) || input.isTriggered(InputType::right) || input.isTriggered(InputType::shot) ||
		input.isTriggered(InputType::up)) {
		updateFunc = &DemoScene::fadeOutUpdate;
	}

	if (GetMovieStateToGraph(demoMovieHandle) == 0) {
		updateFunc = &DemoScene::fadeOutUpdate;
	}

	ultimateTimer = (std::max)(ultimateTimer - 1, 0);

	if (ultimateTimer == 0) {
		ultimateTimer = 120;
	}
}

void DemoScene::fadeInUpdate(const InputState& input)
{
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (--fadeTimer == 0) {
		updateFunc = &DemoScene::normalUpdate;
		fadeValue = 0;
	}
}

void DemoScene::fadeOutUpdate(const InputState& input)
{
	fadeValue = static_cast < int>(255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval)));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new TitleScene(manager_));
		StopMusic();
		return;
	}
}
