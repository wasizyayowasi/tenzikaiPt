#include "Player.h"
#include "../Pad.h"
#include "TimeLimit.h"
#include "DxLib.h"
#include "TimeLimit.h"
#include "../PlayerThrowinAttack.h"

Player::Player()
{
	limit = new TimeLimit;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
}

void Player::update()
{
	Pad::update();

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

	DrawFormatString(0, 220, GetColor(255, 255, 255), "%d", aiu);
}

//‰B‚ê‚é
bool Player::beHidden()
{

	if (Pad::isTrigger(PAD_INPUT_4)) {
		aiu++;
	}

	if (hidden) {
		DrawString(0, 220, "aiu", GetColor(255, 255, 255));
		if (!push) {
			DrawString(0, 240, "kakiku", GetColor(255, 255, 255));
			if (Pad::isPress(PAD_INPUT_4)) {
				DrawString(0, 200, "sasisu", GetColor(255, 255, 255));
				push = true;
			}
		}

	}
	return push;
}

