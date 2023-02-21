#include "Player.h"
#include "../Pad.h"
#include "DxLib.h"
#include "PlayerThrowinAttack.h"
#include "ObjectHp.h"
#include "../field.h"
#include "PlayerMotion.h"
#include "../InputState.h"
#include "Inventory.h"
#include <algorithm>



namespace {
	constexpr float correctionSizeX = 14.0f;
	constexpr float correctionSizeY = 74.0f;
}

Player::Player()
{
	hp = new ObjectHp;
	motion = new PlayerMotion;
	inventory = new Inventory;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
	updateFunc = &Player::updateField;
}

void Player::update(Vec2 offset, const InputState& input)
{

	inventory->update(input);
	hp->setObjectHp(playerHp);

	(this->*updateFunc)(offset,input);

	if (playerHp < 1) {
		motionNum = 4;
		isEnabled = false;
		updateFunc = &Player::updateDeath;
	}
}

void Player::updateField(Vec2 offset, const InputState& input)
{

	collision = false;

	motionNum = 0;
	motion->update(motionNum);

	int underfootChipNoX = (playerPos.x + correctionSizeX) / FieldData::chipSize;
	int underfootChipNoY = (playerPos.y + FieldData::chipSize * 3) / FieldData::chipSize;


	int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX];
	//梯子
	if (chipNo == 2) {
		if (input.isPressed(InputType::down)) {
			playerPos.y += 3.0f;
		}
		if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
			updateFunc = &Player::updateLadder;
		}
	}

	for (int x = -1; x < 1; x++) {
		int chipNo = FieldData::field[underfootChipNoY - 1][underfootChipNoX - x];
		if (chipNo == 2) {
			if (input.isPressed(InputType::up)) {
				if (!collision) {
					collision = ladderCollision(underfootChipNoX - x, underfootChipNoY - 1);
				}
				if (collision) {
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	//降下
	int DescentChipNo3 = FieldData::field[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//移動
	if (move) {
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
	}

	//HPの表示
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}

	//近接攻撃
	if (!push) {
		if (!flyingObject->isEnable()) {
			if (Pad::isPress(PAD_INPUT_2)) {
				motionNum = 3;
				motion->update(motionNum);
				proximityAttack = true;
			}
			else {
				proximityAttack = false;
			}
		}
	}


	//デバッグ用
	{
		/*if (input.isTriggered(InputType::prev)) {
		repairBlock++;
	}

	if (input.isTriggered(InputType::next)) {
		recoveryItem++;
	}*/
	}

	switch (inventory->setCurrentInputIndex()) {
	case 0:
		//投擲
		if (!push) {
			if (input.isTriggered(InputType::shot)) {
				if (!flyingObject->isEnable()) {
					flyingObject->attack(playerPos, playerDirections);
				}
			}
			if (flyingObject->isEnable()) {
				flyingObject->update(offset);
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
					hpDisplay = true;
					hpDisplayTime = 120;
					recoveryItem--;
				}
			}
		}
		break;
	}
	

	


	int objectChipNo = FieldData::field[underfootChipNoY - 1][underfootChipNoX];
	

	if (objectChipNo == 3) {
		if (objectCollision(underfootChipNoX, underfootChipNoY - 1)) {
			if (input.isPressed(InputType::next)) {
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

	int underfootChipNoX = (playerPos.x + FieldData::chipSize) / FieldData::chipSize;
	int underfootChipNoY = (playerPos.y + FieldData::chipSize * 3) / FieldData::chipSize;

	playerPos += vec;

	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {

			const int chipNo = FieldData::field[y][x];

			//地面
			if (chipNo == 1) {
				if (playerFiledCollision(x, y)) {
					if (playerPos.y + correctionSizeY > y * FieldData::chipSize) {
						playerPos.y = y * FieldData::chipSize - 74;
					}
					updateFunc = &Player::updateField;
					return;
				}
			}
		}
	}

	for (int x = -1; x < 1; x++) {

		const int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX + x];
		//地面
		if (chipNo == 2) {
			if (objectCollision(underfootChipNoX + x, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
				return;
			}
		}
	}

}

void Player::updateLadder(Vec2 offset, const InputState& input)
{
	int underfootChipNoX = (playerPos.x + correctionSizeX) / FieldData::chipSize;
	int underfootChipNoY = (playerPos.y + FieldData::chipSize * 2) / FieldData::chipSize;

	//使うな
	{
		//bool noLeftEntry = false;
		//bool noRightEntry = false;

		//for (int y = -2; y < 1; y++) {
		//	int chipNo9 = FieldData::field[underfootChipNoY + y][underfootChipNoX + 1];
		//	if (chipNo9 == 1) {
		//		noLeftEntry = true;
		//	}
		//}

		//for (int y = -2; y < 1; y++) {
		//	int chipNo10 = FieldData::field[underfootChipNoY + y][underfootChipNoX];
		//	if (chipNo10 == 1) {
		//		noRightEntry = true;
		//	}
		//}

		////移動
		//if (!noRightEntry) {
		//	if (input.isPressed(InputType::left)) {
		//		motion->update(motionNum);
		//		playerDirections = true;
		//	}
		//}
		//if (!noLeftEntry) {
		//	if (input.isPressed(InputType::right)) {
		//		motion->update(motionNum);
		//		playerPos.x += 5;
		//		playerDirections = false;
		//	}
		//}
	}

	if (input.isPressed(InputType::up)) {

		motionNum = 2;
		playerPos.y -= 5;
		motion->update(motionNum);
		

		int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX];
		//地面
		if (chipNo == 0) {
			if (playerFiledCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateField;
				return;
			}
		}
	}
	if (input.isPressed(InputType::down)) {
		motionNum = 2;
		playerPos.y += 5;
		motion->update(motionNum);

		int chipNo = FieldData::field[underfootChipNoY + 1][underfootChipNoX];
		//地面
		if (chipNo == 1) {
			if (playerFiledCollision(underfootChipNoX, underfootChipNoY + 1)) {
				motionNum = 0;
				updateFunc = &Player::updateField;
				return;
			}
		}
	}

	
	//落下
	chipNo3 = FieldData::field[underfootChipNoY + 1][underfootChipNoX];
	if (chipNo3 == 0) {
		motionNum = 0;
		motion->update(motionNum);
		updateFunc = &Player::updateDescent;
	}
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
void Player::draw(int handle, Vec2 offset)
{
	//デバッグ用
	{
		/*if (proximityAttack) {
			if (playerDirections) {
				DrawBox(playerPos.x - 50, playerPos.y + 10, playerPos.x  + 20, playerPos.y + 50, 0xff00ff, true);
				DrawString(playerPos.x - 65, playerPos.y + 10, "近接攻撃", 0x000000);
			}
			else {
				DrawBox(playerPos.x + 20, playerPos.y + 10, playerPos.x + 80, playerPos.y + 50, 0xff00ff, true);
				DrawString(playerPos.x + 55, playerPos.y + 10, "近接攻撃", 0x000000);
			}
		}*/

		/*if (updateFunc == &Player::updateField) {
			int aiuX = (playerPos.x + 14) / Field::chipSize;
			int aiuY = (playerPos.y + Field::chipSize * 3) / Field::chipSize;
			int color = 0xff0000;
			DrawString(0, 150, "フィールド", 0xffffff);
			DrawBox(aiuX * Field::chipSize, aiuY * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, aiuY * Field::chipSize + Field::chipSize, 0xff0000, true);
			if (Field::field[aiuY - 1][aiuX] == 2) {
				color = 0x00ff00;
			}
			DrawBox(aiuX * Field::chipSize, (aiuY - 1) * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, (aiuY - 1) * Field::chipSize + Field::chipSize, color, true);
		}
		else if (updateFunc == &Player::updateLadder) {
			int aiuX = (playerPos.x + 14) / Field::chipSize;
			int aiuY = (playerPos.y + Field::chipSize * 2) / Field::chipSize;
			DrawString(0, 150, "梯子", 0xffffff);
			if (Pad::isPress(PAD_INPUT_UP)) {
				DrawBox(aiuX * Field::chipSize, (aiuY ) * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, (aiuY ) * Field::chipSize + Field::chipSize, 0x00ff00, true);
			}
			if (Pad::isPress(PAD_INPUT_DOWN)) {
				DrawBox(aiuX * Field::chipSize, (aiuY+1) * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, (aiuY + 1) * Field::chipSize + Field::chipSize, 0x00ff00, true);
			}
			DrawBox((aiuX + 1) * Field::chipSize, (aiuY - 2) * Field::chipSize, (aiuX + 1) * Field::chipSize + Field::chipSize, (aiuY - 2) * Field::chipSize + Field::chipSize, 0x00ff00, true);
			DrawBox((aiuX + 1) * Field::chipSize, (aiuY - 1) * Field::chipSize, (aiuX + 1) * Field::chipSize + Field::chipSize, (aiuY - 1) * Field::chipSize + Field::chipSize, 0x00ff00, true);
			DrawBox((aiuX + 1) * Field::chipSize, aiuY * Field::chipSize, (aiuX + 1) * Field::chipSize + Field::chipSize, aiuY * Field::chipSize + Field::chipSize, 0x00ff00, true);
		}
		else if (updateFunc == &Player::updateDescent) {
			int aiuX = (playerPos.x) / Field::chipSize;
			int aiuY = (playerPos.y + Field::chipSize * 3) / Field::chipSize;
			DrawString(0, 150, "降下", 0xffffff);
			DrawBox(aiuX * Field::chipSize, aiuY * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, aiuY * Field::chipSize + Field::chipSize, 0x0000ff, true);
		}
		DrawBox(playerPos.x, playerPos.y, playerPos.x + 25, playerPos.y + 74, 0xffffff, true);
		DrawLine(playerPos.x + 15, playerPos.y , playerPos.x + 15, playerPos.y + 74, 0xffffff);
		DrawFormatString(0, 105, 0xffffff, "%d:%d", aiuX2, aiuY2);
		DrawFormatString(0, 120, 0xffffff, "%d", chipNo3);
		DrawFormatString(0, 165, 0xffffff, "%f : %f", playerPos.x + 24,playerPos.y);
		DrawBox(playerPos.x + 10, playerPos.y + Field::chipSize + 10, playerPos.x + Field::chipSize + 10, playerPos.y + Field::chipSize*2 + 10, 0xffffff, false);
		DrawBox(playerPos.x, playerPos.y, playerPos.x + Field::chipSize, playerPos.y + Field::chipSize, 0xffffff, false);

		DrawBox(objectLeft, objectTop, objectRight, objectBottom, 0xff00ff, true);
		DrawBox(playerPos.x + 24.0f, playerPos.y, playerPos.x + 25.0f, playerPos.y + 74, 0x00ff00, true);*/
	}

	DrawFormatString(400, 50, 0xffffff, "%d", push);

	//飛び道具
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	if (hpDisplay) {
		hp->draw(playerPos,offset);
	}

	motion->draw(playerPos, handle, playerDirections, offset);

	inventory->draw();

	//DrawFormatString(312, 948, 0x000000, "x%d", repairBlock);

	if (!flyingObject->isEnable()) {
		DrawString(200, 845, "マチェッエト", 0x000000);
		DrawBox(216, 916, 248, 948, 0xff00ff, true);
	}
	if (repairBlock > 0) {
		DrawString(280, 860, "修復ブロック", 0x000000);
		DrawBox(280, 916, 312, 948, 0x6f4b2c, true);
		DrawFormatString(312, 948, 0x000000, "x%d", repairBlock);
	}
	if (recoveryItem > 0) {
		DrawString(344, 875, "回復アイテム", 0x000000);
		DrawBox(344, 916, 376, 948, 0x0000ff, true);
		DrawFormatString(376, 948, 0x000000, "x%d", recoveryItem);
	}

	DrawFormatString(0, 0, 0xffffff, "お金 : %d", money);
}




//隠れる
bool Player::beHidden()
{
	if (hidden == true) {
		if (!push) {
			if (Pad::isTrigger(PAD_INPUT_4)) {
				push = true;
			}
		}
		else {
			if (Pad::isRelase(PAD_INPUT_4)) {
				push = false;
			}
		}
	}

	return push;
}



void Player::damege()
{
	hpDisplay = true;
	
	motionNum = 5;
	for (int time = 0; time < 120; time++) {
		motion->update(motionNum);
	}

	if (--time < 0) {
		if (playerHp > 0) {
			playerHp--;
			time = 20;
			hpDisplayTime = 120;
		}
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
	float spaceRight = pos.x + FieldData::chipSize * 3 + offset.x;
	float spaceBottom = pos.y + FieldData::chipSize * 3 + offset.y;

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
	float fieldLeft = x * FieldData::chipSize;
	float fieldRight = x * FieldData::chipSize + FieldData::chipSize;
	float fieldTop = y * FieldData::chipSize;
	float fieldBottom = y * FieldData::chipSize + FieldData::chipSize;

	if (playerPos.x + 15.0f < fieldLeft)				return false;
	if (playerPos.x + correctionSizeX > fieldRight)		return false;
	if (playerPos.y + correctionSizeY < fieldTop)		return false;
	if (playerPos.y > fieldBottom)						return false;

	return true;
}

bool Player::objectCollision(int x,int y)
{

	float objectLeft = x * FieldData::chipSize;
	float objectRight = x * FieldData::chipSize  + FieldData::chipSize ;
	float objectTop = y * FieldData::chipSize;
	float objectBottom = y * FieldData::chipSize + FieldData::chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;
		
	return true;
}

bool Player::ladderCollision(int x, int y)
{
	float objectLeft = x * FieldData::chipSize ;
	float objectRight = x * FieldData::chipSize + FieldData::chipSize;
	float objectTop = y * FieldData::chipSize;
	float objectBottom = y * FieldData::chipSize + FieldData::chipSize;

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
	float objectLeft = x * FieldData::chipSize;
	float objectRight = x * FieldData::chipSize + FieldData::chipSize;
	float objectTop = y * FieldData::chipSize;
	float objectBottom = y * FieldData::chipSize + FieldData::chipSize * 3;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;

	return true;
}






