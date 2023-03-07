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
#include "TextScene.h"
#include <algorithm>
#include "GameMain.h"
#include "Pause.h"

TutorialScene::TutorialScene(SceneManager& manager) : SceneBase(manager), updateFunc(&TutorialScene::fadeInUpdate)
{
	bottanHandle = my::myLoadGraph("data/GUIGraph/bottan1.png");
	enemyHandle = my::myLoadGraph("data/objectGraph/enemy.png");
	hpHandle = LoadGraph("data/objectGraph/heart.png");
	portionHandle = my::myLoadGraph("data/objectGraph/portion.png");
	hacheteHandle = my::myLoadGraph("data/objectGraph/machete.png");
	guiHandle = my::myLoadGraph("data/GUIGraph/GUI.png");
	repairHandle = my::myLoadGraph("data/objectGraph/repair.png");
	coinHandle = my::myLoadGraph("data/objectGraph/CopperCoin.png");
	bottanHandle2 = my::myLoadGraph("data/GUIGraph/bottan2.png");
	bottanHandle3 = my::myLoadGraph("data/GUIGraph/bottan3.png");
	truckHandle = my::myLoadGraph("data/objectGraph/truck.png");
	

	player = new Player(1);
	player->setHandle(portionHandle, hacheteHandle, guiHandle, hpHandle, repairHandle, coinHandle);
	player->init();
	player->giftMoney();
	field = std::make_shared<Field>();
	enemy = std::make_shared<Enemy>(1);
	space = std::make_shared<BugSpace>();
	space->setPos({ 4750,280 });
	space->setPlayer(player);
	enemy->setPlayer(player, enemyHandle, coinHandle);
	space->enemySetPlayer(enemyHandle, coinHandle);
	shopperHandle = field->setHandle();
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
	DeleteGraph(coinHandle);
	DeleteGraph(bottanHandle);
	DeleteGraph(bottanHandle2);
	DeleteGraph(bottanHandle3);
	DeleteGraph(truckHandle);
}

void TutorialScene::update(const InputState& input)
{
	
	if (--time < 0) {
		imgX++;
		imgX2++;
		time = 8;
	}

	player->update(offset, input);

	if (player->getPos().x > 1000) {
		enemy->setPos(2200);
	}

	if (spownCount > 0) {
		if (player->getPos().x > 550) {
			enemy->dispatch({ 300,750 });
			spownCount--;
		}
	}

	if (enemy->isEnable()) {
		enemy->update(offset);
	}
	if (space->isEnable()) {
		space->tutorialUpdate(offset);
	}

	(this->*updateFunc)(input);

	if (updateFunc == &TutorialScene::normalUpdate) {
		{
			targetPlayer = { 0.0f,0.0f };
			targetPlayer.x = player->getPos().x + 25 - 850;
			targetPlayer.y = player->getPos().y + 44 - 780;

			targetPlayer2 = { 0.0f,0.0f };
			targetPlayer2.x = player->getPos().x + 25 - 1800;
			targetPlayer2.y = player->getPos().y + 44 - 780;

			targetPlayer3 = { 0.0f,0.0f };
			targetPlayer3.x = player->getPos().x + 25 - 4200;
			targetPlayer3.y = player->getPos().y + 44 - 780;

			targetPlayer4 = { 0.0f,0.0f };
			targetPlayer4.x = player->getPos().x + 25 - 4800;
			targetPlayer4.y = player->getPos().y + 44 - 300;

			targetPlayer5 = { 0.0f,0.0f };
			targetPlayer5.x = player->getPos().x + 25 - 5300;
			targetPlayer5.y = player->getPos().y + 44 - 780;

			targetPlayer6 = { 0.0f,0.0f };
			targetPlayer6.x = player->getPos().x + 25 - 4700;
			targetPlayer6.y = player->getPos().y + 44 - 780;

			targetPlayer7 = { 0.0f,0.0f };
			targetPlayer7.x = player->getPos().x + 25 - 3315;
			targetPlayer7.y = player->getPos().y + 44 - 780;
		}

		if (textCount == 1) {
			personNum = 1;
		}
		else if (textCount == 2) {
			personNum = 2;
		}
		else if (textCount == 3) {
			personNum = 3;
		}

		{
			switch (textCount) {
			case 0:
				if (textCount == 0) {
					if (targetPlayer.length() < 200) {
						manager_.pushScene(new TextScene(manager_, input, 0, offset, bottanHandle));
						textCount = 1;
					}
				}
				break;
			case 1:
				if (textCount == 1) {
					if (targetPlayer2.length() < 200) {
						manager_.pushScene(new TextScene(manager_, input, 1, offset, bottanHandle));
						textCount = 2;
					}
				}
				break;
			case 2:
				if (textCount == 2) {
					if (targetPlayer3.length() < 200) {
						manager_.pushScene(new TextScene(manager_, input, 2, offset, bottanHandle));
						textCount = 3;
					}
				}
				break;
			case 3:
				if (!space->isEnable()) {
					if (targetPlayer5.length() < 200) {
						manager_.pushScene(new TextScene(manager_, input, 3, offset, bottanHandle));
						textCount = 4;
					}
				}
				break;
			}
		}

		if (textCount == 4) {
			if (player->getPos().x > 5500) {
				updateFunc = &TutorialScene::clearFadeOutUpdate;
			}
		}
	}

	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new Pause(manager_, input));
	}

}

void TutorialScene::draw()
{
	field->draw(offset,0);

	if (enemy->isEnable()) {
		enemy->draw(offset);
	}
	else {
		if (targetPlayer7.length() < 300) {
			if (!buy) {
				textDraw(0);
			}
		}
	}

	if (targetPlayer4.length() < 200) {
		if (player->returnRepair() > 0) {
			textDraw(2);
			count--;
		}
	}

	if (space->isEnable()) {
		space->draw(offset);
	}
	
	if (targetPlayer6.length() < 200) {
		textDraw(1);
	}

	//ìXàı
	switch (personNum) {
	case 0:
		DrawRotaGraph(850 + offset.x, 780, 2.0f, 0.0f, shopperHandle, true);
		break;
	case 1:
		DrawRotaGraph(1800 + offset.x, 780, 2.0f, 0.0f, shopperHandle, true);
		break;
	case 2:
		DrawRotaGraph(4200 + offset.x, 780, 2.0f, 0.0f, shopperHandle, true);
		break;
	case 3:
		DrawRotaGraph(5300 + offset.x, 780, 2.0f, 0.0f, shopperHandle, true);
		break;
	}
	
	if (player->getPos().x > 5500) {
		if (textCount == 4) {
			
		}
		else {
			player->draw(offset);
		}
	}
	else {
		player->draw(offset);
	}

	if (textCount == 4) {
		DrawGraph(truckPos + offset.x, 643,truckHandle,true);
	}

	DrawFormatString(1300, 500, 0xffffff, "%d", truckPos, true);

	bool aiu = player->returnFlyingisEnabled();

	if (aiu) {
		DrawString(Game::kScreenWidth / 2 - 130, 900, "ìäÇ∞ÇΩÇ‡ÇÃÇÕÇøÇ·ÇÒÇ∆èEÇ¢Ç‹ÇµÇÂÇ§", 0xffffff);
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

	int x = (3315) / chipSize;
	int y = (780) / chipSize;

	if (player->shopCollision(x, y, offset)) {
		if (input.isTriggered(InputType::next))
		{
			manager_.pushScene(new Shop(manager_, input, player, portionHandle, guiHandle, hpHandle, repairHandle));
			buy = true;
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
	if (targetOffset.x < -field->getTutorialWidth() + Game::kScreenWidth + 16)
	{
		targetOffset.x = -field->getTutorialWidth() + Game::kScreenWidth + 16;
	}


	offset = targetOffset;
}

void TutorialScene::gameoverFadeOutUpdate(const InputState& input)
{
}

void TutorialScene::clearFadeOutUpdate(const InputState& input)
{
	
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	truckPos += 10;
	if (++fadeTimer == fadeInterval) {
		manager_.changeScene(new GameMain(manager_));
		return;
	}
}

void TutorialScene::textDraw(int num)
{
	
	switch (num) {
	case 0:
		imgY = 1;
		if (imgX > 5) {
			imgX = 2;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(3180 + offset.x, 570, 3450 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(3180 + offset.x, 570, 3450 + offset.x, 720, 0xffffff, false);
		DrawString(3205 + offset.x, 620, "Ç®ã‡ÇèEÇ¡ÇƒîÉÇ¢ï®ÇÇµÇÊÇ§", 0xffffff);

		DrawRectRotaGraph(3310 + offset.x, 670, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
		break;
	case 1:

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(4700 + offset.x, 570, 4900 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(4700 + offset.x, 570, 4900 + offset.x, 720, 0xffffff, false);

		imgX = 0;
		if (--time2 == 0) {
			imgY++;
			time2 = 8;
		}

		if (imgY > 1) {
			imgY = 0;
		}
		DrawString(4785 + offset.x, 680, "ìoÇÈ", 0xffffff);
		DrawRectRotaGraph(4800 + offset.x, 650, imgX * 18, imgY * 18, 18, 18, 2.0f, 0.0f, bottanHandle3, true, false);
		break;
	case 2:
		
		imgY = 2;
		imgY2 = 3;
		if (imgX2 > 3) {
			imgX2 = 0;
		}
		if (imgX > 5) {
			imgX = 2;
		}
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(4600 + offset.x, 70, 5000 + offset.x, 220, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(4600 + offset.x, 70, 5000 + offset.x, 220, 0xffffff, false);
		DrawString(4720 + offset.x, 95, "éËéùÇøÇêÿÇËë÷Ç¶ÇÊÇ§", 0xffffff);

		DrawRectRotaGraph(4750 + offset.x, 140, imgX2 * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle2, true, false);
		DrawString(4790 + offset.x, 150, "or", 0xffffff);
		DrawRectRotaGraph(4850 + offset.x, 140, imgX2 * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle2, true, false);
		DrawRectRotaGraph(4730 + offset.x, 190, imgX * 16, (imgY - 2) * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
		DrawString(4750 + offset.x, 180, "Ç≈ÉAÉCÉeÉÄÇégóp", 0xffffff);
		break;
	}
}