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
	PlayerPos += vec;
	if (PlayerPos.y + 64 > 700) {
		PlayerPos.y = 700-64;
		hit = true;
	}
	else {
		hit = false;
	}

	//ˆÚ“®
	if (Pad::isPress(PAD_INPUT_LEFT)) {
		PlayerPos.x -= 10;
		playerDirections = 1;
	}
	if (Pad::isPress(PAD_INPUT_RIGHT)) {
		PlayerPos.x += 10;
		playerDirections = 2;
	}
	if (Pad::isPress(PAD_INPUT_UP)) {
		PlayerPos.y -= 10;
	}
	if (!hit) {
		if (Pad::isPress(PAD_INPUT_DOWN)) {
			PlayerPos.y += 10;
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
		proximityAttack = true;
	}
	else {
		proximityAttack = false;
	}

	//“Š±
	if (Pad::isTrigger(PAD_INPUT_3)) {
		if (!flyingObject->isEnable()) {
			flyingObject->attack(PlayerPos,playerDirections);
		}
	}
	if (flyingObject->isEnable()) {
		flyingObject->update();
	}
	if (flyingObject->landing()) {
		if (flyingObject->playerCollision(PlayerPos)) {
			flyingObject->deadFlyingObject();
		}
	}
	
	//HP‚Ì•\Ž¦
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}
	
	limit->playerGetPos(PlayerPos);

}

//•`‰æ
void Player::draw()
{
	//‹ßÚUŒ‚
	if (proximityAttack) {
		if (playerDirections == 1) {
			DrawBox(PlayerPos.x - 5, PlayerPos.y + 10, PlayerPos.x - 65, PlayerPos.y + 50, 0xff00ff, true);
		}
		else {
			DrawBox(PlayerPos.x + 55, PlayerPos.y + 10, PlayerPos.x + 115, PlayerPos.y + 50, 0xff00ff, true);
		}
	}

	DrawFormatString(0, 45, GetColor(255, 255, 255), "%d", hpDisplayTime);

	DrawBox(PlayerPos.x, PlayerPos.y, PlayerPos.x + 50, PlayerPos.y + 64, GetColor(255, 255, 255), true);
	
	if (playerDirections == 1) {
		DrawBox(PlayerPos.x - 20, PlayerPos.y, PlayerPos.x, PlayerPos.y + 10, GetColor(255, 0, 0), true);
	}
	else if (playerDirections == 2) {
		DrawBox(PlayerPos.x + 50, PlayerPos.y, PlayerPos.x + 70, PlayerPos.y + 10, GetColor(255, 0, 0), true);
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

	if (hpDisplay) {
		hp->draw(PlayerPos);
	}

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

bool Player::proximityAttackCollision(const Vec2& pos)
{
	float enemyLeft = pos.x;
	float enemyTop = pos.y;
	float enemyRight = pos.x + 30;
	float enemyBottom = pos.y + 30;

	if (proximityAttack) {
		if (playerDirections == 1) {
			if (enemyLeft > PlayerPos.x - 5)			return false;
			DrawString(600, 0, "aiu1", 0xffffff);
			if (enemyRight < PlayerPos.x - 65)			return false;
			DrawString(600, 15, "aiu1", 0xffffff);
			if (enemyTop <= PlayerPos.y + 10)			return false;
			DrawString(600, 30, "aiu1", 0xffffff);
			if (enemyBottom <= PlayerPos.y + 50)		return false;
			DrawString(600, 45, "aiu1", 0xffffff);
		}
		else if (playerDirections == 2) {
			if (enemyLeft > PlayerPos.x + 115)			return false;
			DrawString(600, 0, "aiu1", 0xffffff);
			if (enemyRight < PlayerPos.x + 55)			return false;
			DrawString(600, 15, "aiu1", 0xffffff);
			if (enemyTop <= PlayerPos.y + 10)			return false;
			DrawString(600, 30, "aiu1", 0xffffff);
			if (enemyBottom <= PlayerPos.y + 50)		return false;
			DrawString(600, 45, "aiu1", 0xffffff);
		}
		return true;
	}
	return false;
}
