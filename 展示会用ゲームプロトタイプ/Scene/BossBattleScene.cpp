#include "BossBattleScene.h"
#include "SceneManager.h"
#include "GameMain.h"
#include "GameClear.h"
#include "Gameover.h"
#include "../Object/Player.h"
#include "../InputState.h"
#include "../game.h"
#include "../field.h"
#include "DxLib.h"

BossBattleScene::BossBattleScene(SceneManager& manager) : SceneBase(manager),updateFunc(&BossBattleScene::fadeInUpdate)
{
	player = new Player;
	field = std::make_shared<Field>();
}

BossBattleScene::~BossBattleScene()
{
}

void BossBattleScene::init()
{
	player->init();
}

void BossBattleScene::update(const InputState& input)
{
	player->update(offset,input);
	(this->*updateFunc)(input);
}

void BossBattleScene::draw()
{
	player->draw(offset);
	DrawString(0, 0, "bossBattleScene", 0xffffff);
}


void BossBattleScene::normalUpdate(const InputState& input)
{

	Vec2 targetOffset{};

	targetOffset.x = (Game::kScreenWidth / 2) - player->getPos().x;
	if (targetOffset.x > 0)
	{
		targetOffset.x = 0;
	}
	if (targetOffset.x < -field->getWidth() + Game::kScreenWidth)
	{
		targetOffset.x = -field->getWidth() + Game::kScreenWidth;
	}


	offset = targetOffset * 0.5f + offset * 0.5f;

	if (input.isTriggered(InputType::next)) {
		updateFunc = &BossBattleScene::clearFadeOutUpdate;
	}
}

void BossBattleScene::fadeInUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (--fadeTimer == 0) {
		updateFunc = &BossBattleScene::normalUpdate;
		fadeValue = 0;
	}
}

void BossBattleScene::gameoverFadeOutUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new Gameover(manager_,input));
		return;
	}
}

void BossBattleScene::clearFadeOutUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new GameClear(manager_));
		return;
	}
}

