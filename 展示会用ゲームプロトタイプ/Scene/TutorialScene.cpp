#include "TutorialScene.h"
#include "../Object/Player.h"
#include "DxLib.h"
#include "../field.h"
#include "../DrawFunctions.h"

TutorialScene::TutorialScene(SceneManager& manager) : SceneBase(manager), updateFunc(&TutorialScene::fadeInUpdate)
{
	enemyHandle = my::myLoadGraph("data/enemy.png");
	hpHandle = LoadGraph("data/heart.png");
	portionHandle = my::myLoadGraph("data/portion.png");
	hacheteHandle = my::myLoadGraph("data/machete.png");
	guiHandle = my::myLoadGraph("data/GUI.png");
	repairHandle = my::myLoadGraph("data/repair.png");
	bugHandle = my::myLoadGraph("data/Bag.png");

	player = new Player;
	player->setHandle(portionHandle, hacheteHandle, guiHandle, hpHandle, repairHandle,bugHandle);
	player->init();
	field = std::make_shared<Field>();
}

TutorialScene::~TutorialScene()
{
	delete player;
	DeleteGraph(enemyHandle);
	DeleteGraph(portionHandle);
	DeleteGraph(guiHandle);
	DeleteGraph(hacheteHandle);
	DeleteGraph(hpHandle);
	DeleteGraph(repairHandle);
	DeleteGraph(bugHandle);
}

void TutorialScene::update(const InputState& input)
{
	player->update(offset, input);
	(this->*updateFunc)(input);
}

void TutorialScene::draw()
{
	field->draw(offset,0);
	player->draw(offset);
	DrawString(0, 0, "TutorialScene", 0xffffff);
}

void TutorialScene::fadeInUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (--fadeTimer == 0) {
		updateFunc = &TutorialScene::normalUpdate;
		fadeValue = 0;
	}
}

void TutorialScene::normalUpdate(const InputState& input)
{
	Vec2 targetOffset{};

	targetOffset.x = (Game::kScreenWidth / 2) - player->getPos().x;
	if (targetOffset.x > 0)
	{
		targetOffset.x = 0;
	}
	if (targetOffset.x < -field->getWidth() + Game::kScreenWidth + 16)
	{
		targetOffset.x = -field->getWidth() + Game::kScreenWidth + 16;
	}


	offset = targetOffset;
}

void TutorialScene::gameoverFadeOutUpdate(const InputState& input)
{
}

void TutorialScene::clearFadeOutUpdate(const InputState& input)
{
}
