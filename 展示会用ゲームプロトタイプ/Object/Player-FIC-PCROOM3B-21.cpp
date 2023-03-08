#include "Player.h"
#include "../Pad.h"
#include "DxLib.h"
#include "PlayerThrowinAttack.h"
#include "ObjectHp.h"
#include "../field.h"
#include "PlayerMotion.h"
#include "../InputState.h"



namespace {
	constexpr float correctionSizeX = 14.0f;
	constexpr float correctionSizeY = 74.0f;
}

Player::Player()
{
	hp = new ObjectHp;
	motion = new PlayerMotion;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
	updateFunc = &Player::updateField;
}

void Player::update(Vec2 offset, const InputState& input)
{

	(this->*updateFunc)(offset,input);

	if (playerHp < 1) {
		motionNum = 4;
		isEnabled = false;
		updateFunc = &Player::updateDeath;
	}
	//playerPos = playerPos + offset;
}

void Player::updateField(Vec2 offset, const InputState& input)
{

	collision = false;

	motionNum = 0;
	motion->update(motionNum);

	hp->setObjectHp(playerHp);

	int underfootChipNoX = (playerPos.x + correctionSizeX) / FieldData::chipSize;
	int underfootChipNoY = (playerPos.y + FieldData::chipSize * 3) / FieldData::chipSize;

	DrawFormatString(0, 30, 0xffffff, "%d:%d", underfootChipNoX, underfootChipNoY);

	int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX];
	//’òŽq
	if (chipNo == 2) {
		if (input.isPressed(InputType::down)) {
			playerPos.y += 3.0f;
		}
		if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
			DrawString(0, 60, "aiu", 0xffffff);
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
					DrawString(0, 60, "aiu", 0xffffff);
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	DrawBox(underfootChipNoX * FieldData::chipSize, underfootChipNoY * FieldData::chipSize, underfootChipNoX * FieldData::chipSize + FieldData::chipSize, underfootChipNoY * FieldData::chipSize + FieldData::chipSize, 0xff0000, true);

	//~‰º
	int DescentChipNo3 = FieldData::field[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//ˆÚ“®
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

	//HP‚Ì•\Ž¦
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}

	//C•œƒ^ƒCƒ}[
	if (!flyingObject->isEnable()) {
		if (Pad::isPress(PAD_INPUT_1)) {
			spaceHpDisplay = true;
		}
		else {
			spaceHpDisplay = false;
		}
	}

	//‹ßÚUŒ‚
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

	//“Š±
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
			if (flyingObject->playerCollision(playerPos,offset)) {
				flyingObject->deadFlyingObject();
			}
		}
	}


	int objectChipNo = FieldData::field[underfootChipNoY - 1][underfootChipNoX];
	//’òŽq
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

void Player::updateDescent(Vec2 offset, const InputState& input)
{

	int underfootChipNoX = (playerPos.x + FieldData::chipSize) / FieldData::chipSize;
	int underfootChipNoY = (playerPos.y + FieldData::chipSize * 3) / FieldData::chipSize;

	playerPos += vec;

	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {

			const int chipNo = FieldData::field[y][x];

			//’n–Ê
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
		//’n–Ê
		if (chipNo == 2) {
			if (objectCollision(underfootChipNoX + x, underfootChipNoY)) {
				DrawString(0, 60, "aiu", 0xffffff);
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

	bool noLeftEntry = false;
	bool noRightEntry = false;

	DrawFormatString(0, 30, 0xffffff, "%d:%d", underfootChipNoX, underfootChipNoY);

	for (int y = -2; y < 1; y++) {
		int chipNo9 = FieldData::field[underfootChipNoY + y][underfootChipNoX + 1];
		if (chipNo9 == 1) {
			noLeftEntry = true;
		}
	}

	for (int y = -2; y < 1; y++) {
		int chipNo10 = FieldData::field[underfootChipNoY + y][underfootChipNoX];
		if (chipNo10 == 1) {
			noRightEntry = true;
		}
	}

	//ˆÚ“®
	if (!noRightEntry) {
		if (input.isPressed(InputType::left)) {
			motion->update(motionNum);
			playerDirections = true;
		}
	}
	if (!noLeftEntry) {
		if (input.isPressed(InputType::right)) {
			motion->update(motionNum);
			playerPos.x += 5;
			playerDirections = false;
		}
	}


	if (input.isPressed(InputType::up)) {

		motionNum = 2;
		playerPos.y -= 5;
		motion->update(motionNum);
		

		int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX];
		//’n–Ê
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
		//’n–Ê
		if (chipNo == 1) {
			if (playerFiledCollision(underfootChipNoX, underfootChipNoY + 1)) {
				motionNum = 0;
				updateFunc = &Player::updateField;
				return;
			}
		}
	}

	
	//—Ž‰º
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

void Player::playerMotionUpdate(int num,bool directions)
{
	playerDirections = directions;
	motion->update(num);
}

//•`‰æ
void Player::draw(int handle, Vec2 offset)
{
	//ƒfƒoƒbƒO—p
	{
		/*if (proximityAttack) {
			if (playerDirections) {
				DrawBox(playerPos.x - 50, playerPos.y + 10, playerPos.x  + 20, playerPos.y + 50, 0xff00ff, true);
				DrawString(playerPos.x - 65, playerPos.y + 10, "‹ßÚUŒ‚", 0x000000);
			}
			else {
				DrawBox(playerPos.x + 20, playerPos.y + 10, playerPos.x + 80, playerPos.y + 50, 0xff00ff, true);
				DrawString(playerPos.x + 55, playerPos.y + 10, "‹ßÚUŒ‚", 0x000000);
			}
		}*/

		/*if (updateFunc == &Player::updateField) {
			int aiuX = (playerPos.x + 14) / Field::chipSize;
			int aiuY = (playerPos.y + Field::chipSize * 3) / Field::chipSize;
			int color = 0xff0000;
			DrawString(0, 150, "ƒtƒB[ƒ‹ƒh", 0xffffff);
			DrawBox(aiuX * Field::chipSize, aiuY * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, aiuY * Field::chipSize + Field::chipSize, 0xff0000, true);
			if (Field::field[aiuY - 1][aiuX] == 2) {
				color = 0x00ff00;
			}
			DrawBox(aiuX * Field::chipSize, (aiuY - 1) * Field::chipSize, aiuX * Field::chipSize + Field::chipSize, (aiuY - 1) * Field::chipSize + Field::chipSize, color, true);
		}
		else if (updateFunc == &Player::updateLadder) {
			int aiuX = (playerPos.x + 14) / Field::chipSize;
			int aiuY = (playerPos.y + Field::chipSize * 2) / Field::chipSize;
			DrawString(0, 150, "’òŽq", 0xffffff);
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
			DrawString(0, 150, "~‰º", 0xffffff);
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

	//”ò‚Ñ“¹‹ï
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	if (hpDisplay) {
		hp->draw(playerPos,offset);
	}

	motion->draw(playerPos, handle, playerDirections, offset);

}




//‰B‚ê‚é
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


//‹ßÚUŒ‚‚Ì“–‚½‚è”»’è
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

//‹óŠÔ‚ðC•œ‚·‚é
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


	DrawBox(spaceLeft, spaceTop, spaceRight, spaceBottom, 0xff0000, false);

	if (playerRight < spaceLeft)				return false;
	DrawString(500, 15, "aiu1", 0xffffff);
	if (playerLeft > spaceRight)				return false;
	DrawString(500, 30, "aiu2", 0xffffff);
	if (playerBottom < spaceTop)				return false;
	DrawString(500, 45, "aiu3", 0xffffff);
	if (playerTop > spaceBottom)				return false;
	DrawString(500, 60, "aiu4", 0xffffff);
		
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




