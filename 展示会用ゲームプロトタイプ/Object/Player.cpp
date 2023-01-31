#include "Player.h"
#include "../Pad.h"
#include "TimeLimit.h"
#include "DxLib.h"
#include "TimeLimit.h"
#include "../PlayerThrowinAttack.h"
#include "../ObjectHp.h"

Player::Player()
{
	limit = new TimeLimit;
	hp = new ObjectHp;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
}

void Player::update()
{
	hp->setObjectHp(playerHp);

	//’n–Ê‚Æ‚Ì”»’è
	pos += vec;
	if (pos.y + 64 > 700) {
		pos.y = 700-64;
		hit = true;
	}
	else {
		hit = false;
	}

	//ˆÚ“®
	if (Pad::isPress(PAD_INPUT_LEFT)) {
		pos.x -= 10;
		playerDirections = 1;
	}
	if (Pad::isPress(PAD_INPUT_RIGHT)) {
		pos.x += 10;
		playerDirections = 2;
	}
	if (Pad::isPress(PAD_INPUT_UP)) {
		pos.y -= 10;
	}
	if (!hit) {
		if (Pad::isPress(PAD_INPUT_DOWN)) {
			pos.y += 10;
		}
	}

	//C•œƒ^ƒCƒ}[
	if (!flyingObject->isEnable()) {
		if (Pad::isPress(PAD_INPUT_1)) {
			limit->update();
			timeDisplay = true;
		}
		else {
			timeDisplay = false;
		}
	}

	//ˆÈ‰ºUŒ‚Žè’iŽÀ‘•—\’è
	//‹ßÚUŒ‚
	if (Pad::isTrigger(PAD_INPUT_2)) {

	}

	//“Š±
	if (Pad::isTrigger(PAD_INPUT_3)) {
		if (!flyingObject->isEnable()) {
			flyingObject->attack(pos,playerDirections);
		}
	}
	if (flyingObject->isEnable()) {
		flyingObject->update();
	}
	if (flyingObject->landing()) {
		if (flyingObject->playerCollision(pos)) {
			flyingObject->deadFlyingObject();
		}
	}
	
	if (Pad::isTrigger(PAD_INPUT_6)) {
		playerHp--;
	}
	
	limit->playerGetPos(pos);

}

//•`‰æ
void Player::draw()
{
	DrawFormatString(0, 45, GetColor(255, 255, 255), "%d", playerDirections);

	DrawBox(pos.x, pos.y, pos.x + 50, pos.y + 64, GetColor(255, 255, 255), true);
	
	if (playerDirections == 1) {
		DrawBox(pos.x - 20, pos.y, pos.x, pos.y + 10, GetColor(255, 0, 0), true);
	}
	else if (playerDirections == 2) {
		DrawBox(pos.x + 50, pos.y, pos.x + 70, pos.y + 10, GetColor(255, 0, 0), true);
	}
	
	//ƒ^ƒCƒ}[
	if (!flyingObject->isEnable()) {
		if (timeDisplay) {
			limit->draw();
		}
	}

	//”ò‚Ñ“¹‹ï
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	hp->draw(pos);

	DrawFormatString(0, 220, GetColor(255, 255, 255), "aiu : %d", aiu);
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

//bool Player::enemyCollision(Vec2 enemyPos)
//{
//	
//
//	float enemyLeft = enemyPos.x;
//	float enemyTop = enemyPos.y;
//	float enemyRight = enemyPos.x + 50;
//	float enemyBottom = enemyPos.y + 64;
//
//	if (enemyLeft > pos.x)				return false;
//	DrawString(400, 0, "aiu", GetColor(255, 255, 255));
//	if (enemyRight < pos.x + 50)		return false;
//	DrawString(400, 15, "aiu2", GetColor(255, 255, 255));
//	if (enemyTop <= pos.y)				return false;
//	DrawString(400, 30, "aiu3", GetColor(255, 255, 255));
//	if (enemyBottom <= pos.y + 64)		return false;
//	DrawString(400, 45, "aiu4", GetColor(255, 255, 255));
//
//	return false;
//}

void Player::damege()
{
	if (--time < 0) {
		if (playerHp > 0) {
			playerHp--;
			time = 60;
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

