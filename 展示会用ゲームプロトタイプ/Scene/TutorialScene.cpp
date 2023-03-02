#include "TutorialScene.h"
#include "../Object/Player.h"
#include "DxLib.h"
#include "../field.h"
#include "../DrawFunctions.h"
#include"../InputState.h"
#include "SceneManager.h"
#include "Shop.h"
#include "../Object/Enemy.h"
#include "../Object/BugSpace.h"
#include <algorithm>

TutorialScene::TutorialScene(SceneManager& manager) : SceneBase(manager), updateFunc(&TutorialScene::fadeInUpdate)
{
	enemyHandle = my::myLoadGraph("data/enemy.png");
	hpHandle = LoadGraph("data/heart.png");
	portionHandle = my::myLoadGraph("data/portion.png");
	hacheteHandle = my::myLoadGraph("data/machete.png");
	guiHandle = my::myLoadGraph("data/GUI.png");
	repairHandle = my::myLoadGraph("data/repair.png");
	bugHandle = my::myLoadGraph("data/Bag.png");
	bottanHandle = my::myLoadGraph("data/bottan1.png");
	bottanHandle2 = my::myLoadGraph("data/bottan2.png");
	bottanHandle3 = my::myLoadGraph("data/bottan3.png");
	coinHandle = my::myLoadGraph("data/CopperCoin.png");

	player = new Player(1);
	player->setHandle(portionHandle, hacheteHandle, guiHandle, hpHandle, repairHandle,bugHandle);
	player->init();
	player->giftMoney();
	field = std::make_shared<Field>();
	enemy = std::make_shared<Enemy>(1);
	space = std::make_shared<BugSpace>(1);
	space->setPos({ 1750,280 });
	space->setPlayer(player);
	enemy->dispatch({0,700});
	enemy->setPlayer(player, enemyHandle, coinHandle);
	space->enemySetPlayer(enemyHandle, coinHandle);
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
	DeleteGraph(bottanHandle);
	DeleteGraph(bottanHandle2);
	DeleteGraph(bottanHandle3);
	DeleteGraph(coinHandle);
}

void TutorialScene::update(const InputState& input)
{
	player->update(offset, input);
	if (enemy->isEnable()) {
		enemy->update(offset);
	}
	if (space->isEnable()) {
		space->tutorialUpdate(offset);
	}

	(this->*updateFunc)(input);
}

void TutorialScene::draw()
{
	field->draw(offset,0);

	if (enemy->isEnable()) {
		enemy->draw(offset);
	}
	else {
		textDraw(3);
	}

	if (space->isEnable()) {
		space->draw(offset);
	}
	
	if (player->returnRepair() > 0) {
		textDraw(4);
	}

	player->draw(offset);

	DrawString(0, 0, "TutorialScene", 0xffffff);
	DrawCircle(650 + offset.x, 780 + offset.y, 200, 0xff0000, false);
	DrawCircle(2600 + offset.x, 780 + offset.y, 200, 0xff0000, false);
	DrawCircle(1800 + offset.x, 780 + offset.y, 550, 0xff0000, false);

	targetPlayer = { 0.0f,0.0f };
	targetPlayer.x = player->getPos().x + 25 - 650;
	targetPlayer.y = player->getPos().y + 44 - 780;

	targetPlayer2 = { 0.0f,0.0f };
	targetPlayer2.x = player->getPos().x + 25 - 1800;
	targetPlayer2.y = player->getPos().y + 44 - 780;

	targetPlayer3 = { 0.0f,0.0f };
	targetPlayer3.x = player->getPos().x + 25 - 2600;
	targetPlayer3.y = player->getPos().y + 44 - 780;

	if (targetPlayer.length() < 200) {
		textDraw(0);
	}
	if (targetPlayer2.length() < 550) {
		textDraw(1);
	}
	if (targetPlayer3.length() < 200) {
		textDraw(2);
	}


	bool aiu = player->returnFlyingisEnabled();

	if (aiu) {
		DrawString(100, 700, "ƒ|ƒXƒg‚É‰B‚ê‚ë", 0xffffff);
	}
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
	
	if (--time < 0) {
		imgX++;
		time = 8;
	}

	int x = (3315) / chipSize;
	int y = (780) / chipSize;

	if (player->shopCollision(x, y, offset)) {
		if (input.isTriggered(InputType::next))
		{
			manager_.pushScene(new Shop(manager_, input, player, portionHandle, guiHandle, hpHandle, repairHandle));
		}
	}

	

	if (player->returnFlyingisEnabled()); {
		textDraw(5);
	}

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

void TutorialScene::textDraw(int num)
{
	
	switch (num) {
	case 0:
		imgY = 1;
		if (imgX > 5) {
			imgX = 2;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(550 + offset.x, 650, 750 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(550 + offset.x, 650, 750 + offset.x, 720, 0xffffff, false);

		DrawString(590 + offset.x, 700, "ƒ|ƒXƒg‚É‰B‚ê‚ë", 0xffffff);

		DrawRectRotaGraph(650 + offset.x, 680, imgX * 16, imgY * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
		break;
	case 1:
		imgX = 0;
		if (--time2 == 0) {
			imgY++;
			time2 = 8;
		}
		
		if (imgY > 1) {
			imgY = 0;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(1750 + offset.x, 650, 1850 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(1750 + offset.x, 650, 1850 + offset.x, 720, 0xffffff, false);

		DrawString(1780 + offset.x, 700, "“o‚é", 0xffffff);

		DrawRectRotaGraph(1800 + offset.x, 680, imgX * 18, imgY * 18, 18, 18, 2.0f, 0.0f, bottanHandle3, true, false);
		break;
	case 2:
		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(2470 + offset.x, 550, 2720 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(2470 + offset.x, 550, 2720 + offset.x, 720, 0xffffff, false);

		DrawRectRotaGraph(2530 + offset.x, 640, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
		DrawString(2490 + offset.x, 680, "•€‚ğ“Š‚°‚é", 0xffffff);
		DrawRectRotaGraph(2660 + offset.x, 640, imgX * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
		DrawString(2620 + offset.x, 680, "a‚è‚Â‚¯‚é", 0xffffff);
		DrawString(2550 + offset.x, 570, "“G‚ğ“|‚»‚¤", 0xffffff);
		DrawString(2585 + offset.x, 650, "or", 0xffffff);
		break;
	case 3:
		imgY = 1;
		if (imgX > 5) {
			imgX = 2;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(3180 + offset.x, 570, 3450 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(3180 + offset.x, 570, 3450 + offset.x, 720, 0xffffff, false);
		DrawString(3205 + offset.x, 620, "‚¨‹à‚ğE‚Á‚Ä”ƒ‚¢•¨‚ğ‚µ‚æ‚¤", 0xffffff);

		DrawRectRotaGraph(3310 + offset.x, 670, imgX * 16, imgY * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
		break;
	case 4:
		imgY = 2;
		imgY2 = 3;
		if (imgX > 4) {
			imgX = 0;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(1620 + offset.x, 70, 1950 + offset.x, 220, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(1620 + offset.x, 70, 1950 + offset.x, 220, 0xffffff, false);
		DrawString(1705 + offset.x, 95, "è‚¿‚ğØ‚è‘Ö‚¦‚æ‚¤", 0xffffff);

		DrawRectRotaGraph(1745 + offset.x, 140, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle2, true, false);
		DrawString(1785 + offset.x, 150, "or", 0xffffff);
		DrawRectRotaGraph(1845 + offset.x, 140, imgX * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle2, true, false);
		DrawRectRotaGraph(1740 + offset.x, 190, imgX * 16, (imgY - 2) * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
		DrawString(1755 + offset.x, 180, "‚ÅƒAƒCƒeƒ€‚ğg—p", 0xffffff);
		break;
	case 5:
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(50, 50, 350, 250, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(50, 50, 350, 250, 0xffffff, false);
		DrawString(50, 95, "è‚¿‚ğØ‚è‘Ö‚¦‚æ‚¤", 0xffffff);
		break;
	}
	
	
	

}