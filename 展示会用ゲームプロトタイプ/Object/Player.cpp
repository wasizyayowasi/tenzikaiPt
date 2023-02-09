#include "Player.h"
#include "../Pad.h"
#include "DxLib.h"
#include "PlayerThrowinAttack.h"
#include "ObjectHp.h"
#include "../field.h"
#include "../PlayerMotion.h"

namespace {
	constexpr int playerSizeX = 115;
	constexpr int playerSizeY = 74;

	constexpr int groundY = 700;
}

Player::Player()
{
	hp = new ObjectHp;
	motion = new PlayerMotion;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
}

void Player::update()
{
	ladder = false;
	hidden = false;
	if (motionNum != 3) {
		hit = false;
	}

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 1) {
				if (playerFiledCollision(x,y)) {
					hit = true;
				}
			}
			else if (chipNo == 2) {
				if (objectCollision(x, y)) {
					ladder = true;
				}
			}
			else if (chipNo == 3) {
				if (objectCollision(x, y)) {
					hidden = true;
				}
			}
		}
	}

	//–ˆƒtƒŒ[ƒ€AƒvƒŒƒCƒ„[‚ÌHP‚ðŠm”F‚·‚é
	hp->setObjectHp(playerHp);

	if (hit) {
		if (--proximityAttackTime < 0) {
			motionNum = 0;
			proximityAttackTime = 0;
		}
	}

	if (hit) {
		DrawString(0, 60, "aiu", 0xffffff);
		//upperLimit = true;
	}

	//’òŽq‚ð“o‚èI‚í‚Á‚½‚Æ‚«A‚³‚ç‚É“o‚ë‚¤‚Æ‚µ‚È‚¢‚æ‚¤‚É‚·‚é‚½‚ß‚Ì‚â‚Â


	//‹ßÚUŒ‚
	if (!push) {
		if (hit) {
			if (!flyingObject->isEnable()) {
				if (Pad::isPress(PAD_INPUT_2)) {
					proximityAttack = true;
					motionNum = 3;
					motion->update(motionNum);
					proximityAttackTime = 10;
				}
				else {
					proximityAttack = false;
				}
			}
		}
	}



	//ˆÚ“®
	if (!push) {
		if (motionNum != 2) {
			if (Pad::isPress(PAD_INPUT_LEFT)) {
				if (!(motionNum == 3)) {
					motionNum = 1;
				}
				playerPos.x -= 5;
				playerDirections = true;
				motion->update(motionNum);
			}
			else if (Pad::isPress(PAD_INPUT_RIGHT)) {
				if (!(motionNum == 3)) {
					motionNum = 1;
				}
				playerPos.x += 5;
				playerDirections = false;
				motion->update(motionNum);
			}
		}
		if (ladder) {
			if (Pad::isPress(PAD_INPUT_UP)) {
				motionNum = 2;
				playerPos.y -= 5;
				motion->update(motionNum);
			}
		}
		if (!hit) {
			if (Pad::isPress(PAD_INPUT_DOWN)) {
				motionNum = 2;
				playerPos.y += 5;
				motion->update(motionNum);
			}
		}
	}

	
	//d—Í‚ð‘«‚µ‘±‚¯‚é
	if (!ladder) {
		vec.y = 8.0f;
		if (!hit) {
			playerPos += vec;
		}
	}


	//C•œƒ^ƒCƒ}[
	if (!flyingObject->isEnable()) {
		if (repair == 1) {
			if (Pad::isPress(PAD_INPUT_1)) {
				spaceHpDisplay = true;
			}
			else {
				spaceHpDisplay = false;
			}
		}
	}


	//ˆÈ‰ºUŒ‚Žè’iŽÀ‘•—\’è 
	//“Š±
	if (!push) {
		if (hit) {
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
	}


	

	
	//HP‚Ì•\Ž¦
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}

	repair = 0;
	if (motionNum == 3) {
	}
	else if(!Pad::pressButton()) {
		if (hit||upperLimit) {
			motionNum = 0;
		}
	}

	if (motionNum == 0) {
		motion->update(motionNum);
	}

	
}

//•`‰æ
void Player::draw(int handle)
{
	//‹ßÚUŒ‚
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
	

	DrawString(playerPos.x, playerPos.y - 30, "ƒvƒŒƒCƒ„[", 0xff00ff);
	DrawBox(playerPos.x+25, playerPos.y+32, playerPos.x + 27, playerPos.y + 34, 0x000000,true);
	DrawFormatString(0, 75, 0xffffff, "%d", proximityAttackTime);

	//”ò‚Ñ“¹‹ï
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	if (hpDisplay) {
		hp->draw(playerPos);
	}

	motion->draw(playerPos,handle, playerDirections);
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
			if (Pad::isTrigger(PAD_INPUT_4)) {
				push = false;
			}
		}
	}

	return push;
}

void Player::damege()
{
	hpDisplay = true;

	if (--time < 0) {
		if (playerHp > 0) {
			playerHp--;
			time = 60;
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
bool Player::repairSpace(const Vec2& pos)
{
	float spaceLeft = pos.x;
	float spaceTop = pos.y;
	float spaceRight = pos.x + Field::chipSize * 3;
	float spaceBottom = pos.y + Field::chipSize * 3;

	DrawBox(spaceLeft, spaceTop, spaceRight, spaceBottom, 0xff0000, false);

	if (playerPos.x + 25 < spaceLeft)				return false;
	if (playerPos.x > spaceRight)					return false;
	if (playerPos.y + 74 < spaceTop)				return false;
	if (playerPos.y > spaceBottom)					return false;
		
	return true;

}

void Player::setRepair(int num)
{
	
	repair = num;
	if (repair) {
		DrawString(0, 0, "’¼‚¹‚é‚æ", 0xffffff);
	}
	else {
		DrawString(0, 15, "’¼‚¹‚È‚¢‚æ", 0xffffff);
	}
}

bool Player::playerFiledCollision(int x,int y)
{
	float filedLeft = x * Field::chipSize;
	float filedRight = x * Field::chipSize + Field::chipSize;
	float filedTop = y * Field::chipSize;
	float filedBottom = y * Field::chipSize + Field::chipSize;

	if (playerPos.x + 25 < filedLeft)	return false;
	if (playerPos.x > filedRight)		return false;
	if (playerPos.y + 74 < filedTop)	return false;
	if (playerPos.y > filedBottom)		return false;

	return true;
}

bool Player::objectCollision(int x,int y)
{
	
	float objectLeft = x * Field::chipSize;
	float objectRight = x * Field::chipSize  + Field::chipSize;
	float objectTop = y * Field::chipSize;
	float objectBottom = y * Field::chipSize + Field::chipSize;

	if (playerPos.x + 26 < objectLeft)		return false;
	if (playerPos.x + 25 > objectRight)		return false;
	if (playerPos.y + 74 < objectTop)		return false;
	if (playerPos.y > objectBottom)			return false;

	return true;
}
