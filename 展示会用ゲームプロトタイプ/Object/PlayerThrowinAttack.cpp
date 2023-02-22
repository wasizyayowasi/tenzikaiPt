#include "PlayerThrowinAttack.h"
#include "../game.h"
#include "../Pad.h"
#include <math.h>
#include "../field.h"

PlayerThrowinAttack::PlayerThrowinAttack()
{
}

void PlayerThrowinAttack::attack(const Vec2& playerPos, bool directions)
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

void PlayerThrowinAttack::update(Vec2 offset)
{
	flyingObjectPos = pos + offset;
	if (!landingObject) {
		angle += 0.3f;
	}
	else {
		if (!playerDirections){
			angle = 0.8f;
		}
		else {
			angle = 5.4f;
		}
	}
	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {

			const int chipNo = FieldData::field[y][x];

			if (chipNo == 1) {
				if (filedCollision(x,y)) {
					landingObject = true;
				}
			}
		}
	}
	
	if (!landingObject) {
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
	if (!landingObject) {
		if (playerDirections) {
			pos.x -= vec.x;
			pos.y += vec.y;
		}
		else if (!playerDirections) {
			pos += vec;
		}
	}

	//範囲外で消える
	if (pos.x < -30) {
		init();
	}
	if (pos.x - 20 > Game::kScreenWidth * 2) {
		init();
	}
	if (pos.y < 0) {
		init();
	}
	if (pos.y > Game::kScreenHeight) {
		init();
	}

}

void PlayerThrowinAttack::draw(int handle)
{
	//DrawBox(flyingObjectPos.x, flyingObjectPos.y, flyingObjectPos.x + 20, flyingObjectPos.y + 20, GetColor(252, 15, 192), true);
	DrawString(flyingObjectPos.x, flyingObjectPos.y - 10, "弾", 0xffffff);
	DrawRotaGraph(flyingObjectPos.x, flyingObjectPos.y,1.5f, angle,handle, true, playerDirections);
}


//プレイヤーとの当たり判定
bool PlayerThrowinAttack::playerCollision(const Vec2& pos, Vec2 offset)
{
	float playerLeft = pos.x + offset.x;
	float playerTop = pos.y + offset.y;
	float playerRight = pos.x + 50 + offset.x;
	float playerBottom = pos.y + 74 + offset.y;

	if (playerLeft > flyingObjectPos.x + 20)	return false;
	if (playerRight < flyingObjectPos.x)		return false;
	if (playerTop >= flyingObjectPos.y + 20)	return false;
	if (playerBottom <= flyingObjectPos.y)		return false;

	return true;
}


//投擲物との当たり判定
bool PlayerThrowinAttack::enemyCollision(const Vec2& pos, Vec2 offset)
{
	
	if (isEnabled) {
		if (!landingObject) {
			float enemyLeft = pos.x + offset.x;
			float enemyTop = pos.y + offset.y;
			float enemyRight = pos.x + 30 + offset.x;
			float enemyBottom = pos.y + 30 + offset.y;

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
	else {
		return false;
	}

}


void PlayerThrowinAttack::deadFlyingObject()
{
	init();
}

bool PlayerThrowinAttack::filedCollision(int x,int y)
{
	
	float flyingObjectLeft = flyingObjectPos.x;
	float flyingObjectRight = flyingObjectPos.x + 24;
	float flyingObjectTop = flyingObjectPos.y;
	float flyingObjectBottom = flyingObjectPos.y + 24;

	float fieldLeft = x * FieldData::chipSize;
	float fieldRight = x * FieldData::chipSize + FieldData::chipSize;
	float fieldTop = y * FieldData::chipSize;
	float fieldBottom = y * FieldData::chipSize + FieldData::chipSize;

	if (flyingObjectBottom < fieldTop)		return false;
	if (flyingObjectTop > fieldBottom)		return false;
	if (flyingObjectRight < fieldLeft)		return false;
	if (flyingObjectLeft > fieldRight)		return false;

	return true;
}