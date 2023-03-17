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
	arrowHandle = my::myLoadGraph("data/GUIGraph/arrow.png");
	descriptionHandle = my::myLoadGraph("data/fieldGraph/1.png");
	descriptionHandle2 = my::myLoadGraph("data/fieldGraph/2.png");
	descriptionHandle3 = my::myLoadGraph("data/fieldGraph/3.png");
	descriptionHandle4 = my::myLoadGraph("data/fieldGraph/4.png");

	GetGraphSize(descriptionHandle4, &graphWidth, &graphHeight);

	LPCSTR fontPath = "data/other/Silver.ttf";
	AddFontResourceEx(fontPath, FR_PRIVATE, NULL);

	fontHandle = CreateFontToHandle("Silver", 48, 9, -1);

	player = new Player(1);
	player->setHandle(portionHandle, hacheteHandle, guiHandle, hpHandle, repairHandle, coinHandle);
	player->init();

	field = std::make_shared<Field>();

	enemy = std::make_shared<Enemy>(1);

	space = std::make_shared<BugSpace>();
	int x = 4750 / chipSize;
	int y = 280 / chipSize;
	space->init(x,y);
	space->setPos({ 3020,280 });
	space->setPlayer(player);
	enemy->setPlayer(player, enemyHandle, coinHandle);
	shopperHandle = field->setHandle();

	{
		targetPlayer = { 0.0f,0.0f };
		targetPlayer.x = player->getPos().x + 25 - 850;
		targetPlayer.y = player->getPos().y + 44 - 780;

		targetPlayer2 = { 0.0f,0.0f };
		targetPlayer2.x = player->getPos().x + 25 - 2100;
		targetPlayer2.y = player->getPos().y + 44 - 780;

		targetPlayer3 = { 0.0f,0.0f };
		targetPlayer3.x = player->getPos().x + 25 - 2500;
		targetPlayer3.y = player->getPos().y + 44 - 780;

		targetPlayer4 = { 0.0f,0.0f };
		targetPlayer4.x = player->getPos().x + 25 - 3000;
		targetPlayer4.y = player->getPos().y + 44 - 300;

		targetPlayer5 = { 0.0f,0.0f };
		targetPlayer5.x = player->getPos().x + 25 - 3500;
		targetPlayer5.y = player->getPos().y + 44 - 780;

		targetPlayer6 = { 0.0f,0.0f };
		targetPlayer6.x = player->getPos().x + 25 - 3000;
		targetPlayer6.y = player->getPos().y + 44 - 780;

		targetPlayer7 = { 0.0f,0.0f };
		targetPlayer7.x = player->getPos().x + 25 - 3635;
		targetPlayer7.y = player->getPos().y + 44 - 780;
	}
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
	DeleteGraph(arrowHandle);
	DeleteGraph(descriptionHandle);
	DeleteGraph(descriptionHandle2);
	DeleteGraph(descriptionHandle3);
	DeleteGraph(descriptionHandle4);
	DeleteFontToHandle(fontHandle);
}

void TutorialScene::update(const InputState& input)
{

	SetVolumeMusic(musicVolume);

	if (--time < 0) {
		imgX++;
		imgX2++;
		time = 8;
	}

	player->update(offset, input);

	if (player->getPos().x > 1500) {
		enemy->setPos(2200);
	}

	if (spownCount > 0) {
		if (player->getPos().x > 550) {
			enemy->dispatch({ 300,750 });
			spownCount--;
		}
	}

	if (player->getPos().x > 550) {
		if (!enemy->isEnable()) {
			enemy->dispatch({ 0,750 });
			enemy->setPlayer(player, enemyHandle, coinHandle);
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
			targetPlayer2.x = player->getPos().x + 25 - 2100;
			targetPlayer2.y = player->getPos().y + 44 - 780;

			targetPlayer3 = { 0.0f,0.0f };
			targetPlayer3.x = player->getPos().x + 25 - 2500;
			targetPlayer3.y = player->getPos().y + 44 - 780;

			targetPlayer4 = { 0.0f,0.0f };
			targetPlayer4.x = player->getPos().x + 25 - 3000;
			targetPlayer4.y = player->getPos().y + 44 - 300;

			targetPlayer5 = { 0.0f,0.0f };
			targetPlayer5.x = player->getPos().x + 25 - 3500;
			targetPlayer5.y = player->getPos().y + 44 - 780;

			targetPlayer6 = { 0.0f,0.0f };
			targetPlayer6.x = player->getPos().x + 25 - 3000;
			targetPlayer6.y = player->getPos().y + 44 - 780;

			targetPlayer7 = { 0.0f,0.0f };
			targetPlayer7.x = player->getPos().x + 25 - 2520;
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
						//manager_.pushScene(new TextScene(manager_, input, 0, offset, bottanHandle));
						textCount = 1;
					}
				}
				break;
			case 1:
				if (textCount == 1) {
					if (targetPlayer2.length() < 200) {
						//manager_.pushScene(new TextScene(manager_, input, 1, offset, bottanHandle));
						textCount = 2;
					}
				}
				break;
			case 2:
				if (textCount == 2) {
					if (targetPlayer3.length() < 200) {
						//manager_.pushScene(new TextScene(manager_, input, 2, offset, bottanHandle));
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
			if (player->getPos().x > 3650) {
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

	if (player->getPos().x < 500) {
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(300, 50, 1620, 520, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(300, 50, 1620, 520, 0xffffff, false);
		DrawRotaGraph(Game::kScreenWidth / 2 - 30, Game::kScreenHeight / 2 - 280, 6.0f, 0.0f, descriptionHandle3, true);
		int size = GetDrawStringWidthToHandle("ï˚å¸ÉLÅ[ ñîÇÕ ç∂ÉXÉeÉBÉbÉNÇ≈à⁄ìÆ", strlen("ï˚å¸ÉLÅ[ ñîÇÕ ç∂ÉXÉeÉBÉbÉNÇ≈à⁄ìÆ"), fontHandle);
		DrawStringToHandle(Game::kScreenWidth / 2 - size / 2, 460, "ï˚å¸ÉLÅ[ ñîÇÕ ç∂ÉXÉeÉBÉbÉNÇ≈à⁄ìÆ", 0xffffff, fontHandle);
	}

	if (targetPlayer.length() < 200 && personNum == 1) {
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(300, 50, 1620, 520, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(300, 50, 1620, 520, 0xffffff, false);
		DrawRotaGraph(Game::kScreenWidth / 2 + 70, Game::kScreenHeight / 2 - 280, 6.0f, 0.0f, descriptionHandle, true);
		DrawStringToHandle(Game::kScreenWidth / 2 + 30, 350, "âüÇµÇƒÇ¢ÇÈä‘", 0xffffff, fontHandle);
		DrawStringToHandle(Game::kScreenWidth / 2 + 20, 400, "ìGÇ©ÇÁâBÇÍÇ‹Ç∑", 0xffffff, fontHandle);
	}

	if (targetPlayer2.length() < 200 && personNum == 2) {
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(300, 50, 1620, 520, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(300, 50, 1620, 520, 0xffffff, false);
		DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 - 240,6.0f,0.0f, descriptionHandle2, true);
		DrawStringToHandle(Game::kScreenWidth / 2, 400, "or", 0xffffff, fontHandle);
		DrawStringToHandle(580, 460, "éaÇËÇ¬ÇØÇÈ", 0xffffff, fontHandle);
		DrawStringToHandle(Game::kScreenWidth / 2 + 250, 460, "ìäù±", 0xffffff, fontHandle);
	}

	if (enemy->isEnable()) {
		enemy->draw(offset);
	}
	
	if (targetPlayer7.length() < 300) {
		if (player->setMoneyPossessed() > 0) {
			textDraw(0);
		}
	}
	

	if (targetPlayer4.length() < 200) {
		textDraw(2);
		count--;
	}

	if (space->isEnable()) {
		space->draw(offset);
	}
	
	if (targetPlayer6.length() < 200) {
		textDraw(1);
	}

	//ìXàı
	switch (personNum) {
	case 3:
		DrawRotaGraph(3400 + offset.x, 780, 2.0f, 0.0f, shopperHandle, true);
		break;
	}
	
	if (player->getPos().x > 3650) {
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

	bool checkFlyingObject = player->returnFlyingisEnabled();

	if (checkFlyingObject) {
		DrawString(Game::kScreenWidth / 2 - 130, 900, "ìäÇ∞ÇΩÇ‡ÇÃÇÕÇøÇ·ÇÒÇ∆èEÇ¢Ç‹ÇµÇÂÇ§", 0xffffff);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue);
	//âÊñ ëSëÃÇê^Ç¡çïÇ…ìhÇËÇ¬Ç‘Ç∑
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TutorialScene::fadeInUpdate(const InputState& input)
{
	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	if (--fadeTimer == 0) {
		updateFunc = &TutorialScene::normalUpdate;
		PlayMusic("data/music/Empty Streets.wav", DX_PLAYTYPE_LOOP);
		fadeValue = 0;
	}
}

void TutorialScene::normalUpdate(const InputState& input)
{

	int x = (2520) / chipSize;
	int y = (780) / chipSize;

	if (player->shopCollision(x, y, offset)) {
		if (input.isTriggered(InputType::next))
		{
			manager_.pushScene(new Shop(manager_, input, player, portionHandle, guiHandle, hpHandle, repairHandle,coinHandle));
			buy = true;
		}
	}


	Vec2 targetOffset{};

	targetOffset.x = (Game::kScreenWidth / 2) - player->getPos().x;
	if (targetOffset.x > 0)
	{
		targetOffset.x = 0;
	}
	if (targetOffset.x < -field->getTutorialWidth() + Game::kScreenWidth)
	{
		targetOffset.x = -field->getTutorialWidth() + Game::kScreenWidth;
	}


	offset = targetOffset;
}

void TutorialScene::clearFadeOutUpdate(const InputState& input)
{
	if (--musicVolume > 1) {
		SetVolumeMusic(musicVolume);
	}

	fadeValue = 255 * (static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	truckPos += 10;
	if (++fadeTimer == fadeInterval) {
		StopMusic();
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
		DrawBox(2370 + offset.x, 570, 2653 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(2370 + offset.x, 570, 2653 + offset.x, 720, 0xffffff, false);
		DrawString(2405 + offset.x, 620, "Ç®ã‡ÇèEÇ¡ÇƒîÉÇ¢ï®ÇÇµÇÊÇ§", 0xffffff);

		DrawRectRotaGraph(2510 + offset.x, 670, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
		break;
	case 1:

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(2970 + offset.x, 570, 3170 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(2970 + offset.x, 570, 3170 + offset.x, 720, 0xffffff, false);

		if (--time2 == 0) {
			imgX++;
			time2 = 8;
		}

		if (imgX > 4) {
			imgX = 0;
		}
		DrawString(3055 + offset.x, 680, "ìoÇÈ", 0xffffff);
		DrawRectRotaGraph(3070 + offset.x, 635, imgX * 112, 0, 112, 153, 0.4f, 0.0f, arrowHandle, true, false);
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
		DrawBox(2870 + offset.x, 20, 3250 + offset.x, 60, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(2870 + offset.x, 20, 3250 + offset.x, 60, 0xffffff, false);
		DrawString(2990 + offset.x, 35, "ÉQÅ[ÉgÇîjâÛÇµÇÊÇ§", 0xff0000);

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(2870 + offset.x, 70, 3250 + offset.x, 220, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(2870 + offset.x, 70, 3250 + offset.x, 220, 0xffffff, false);
		DrawString(2980 + offset.x, 95, "éËéùÇøÇêÿÇËë÷Ç¶ÇÊÇ§", 0xffffff);

		DrawRectRotaGraph(3010 + offset.x, 140, imgX2 * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle2, true, false);
		DrawString(3050 + offset.x, 150, "or", 0xffffff);
		DrawRectRotaGraph(3110 + offset.x, 140, imgX2 * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle2, true, false);
		DrawRectRotaGraph(2930 + offset.x, 190, imgX * 16, (imgY - 2) * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
		DrawString(2950 + offset.x, 180, "ÇâüÇµÇƒÇ¢ÇÈä‘ÅAÉAÉCÉeÉÄÇégóp", 0xffffff);

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//èÊéZçáê¨
		DrawBox(3040 - graphWidth * 2 - 50 + offset.x, 480, 3040 + graphWidth * 2 + 50 + offset.x, 650 + graphHeight * 2 - 30, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//í èÌï`âÊÇ…ñﬂÇ∑
		DrawBox(3040 - graphWidth * 2 - 50 + offset.x, 480, 3040 + graphWidth * 2 + 50 + offset.x, 650 + graphHeight * 2 - 30, 0xffffff, false);
		DrawRotaGraph(3040 + offset.x, 670, 4.0f, 0.0f, descriptionHandle4, true);
		DrawString(3310 + offset.x, 520, "îjâÛ", 0xff0000);

		break;
	}
}