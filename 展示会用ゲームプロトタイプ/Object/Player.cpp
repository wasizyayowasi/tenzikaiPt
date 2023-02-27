#include "Player.h"
#include "PlayerThrowinAttack.h"

#include "PlayerMotion.h"
#include "ObjectHp.h"
#include "Inventory.h"
#include "../field.h"

#include "../DrawFunctions.h"
#include "../InputState.h"
#include "DxLib.h"
#include <algorithm>



namespace {
	constexpr float correctionSizeX = 14.0f;
	constexpr float correctionSizeY = 74.0f;
	constexpr int ultimate_frames = 120;
}

Player::Player()
{
	playerHandle =my::myLoadGraph("data/player.png");

	hp = new ObjectHp;
	motion = new PlayerMotion;
	inventory = new Inventory();
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
	updateFunc = &Player::updateField;
	
}

Player::~Player()
{
	delete hp;
	delete motion;
	delete inventory;

	DeleteGraph(playerHandle);
}

void Player::init()
{
	inventory->setHandle(portionHandle, macheteHandle,guiHandle,repairHandle);
}

void Player::update(Vec2 offset, const InputState& input)
{
	
	inventory->update(input);
	hp->setObjectHp(playerHp);

	if (flyingObject->isEnable()) {
		flyingObject->update(offset);
	}

	ultimateTimer_ = (std::max)(ultimateTimer_ - 1, 0);

	(this->*updateFunc)(offset,input);

	if (playerPos.x < 0) {
		playerPos.x = 0;
	}
	if (playerPos.x + 50 > Game::kScreenWidth*2) {
		playerPos.x = Game::kScreenWidth * 2 - 50;
	}

	if (playerHp < 1) {
		motionNum = 4;
		isEnabled = false;
		updateFunc = &Player::updateDeath;
	}
}

void Player::tutorialUpdate(Vec2 offset, const InputState& input)
{
	//モーション関係
	motionNum = 0;
	motion->update(motionNum);

	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//配列の中身を見る
	int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX];

	//梯子を降りる
	if (chipNo == 119 || chipNo == 121) {
		if (input.isPressed(InputType::down)) {
			playerPos.y += 3.0f;
			if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
			}
		}
	}

	//梯子を上る
	for (int x = -1; x < 1; x++) {
		int chipNo = buildingData::building[underfootChipNoY - 1][underfootChipNoX - x];
		if (chipNo == 119 || chipNo == 121) {
			if (input.isPressed(InputType::up)) {
				if (ladderCollision(underfootChipNoX - x, underfootChipNoY - 1)) {
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	//降下
	int DescentChipNo3 = groundData::ground[underfootChipNoY][underfootChipNoX];
	int DescentChipNo4 = buildingData::building[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0 && DescentChipNo4 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//移動
	if (!push) {
		if (input.isPressed(InputType::left)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			motion->update(motionNum);
			playerPos.x -= 5;
			playerDirections = true;
		}
		else if (input.isPressed(InputType::right)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			motion->update(motionNum);
			playerPos.x += 5;
			playerDirections = false;
		}
	}


	//近接攻撃
	if (!push) {
		if (!flyingObject->isEnable()) {
			if (input.isPressed(InputType::attack)) {
				motionNum = 3;
				motion->update(motionNum);
				proximityAttack = true;
			}
			else {
				proximityAttack = false;
			}
		}
	}

	//アイテムの切り替え
	switch (inventory->setCurrentInputIndex()) {
	case 0:
		//投擲
		if (!push) {
			if (input.isTriggered(InputType::shot)) {
				if (!flyingObject->isEnable()) {
					flyingObject->attack(playerPos, playerDirections);
				}
			}
			if (flyingObject->landing()) {
				if (flyingObject->playerCollision(playerPos, offset)) {
					flyingObject->deadFlyingObject();
				}
			}
		}
		break;
	case 1:
		//修復タイマー
		if (!flyingObject->isEnable()) {
			if (repairBlock > 0) {
				if (input.isPressed(InputType::shot)) {
					spaceHpDisplay = true;
				}
				else {
					spaceHpDisplay = false;
				}
			}
			else {
				spaceHpDisplay = false;
			}
		}
		break;
	case 2:
		if (recoveryItem > 0) {
			if (input.isTriggered(InputType::shot)) {
				if (playerHp < 10) {
					playerHp = (std::min)({ playerHp + 5, 10 });
					recoveryItem--;
				}
			}
		}
		break;
	}

	//隠れる処理
	int objectChipNo = objectData::object[underfootChipNoY - 1][underfootChipNoX];
	if (objectChipNo == 36 || objectChipNo == 37) {
		if (objectCollision(underfootChipNoX, underfootChipNoY - 1)) {
			if (input.isPressed(InputType::next)) {
				DrawString(300, 100, "aiueo", 0xffffff);
				push = true;
			}
			else {
				push = false;
			}
		}
	}
	else {
		hidden = false;
		push = false;
	}
}

void Player::updateField(Vec2 offset, const InputState& input)
{
	//モーション関係
	motionNum = 0;
	motion->update(motionNum);

	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) /chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//配列の中身を見る
	int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX];

	//梯子を降りる
	if (chipNo == 119 || chipNo == 121) {
		if (input.isPressed(InputType::down)) {
			playerPos.y += 3.0f;
			if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
			}
		}
	}

	//梯子を上る
	for (int x = -1; x < 1; x++) {
		int chipNo = buildingData::building[underfootChipNoY - 1][underfootChipNoX - x];
		if (chipNo == 119 || chipNo == 121) {
			if (input.isPressed(InputType::up)) {
				if (ladderCollision(underfootChipNoX - x, underfootChipNoY - 1)) {
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	//降下
	int DescentChipNo3 = groundData::ground[underfootChipNoY][underfootChipNoX];
	int DescentChipNo4 = buildingData::building[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0 && DescentChipNo4 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//移動
	if (!push) {
		if (input.isPressed(InputType::left)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			motion->update(motionNum);
			playerPos.x -= 5;
			playerDirections = true;
		}
		else if (input.isPressed(InputType::right)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			motion->update(motionNum);
			playerPos.x += 5;
			playerDirections = false;
		}
	}
	

	//近接攻撃
	if (!push) {
		if (!flyingObject->isEnable()) {
			if (input.isPressed(InputType::attack)) {
				motionNum = 3;
				motion->update(motionNum);
				proximityAttack = true;
			}
			else {
				proximityAttack = false;
			}
		}
	}

	//アイテムの切り替え
	switch (inventory->setCurrentInputIndex()) {
	case 0:
		//投擲
		if (!push) {
			if (input.isTriggered(InputType::shot)) {
				if (!flyingObject->isEnable()) {
					flyingObject->attack(playerPos, playerDirections);
				}
			}
			if (flyingObject->landing()) {
				if (flyingObject->playerCollision(playerPos, offset)) {
					flyingObject->deadFlyingObject();
				}
			}
		}
		break;
	case 1:
		//修復タイマー
		if (!flyingObject->isEnable()) {
			if (repairBlock > 0) {
				if (input.isPressed(InputType::shot)) {
					spaceHpDisplay = true;
				}
				else {
					spaceHpDisplay = false;
				}
			}
			else {
				spaceHpDisplay = false;
			}
		}
		break;
	case 2:
		if (recoveryItem > 0) {
			if (input.isTriggered(InputType::shot)) {
				if (playerHp < 10) {
					playerHp = (std::min)({ playerHp + 5, hp->returnMaxHp()});
					recoveryItem--;
				}
			}
		}
		break;
	}
	
	//隠れる処理
	int objectChipNo = objectData::object[underfootChipNoY - 1][underfootChipNoX];
	if (objectChipNo == 36 || objectChipNo == 37) {
		if (objectCollision(underfootChipNoX, underfootChipNoY - 1)) {
			if (input.isPressed(InputType::next)) {
				DrawString(300, 100, "aiueo", 0xffffff);
				push = true;
			}
			else {
				push = false;
			}
		}
	}
	else {
		hidden = false;
		push = false;
	}
}

void Player::updateDescent(Vec2 offset, const InputState& input)
{
	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + chipSize) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//降下
	playerPos += vec;

	//地面との判定
	for (int x = 0; x < bgNumX; x++) {
		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = groundData::ground[y][x];

			//地面
			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				if (playerFiledCollision(x, y)) {
					if (playerPos.y + correctionSizeY > y * chipSize) {
						playerPos.y = y * chipSize - 74;
					}
					updateFunc = &Player::updateField;
					return;
				}
			}
		}
	}

	//梯子との判定
	for (int x = -1; x < 1; x++) {

		const int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX + x];
		//地面
		if (chipNo == 119 || chipNo == 121) {
			if (objectCollision(underfootChipNoX + x, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
				return;
			}
		}
	}

}

void Player::updateLadder(Vec2 offset, const InputState& input)
{
	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 2) / chipSize;

	//登りきった後
	if (input.isPressed(InputType::up)) {

		motionNum = 2;
		playerPos.y -= 5;
		motion->update(motionNum);
		
		int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX];
		//地面
		if (chipNo == 0 || chipNo == 106|| chipNo == 108) {
			if (playerFiledCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateField;
				return;
			}
		}
	}

	//下ったとき
	if (input.isPressed(InputType::down)) {
		motionNum = 2;
		playerPos.y += 5;
		motion->update(motionNum);

		int chipNo = groundData::ground[underfootChipNoY + 1][underfootChipNoX];
		//地面
		if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
			if (playerFiledCollision(underfootChipNoX, underfootChipNoY + 1)) {
				motionNum = 0;
				updateFunc = &Player::updateField;
				return;
			}
		}
	}

	
	//落下
	//chipNo3 = buildingData::building[underfootChipNoY + 1][underfootChipNoX];
	//if (chipNo3 == 0) {
	//	motionNum = 0;
	//	motion->update(motionNum);
	//	//updateFunc = &Player::updateDescent;
	//}
}

void Player::updateDeath(Vec2 offset, const InputState& input)
{
	motion->update(motionNum);
	if (motion->setDead()) {
		gameoverScene = true;
	}
}

void Player::setItemControl(int num)
{
	switch (num) {
	case 0:
		hp->setObjectMaxHp(hp->returnMaxHp() + 1);
		break;
	case 1:
		repairBlock++;
		break;
	case 2:
		recoveryItem++;
		break;
	}
}

void Player::setMoney(int amount)
{
	money -= amount;
}

void Player::consumption()
{
	repairBlock--;
}

//描画
void Player::draw(Vec2 offset)
{
	hp->playerHpDraw(hpHandle);

	inventory->setNum(repairBlock, recoveryItem, flyingObject->isEnable());
	inventory->draw();

	DrawRotaGraph(1810, 50, 2.0f, 0.0f, coinHandle, true);
	DrawFormatString(1845, 50, 0xffffff, "%d", money);

	//飛び道具
	if (flyingObject->isEnable()) {
		flyingObject->draw(macheteHandle, offset);
	}

	if (ultimateTimer_ > 0) {
		if ((ultimateTimer_ / 10) % 2 == 0) {
			return;
		}
	}

	if (!push) {
		motion->draw(playerPos, playerHandle, playerDirections, offset);
	}
}


//隠れる
bool Player::beHidden()
{
	return push;
}



void Player::damege(bool inversion)
{
	if (ultimateTimer_ <= 0) {
		if (!inversion) {
			playerPos.x += 30.0f;
		}
		else {
			playerPos.x -= 30.0f;
		}

		if (playerHp > 0) {
			playerHp--;
		}
		ultimateTimer_ = ultimate_frames;
	}

}

int Player::enemyAttack(Vec2 enemyPos, Vec2 offset)
{
	if (flyingObject->enemyCollision(enemyPos,offset)) {
		enemyHit = true;
	}
	else {
		enemyHit = false;
	}

	return enemyHit;
}



//++++++++++++++++++++++++++++++++++
//　　　　　　当たり判定
//++++++++++++++++++++++++++++++++++

//近接攻撃の当たり判定
bool Player::proximityAttackCollision(const Vec2& pos)
{
	float enemyLeft = pos.x;
	float enemyTop = pos.y;
	float enemyRight = pos.x + 30;
	float enemyBottom = pos.y + 30;

	if (proximityAttack) {
		if (playerDirections) {
			if (enemyLeft > playerPos.x + 20)			return false;
			if (enemyRight < playerPos.x - 50)			return false;
			if (enemyTop <= playerPos.y + 10)			return false;
			if (enemyBottom <= playerPos.y + 50)		return false;
		}
		else if (!playerDirections) {
			if (enemyLeft > playerPos.x  + 80)			return false;
			if (enemyRight < playerPos.x + 20)				return false;
			if (enemyTop <= playerPos.y + 10)			return false;
			if (enemyBottom <= playerPos.y + 50)		return false;
		}
		return true;
	}
	return false;
}

//空間を修復する
bool Player::repairSpace(const Vec2& pos,Vec2 offset)
{
	float spaceLeft = pos.x + offset.x;
	float spaceTop = pos.y + offset.y;
	float spaceRight = pos.x + chipSize * 3 + offset.x;
	float spaceBottom = pos.y + chipSize * 3 + offset.y;

	float playerLeft = playerPos.x + 14 + offset.x;
	float playerTop = playerPos.y + offset.y;
	float playerRight = playerPos.x + 15 + offset.x;
	float playerBottom = playerPos.y + 74 + offset.y;

	if (playerRight < spaceLeft)				return false;
	if (playerLeft > spaceRight)				return false;
	if (playerBottom < spaceTop)				return false;
	if (playerTop > spaceBottom)				return false;
		
	return true;

}

bool Player::playerFiledCollision(int x,int y)
{
	float fieldLeft = x * chipSize;
	float fieldRight = x * chipSize + chipSize;
	float fieldTop = y * chipSize;
	float fieldBottom = y * chipSize + chipSize;

	if (playerPos.x + 15.0f < fieldLeft)				return false;
	if (playerPos.x + correctionSizeX > fieldRight)		return false;
	if (playerPos.y + correctionSizeY < fieldTop)		return false;
	if (playerPos.y > fieldBottom)						return false;

	return true;
}

bool Player::objectCollision(int x,int y)
{

	float objectLeft = x * chipSize;
	float objectRight = x * chipSize  + chipSize ;
	float objectTop = y * chipSize;
	float objectBottom = y * chipSize + chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;
		
	return true;
}

bool Player::ladderCollision(int x, int y)
{
	float objectLeft = x * chipSize ;
	float objectRight = x * chipSize + chipSize;
	float objectTop = y * chipSize;
	float objectBottom = y * chipSize + chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;

	return true;
}

bool Player::coinCollision(Vec2 pos, Vec2 offset)
{
	float objectLeft =pos.x + offset.x;
	float objectRight = pos.x + 30 + offset.x;
	float objectTop = pos.y + offset.y;
	float objectBottom = pos.y + 30 + offset.y;

	if (playerPos.x + 15.0f + offset.x < objectLeft)				return false;
	if (playerPos.x + correctionSizeX + offset.x > objectRight)		return false;
	if (playerPos.y + correctionSizeY + offset.y < objectTop)		return false;
	if (playerPos.y + offset.y > objectBottom)						return false;

	money += 1000;

	return true;
}

bool Player::shopCollision(int x, int y, Vec2 offset)
{
	float objectLeft = x * chipSize;
	float objectRight = x * chipSize + chipSize;
	float objectTop = y * chipSize;
	float objectBottom = y * chipSize + chipSize * 3;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	DrawString(300, 400, "aiu", 0xffffff);
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	DrawString(300, 400, "aiu", 0xffffff);
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	DrawString(300, 400, "aiu", 0xffffff);
	if (playerPos.y > objectBottom)						return false;

	return true;
}






