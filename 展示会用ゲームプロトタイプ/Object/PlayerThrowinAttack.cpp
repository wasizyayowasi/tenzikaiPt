#include "PlayerThrowinAttack.h"
#include "../game.h"
#include "../Pad.h"
#include <math.h>
#include "../field.h"

PlayerThrowinAttack::PlayerThrowinAttack(int num) : sceneNum(num)
{
	if (num == 2) {
		isEnabled = true;
		flyingObjectPos = { 14900,800 };
	}
}

void PlayerThrowinAttack::attack(const Vec2& playerPos, bool directions)
{
	flyingObjectPos = playerPos;
	isEnabled = true;
	//方向
	playerDirections = directions;
}

void PlayerThrowinAttack::bossAttack(const Vec2& pos, bool directions)
{
	flyingObjectPos = pos;
	isEnabled = true;
	//方向
	playerDirections = directions;
	vec = {40.0f,-100.0f};
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
	if (!landingObject) {
		if (!playerDirections) {
			angle += 0.3f;
		}
		else {
			angle -= 0.3f;
		}
		
	}
	else {
		if (!playerDirections){
			angle = 0.8f;
		}
		else {
			angle = 5.4f;
		}
	}

	int underBlockX = static_cast<int>(flyingObjectPos.x) / chipSize;
	int underBlockY = static_cast<int>(flyingObjectPos.y + chipSize) / chipSize;

	if (sceneNum == 0) {
		chipNo = groundData::ground[underBlockY][underBlockX];
	}
	else {
		chipNo = groundData::tutorialGround[underBlockY][underBlockX];
	}


	if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {	
		landingObject = true;
	}
	
	if (!landingObject) {
		tempX = static_cast <float>(flyingObjectPos.y + r + cos(45));
		tempY = static_cast <float>(flyingObjectPos.y + r + sin(45));

		normalize.x = tempX;
		normalize.y = -tempY;

		vec = normalize.normalize() * 2;

		up += 0.3f;

		vec.y += up;

		vec.x *= 15;
	}

	//プレイヤーの向く方向によってobjectを飛ばす方向を変化させる
	if (!landingObject) {
		if (playerDirections) {
			flyingObjectPos.x -= vec.x;
			flyingObjectPos.y += vec.y;
		}
		else if (!playerDirections) {
			flyingObjectPos += vec;
		}
	}

	//範囲外で消える
	if (flyingObjectPos.x < -30) {
		init();
	}
	if (flyingObjectPos.x> Game::kScreenWidth * 2 - 40) {
		init();
	}
	
	
	if (flyingObjectPos.y < 0) {
		init();
	}
	if (flyingObjectPos.y > Game::kScreenHeight) {
		init();
	}

}

void PlayerThrowinAttack::bossUpdate(Vec2 offset)
{
	if (!landingObject) {
		if (!playerDirections) {
			angle += 0.3f;
		}
		else {
			angle -= 0.3f;
		}

	}
	else {
		if (!playerDirections) {
			angle = 0.8f;
		}
		else {
			angle = 5.4f;
		}
	}

	int underBlockX = static_cast<int>(flyingObjectPos.x) / chipSize;
	int underBlockY = static_cast<int>(flyingObjectPos.y + chipSize) / chipSize;

	const int chipNo = groundData::bossGround2[underBlockY][underBlockX];

	if (flyingObjectPos.y > 800) {
		flyingObjectPos.y = 800;
		landingObject = true;
		vec = { 0.0f,0.0f };
	}


	if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
		landingObject = true;
		vec.y = 0.0f;
	}

	//プレイヤーの向く方向によってobjectを飛ばす方向を変化させる
	if (!landingObject) {
		if (playerDirections) {
			flyingObjectPos.x -= vec.x;
			flyingObjectPos.y += vec.y;
		}
		else if (!playerDirections) {
			flyingObjectPos += vec;
		}
	}

	if (!landingObject) {

		vec.y += 10.0f;

	}

}

void PlayerThrowinAttack::draw(int handle, Vec2 offset)
{
	DrawRotaGraphF(flyingObjectPos.x + offset.x, flyingObjectPos.y + 10,1.5f, angle,handle, true, playerDirections);
}

void PlayerThrowinAttack::bossDraw(int handle, Vec2 offset)
{
	DrawRotaGraphF(flyingObjectPos.x + offset.x, flyingObjectPos.y - 100, 10.0f, angle, handle, true, playerDirections);
}


//プレイヤーとの当たり判定
bool PlayerThrowinAttack::playerCollision(const Vec2& pos, Vec2 offset)
{
	float playerLeft = pos.x + offset.x;
	float playerTop = pos.y + offset.y;
	float playerRight = pos.x + 50 + offset.x;
	float playerBottom = pos.y + 74 + offset.y;

	if (playerLeft > flyingObjectPos.x + 20 + offset.x)	return false;
	if (playerRight < flyingObjectPos.x + offset.x)		return false;
	if (playerTop >= flyingObjectPos.y + 20)	return false;
	if (playerBottom <= flyingObjectPos.y)		return false;

	return true;
}


//投擲物との当たり判定
bool PlayerThrowinAttack::enemyCollision(const Vec2& pos, Vec2 offset)
{
	
	if (isEnabled) {
		if (!landingObject) {
			
			float enemyLeft = pos.x ;
			float enemyTop = pos.y - 10;
			float enemyRight = pos.x + 30;
			float enemyBottom = pos.y + 30;

			DrawBoxAA(enemyLeft, enemyTop, enemyRight, enemyBottom, 0xffffff, false);

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

bool PlayerThrowinAttack::bossEnemyCollision(const Vec2& pos, Vec2 offset)
{
	if (isEnabled) {
		if (!landingObject) {

			float enemyRight = pos.x + 400;

			if (enemyRight < flyingObjectPos.x)			return false;

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

bool PlayerThrowinAttack::filedCollision(int x,int y,Vec2 offset)
{
	
	float flyingObjectLeft = flyingObjectPos.x;
	float flyingObjectRight = flyingObjectPos.x + 24;
	float flyingObjectTop = flyingObjectPos.y + offset.y;
	float flyingObjectBottom = flyingObjectPos.y + 24 + offset.y;

	int fieldLeft = x * chipSize;
	int fieldRight = x * chipSize + chipSize;
	int fieldTop = y * chipSize ;
	int fieldBottom = y * chipSize + chipSize;

	

	if (flyingObjectRight < fieldLeft)		return false;
	if (flyingObjectLeft > fieldRight)		return false;
	if (flyingObjectTop > fieldBottom)		return false;
	if (flyingObjectBottom > fieldTop)		return false;

	return true;
}

