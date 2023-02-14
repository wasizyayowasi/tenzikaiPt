#include "Player.h"
#include "../Pad.h"
#include "DxLib.h"
#include "PlayerThrowinAttack.h"
#include "ObjectHp.h"
#include "../field.h"
#include "../PlayerMotion.h"

namespace {
	constexpr float correctionSizeX = 14.0f;
	constexpr float correctionSizeY = 74.0f;

	constexpr int groundY = 700;
}

Player::Player()
{
	hp = new ObjectHp;
	motion = new PlayerMotion;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
	updateFunc = &Player::updateField;
}

void Player::update()
{
	(this->*updateFunc)();

	if (playerHp < 1) {
		motionNum = 4;
		isEnabled = false;
		updateFunc = &Player::updateDeath;
	}

}

//描画
void Player::draw(int handle)
{
	//デバッグ用
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
	{
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

	DrawString(playerPos.x, playerPos.y - 30, "プレイヤー", 0xff00ff);
	DrawBox(playerPos.x+25, playerPos.y+32, playerPos.x + 27, playerPos.y + 34, 0x000000,true);

	//飛び道具
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	if (hpDisplay) {
		hp->draw(playerPos);
	}

	motion->draw(playerPos,handle, playerDirections);

}


void Player::updateField()
{
	collision = false;

	motionNum = 0;
	motion->update(motionNum);

	hp->setObjectHp(playerHp);

	int underfootChipNoX = (playerPos.x + correctionSizeX) / Field::chipSize;
	int underfootChipNoY = (playerPos.y + Field::chipSize * 3) / Field::chipSize;

	DrawFormatString(0, 30, 0xffffff, "%d:%d", underfootChipNoX, underfootChipNoY);

	int chipNo = Field::field[underfootChipNoY][underfootChipNoX];
	//梯子
	if (chipNo == 2) {
		if (Pad::isPress(PAD_INPUT_DOWN)) {
			playerPos.y += 3.0f;
		}
		if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
			DrawString(0, 60, "aiu", 0xffffff);
			updateFunc = &Player::updateLadder;
		}
	}

	for (int x = -1; x < 1; x++) {
		int chipNo = Field::field[underfootChipNoY - 1][underfootChipNoX - x];
		if (chipNo == 2) {
			if (Pad::isPress(PAD_INPUT_UP)) {
				if (!collision) {
					collision = ladderCollision(underfootChipNoX - x, underfootChipNoY - 1);
				}
				if (collision) {
					DrawString(0, 60, "aiu", 0xffffff);
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	DrawBox(underfootChipNoX * Field::chipSize, underfootChipNoY * Field::chipSize, underfootChipNoX * Field::chipSize + Field::chipSize, underfootChipNoY * Field::chipSize + Field::chipSize, 0xff0000, true);

	//降下
	int DescentChipNo3 = Field::field[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//移動
	if (move) {
		if (!push) {
			if (Pad::isPress(PAD_INPUT_LEFT)) {
				if (!(motionNum == 3)) {
					motionNum = 1;
				}
				motion->update(motionNum);
				playerPos.x -= 5;
				playerDirections = true;
			}
			else if (Pad::isPress(PAD_INPUT_RIGHT)) {
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

	//修復タイマー
	if (!flyingObject->isEnable()) {
		if (Pad::isPress(PAD_INPUT_1)) {
			spaceHpDisplay = true;
		}
		else {
			spaceHpDisplay = false;
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

	//投擲
	if (!push) {
		if (Pad::isTrigger(PAD_INPUT_3)) {
			if (!flyingObject->isEnable()) {
				flyingObject->attack(playerPos, playerDirections);
			}
		}
		if (flyingObject->isEnable()) {
			flyingObject->update();
		}
		if (flyingObject->landing()) {
			if (flyingObject->playerCollision(playerPos)) {
				flyingObject->deadFlyingObject();
			}
		}
	}


	int objectChipNo = Field::field[underfootChipNoY - 1][underfootChipNoX];
	//梯子
	if (objectChipNo == 3) {
		if (objectCollision(underfootChipNoX, underfootChipNoY - 1)) {
			DrawString(0, 60, "aiu", 0xffffff);
			hidden = true;
		}
	}
	else {
		hidden = false;
	}
}

void Player::updateDescent()
{

	int underfootChipNoX = (playerPos.x + Field::chipSize) / Field::chipSize;
	int underfootChipNoY = (playerPos.y + Field::chipSize * 3) / Field::chipSize;

	playerPos += vec;

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			//地面
			if (chipNo == 1) {
				if (playerFiledCollision(x, y)) {
					if (playerPos.y + correctionSizeY > y * Field::chipSize) {
						playerPos.y = y * Field::chipSize - 74;
					}
					updateFunc = &Player::updateField;
					return;
				}
			}
		}
	}

	for (int x = -1; x < 1; x++) {

		const int chipNo = Field::field[underfootChipNoY][underfootChipNoX + x];
		//地面
		if (chipNo == 2) {
			if (objectCollision(underfootChipNoX + x, underfootChipNoY)) {
				DrawString(0, 60, "aiu", 0xffffff);
				updateFunc = &Player::updateLadder;
				return;
			}
		}
	}

}

void Player::updateLadder()
{
	int underfootChipNoX = (playerPos.x + correctionSizeX) / Field::chipSize;
	int underfootChipNoY = (playerPos.y + Field::chipSize * 2) / Field::chipSize;

	bool noLeftEntry = false;
	bool noRightEntry = false;

	DrawFormatString(0, 30, 0xffffff, "%d:%d", underfootChipNoX, underfootChipNoY);

	for (int y = -2; y < 1; y++) {
		int chipNo9 = Field::field[underfootChipNoY + y][underfootChipNoX + 1];
		if (chipNo9 == 1) {
			noLeftEntry = true;
		}
	}

	for (int y = -2; y < 1; y++) {
		int chipNo10 = Field::field[underfootChipNoY + y][underfootChipNoX];
		if (chipNo10 == 1) {
			noRightEntry = true;
		}
	}

	//移動
	if (!noRightEntry) {
		if (Pad::isPress(PAD_INPUT_LEFT)) {
			motion->update(motionNum);
			playerPos.x -= 5;
			playerDirections = true;
		}
	}
	if (!noLeftEntry) {
		if (Pad::isPress(PAD_INPUT_RIGHT)) {
			motion->update(motionNum);
			playerPos.x += 5;
			playerDirections = false;
		}
	}


	if (Pad::isPress(PAD_INPUT_UP)) {

		motionNum = 2;
		playerPos.y -= 5;
		motion->update(motionNum);
		

		int chipNo = Field::field[underfootChipNoY][underfootChipNoX];
		//地面
		if (chipNo == 0) {
			if (playerFiledCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateField;
				return;
			}
		}
	}
	if (Pad::isPress(PAD_INPUT_DOWN)) {
		motionNum = 2;
		playerPos.y += 5;
		motion->update(motionNum);

		int chipNo = Field::field[underfootChipNoY + 1][underfootChipNoX];
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
	chipNo3 = Field::field[underfootChipNoY + 1][underfootChipNoX];
	if (chipNo3 == 0) {
		motionNum = 0;
		motion->update(motionNum);
		updateFunc = &Player::updateDescent;
	}
}

void Player::updateDeath()
{
	motion->update(motionNum);
	if (motion->setDead()) {
		isEnabled = false;
	}
}

void Player::playerMotionUpdate(int num,bool directions)
{
	playerDirections = directions;
	motion->update(num);
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
				DrawString(0, 75, "aiu", 0xffffff);
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

int Player::enemyAttack(Vec2 enemyPos)
{
	if (flyingObject->enemyCollision(enemyPos)) {
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
bool Player::repairSpace(const Vec2& pos)
{
	float spaceLeft = pos.x;
	float spaceTop = pos.y;
	float spaceRight = pos.x + Field::chipSize * 3;
	float spaceBottom = pos.y + Field::chipSize * 3;

	DrawBox(spaceLeft, spaceTop, spaceRight, spaceBottom, 0xff0000, false);

	if (playerPos.x + 15 < spaceLeft)				return false;
	if (playerPos.x + 14 > spaceRight)				return false;
	if (playerPos.y + 74 < spaceTop)				return false;
	if (playerPos.y > spaceBottom)					return false;
		
	return true;

}

bool Player::playerFiledCollision(int x,int y)
{
	float fieldLeft = x * Field::chipSize;
	float fieldRight = x * Field::chipSize + Field::chipSize;
	float fieldTop = y * Field::chipSize;
	float fieldBottom = y * Field::chipSize + Field::chipSize;

	if (playerPos.x + 15.0f < fieldLeft)				return false;
	if (playerPos.x + correctionSizeX > fieldRight)		return false;
	if (playerPos.y + correctionSizeY < fieldTop)		return false;
	if (playerPos.y > fieldBottom)						return false;

	return true;
}

bool Player::objectCollision(int x,int y)
{

	float objectLeft = x * Field::chipSize;
	float objectRight = x * Field::chipSize  + Field::chipSize ;
	float objectTop = y * Field::chipSize;
	float objectBottom = y * Field::chipSize + Field::chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;
		
	return true;
}

bool Player::ladderCollision(int x, int y)
{
	float objectLeft = x * Field::chipSize ;
	float objectRight = x * Field::chipSize + Field::chipSize;
	float objectTop = y * Field::chipSize;
	float objectBottom = y * Field::chipSize + Field::chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;

	return true;
}




