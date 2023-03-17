#include "DemoScene.h"
#include "DxLib.h"
#include "../game.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "../game.h"

DemoScene::DemoScene(SceneManager& manager) : SceneBase(manager)
{
	demoMovieHandle = my::myLoadGraph("data/movie/project Viking mainDemo.mp4");
	PlayMovieToGraph(demoMovieHandle);

	LPCSTR UIfontPath = "data/other/Silver.ttf";
	AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL);
	UIFontHandle = CreateFontToHandle("Silver", 48, 9, -1);
}

DemoScene::~DemoScene()
{
	DeleteGraph(demoMovieHandle);
	DeleteFontToHandle(UIFontHandle);
}

void DemoScene::update(const InputState& input)
{
	if (input.isTriggered(InputType::attack) || input.isTriggered(InputType::down) || input.isTriggered(InputType::left) || input.isTriggered(InputType::next) || input.isTriggered(InputType::nextItem) ||
		input.isTriggered(InputType::pause) || input.isTriggered(InputType::prev) || input.isTriggered(InputType::prevItem) || input.isTriggered(InputType::right) || input.isTriggered(InputType::shot) ||
		input.isTriggered(InputType::up)) {
		updateFunc = &DemoScene::fadeOutUpdate;
	}
}

void DemoScene::draw()
{
	
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, demoMovieHandle, false);

	DrawStringToHandle(0, 800, "DEMO MOVIE", 0xff4444, UIFontHandle);
}

void DemoScene::normalUpdate(const InputState& input)
{
	
}

void DemoScene::fadeInUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (--fadeTimer == 0) {
		updateFunc = &DemoScene::update;
		fadeValue = 0;
	}
}

void DemoScene::fadeOutUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new TitleScene(manager_));
		StopMusic();
		return;
	}
}
