#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"
#include"../field.h"
#include "SceneManager.h"
#include "Gameover.h"
#include "../InputState.h"
#include "GameClear.h"
#include "Pause.h"
#include "Shop.h"


GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc(&GameMain::fadeInUpdate)
{
	enemyHandle = LoadGraph("data/enemy.png");
	playerHandle = LoadGraph("data/player.png");

	player = new Player;
	//空間のデータを作る
	for (int i = 0; i < 3;i++) {
		space[i] = std::make_shared<BugSpace>(i);
	}
	//プレイヤーのデータを送る
	for (auto& space : space) {
		space->setPlayer(player);
	}
	for (auto& space : space) {
		space->enemySetPlayer(enemyHandle);
	}
	field = std::make_shared<Field>();

	init();
}

GameMain::~GameMain()
{
	delete player;
	DeleteGraph(enemyHandle);
}


void GameMain::init()
{

	int i = 0;
	
	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {
			const int chipNo = FieldData::field[y][x];
			if (chipNo == 4) {
				noLongerUsedX[i] = x;
				noLongerUsedY[i] = y;
				i++;
			}
		}
	}

	i = 0;

	//空間の場所をランダムで設定
	for (auto& space : space) {
		space->init(noLongerUsedX[i],noLongerUsedY[i]);
		i++;
	}
}

void GameMain::update(const InputState& input)
{
	(this->*updateFunc)(input);
}

void GameMain::draw()
{

	//フィールドの描画
	field->draw(offset);

	//空間の描画
	for (auto& space : space) {
		if (space->isEnable()) {
			space->draw(offset);
		}
	}

	//網目の描画
	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {
			DrawBox(x * FieldData::chipSize, y * FieldData::chipSize, x * FieldData::chipSize + FieldData::chipSize, y * FieldData::chipSize + FieldData::chipSize, 0x224422, false);
		}
	}

	//プレイヤーの描画
	player->draw(playerHandle,offset);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, FadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0) {
		updateFunc = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameMain::normalUpdate(const InputState& input)
{
	//プレイヤーの更新
	if (player->isEnable()) {
		player->update(offset,input);
		//入力装置の情報を読み取る
		Pad::update();
	}
	else {
		player->updateDeath(offset, input);
		if (player->nextScene()) {
			updateFunc = &GameMain::gameoverFadeOutUpdate;
		}
	}

	if (player->isEnable()) {
		//空間の更新
		for (auto& space : space) {
			if (space->isEnable()) {
				space->update(offset);
			}
		}
	}

	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {
			const int chipNo = FieldData::field[y][x];
			if (chipNo == 6) {
				if (player->shopCollision(x, y, offset)) {
					if (input.isTriggered(InputType::next))
					{
						manager_.pushScene(new Shop(manager_,input));
					}
				}
			}
		}
	}

	clearCount = 0;

	for (auto& space : space) {
		if (!space->isEnable()) {
			clearCount++;
		}
	}

	if (clearCount == 3) {
		updateFunc = &GameMain::clearFadeOutUpdate;
	}

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


	offset = targetOffset * 0.2f + offset * 0.8f;


	
	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new Pause(manager_));
	}

}

void GameMain::gameoverFadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new Gameover(manager_));
		return;
	}
}

void GameMain::clearFadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.changeScene(new GameClear(manager_));
		return;
	}
}

