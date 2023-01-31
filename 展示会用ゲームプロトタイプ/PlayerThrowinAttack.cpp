#include "PlayerThrowinAttack.h"
#include "game.h"
#include "Pad.h"
#include <math.h>

PlayerThrowinAttack::PlayerThrowinAttack()
{
}

void PlayerThrowinAttack::attack(const Vec2& playerPos, int directions)
{
	pos = playerPos;
	isEnabled = true;
	//方向
	playerDirections = directions;
}

bool PlayerThrowinAttack::isEnable() const
{
	return isEnabled;
}

void PlayerThrowinAttack::init()
{
	landingObject = false;
	isEnabled = false;
	vec = { 5.0f,0.0f };
	up = 0;
}

void PlayerThrowinAttack::update()
{
	flyingObjectPos = pos;

	//飛翔物が地面に着地したときor飛翔中の時
	if (pos.y >= 680) {
		vec = { 0.0f,0.0f };
		landingObject = true;
		pos.y = 680;
	}
	else {
		x = pos.y + r + cos(45);
		y = pos.y + r + sin(45);

		normalize.x = x;
		normalize.y = -y;

		vec = normalize.normalize() * 2;

		up += 0.3f;

		vec.y += up;

		vec.x *= 15;
	}

	//プレイヤーの向く方向によってobjectを飛ばす方向を変化させる
	if (playerDirections == 1) {
		pos.x -= vec.x;
		pos.y += vec.y;
	}
	else if (playerDirections == 2) {
		pos += vec;
	}

	//範囲外で消える
	if (pos.x < 0) {
		init();
	}
	if (pos.x - 20 > Game::kScreenWidth) {
		init();
	}
	if (pos.y < 0) {
		init();
	}
	if (pos.y > Game::kScreenHeight) {
		init();
	}

}

void PlayerThrowinAttack::draw()
{
	DrawBox(flyingObjectPos.x, flyingObjectPos.y, flyingObjectPos.x + 20, flyingObjectPos.y + 20, GetColor(252, 15, 192), true);
	DrawFormatString(0, 30, GetColor(255, 255, 255), "%d", playerDirections);
}


//プレイヤーとの当たり判定
bool PlayerThrowinAttack::playerCollision(const Vec2& pos)
{
	float playerLeft = pos.x;
	float playerTop = pos.y;
	float playerRight = pos.x + 50;
	float playerBottom = pos.y + 64;

	if (playerLeft > flyingObjectPos.x + 20)	return false;
	if (playerRight < flyingObjectPos.x)		return false;
	if (playerTop >= flyingObjectPos.y + 20)	return false;
	if (playerBottom <= flyingObjectPos.y)		return false;

	return true;
}


//投擲物との当たり判定
bool PlayerThrowinAttack::enemyCollision(const Vec2& pos)
{
	if (isEnabled) {
		float enemyLeft = pos.x;
		float enemyTop = pos.y;
		float enemyRight = pos.x + 30;
		float enemyBottom = pos.y + 30;

		if (enemyLeft > flyingObjectPos.x + 20)		return false;
		if (enemyRight < flyingObjectPos.x)			return false;
		if (enemyTop >= flyingObjectPos.y + 20)		return false;
		if (enemyBottom <= flyingObjectPos.y)		return false;

		return true;
	}
	else {
		return false;
	}
	
}


void PlayerThrowinAttack::deadFlyingObject()
{
	init();
}
