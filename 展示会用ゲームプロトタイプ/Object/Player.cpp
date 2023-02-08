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

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 1) {
				if (playerFiledCollision(y)) {
					hit = true;
				}
			}
		}
	}

	//毎フレーム、プレイヤーのHPを確認する
	hp->setObjectHp(playerHp);

	if (!ladder) {
		if (--proximityAttackTime < 0) {
			motionNum = 0;
		}
	}

	//重力を足し続ける
	if (!ladder) {
		if (upperLimit) {
			vec.y = 20.0f;
		}
		else {
			vec.y = 10.0f;
		}
		if (!hit) {
			playerPos += vec;
		}
	}

	//梯子を登り終わったとき、さらに登ろうとしないようにするためのやつ
	if (Pad::isPress(PAD_INPUT_UP)) {
		if (!ladder) {
			upperLimit = true;
		}
	}

	//移動
	if (!push) {
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
		if (ladder) {
			if (!upperLimit) {
				if (Pad::isPress(PAD_INPUT_UP)) {
					motionNum = 2;
					playerPos.y -= 5;
					motion->update(motionNum);
				}
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


	//地面との判定
	/*if (playerPos.y + playerSizeY > groundY) {
		playerPos.y = groundY - playerSizeY;
		hit = true;
		upperLimit = false;
	}*/
	
	if (playerPos.y + playerSizeY < groundY) {
		hit = false;
	}


	//修復タイマー
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


	//以下攻撃手段実装予定 
	//投擲
	if (Pad::isTrigger(PAD_INPUT_3)) {
		if (!flyingObject->isEnable()) {
			flyingObject->attack(playerPos,playerDirections);
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


	//近接攻撃
	if (!flyingObject->isEnable()) {
		if (Pad::isTrigger(PAD_INPUT_2)) {
			proximityAttack = true;
			motionNum = 3;
			motion->attack();
			proximityAttackTime = 10;
		}
		else {
			proximityAttack = false;
		}
	}

	
	//HPの表示
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}

	repair = 0;

	motion->update(motionNum);


	
}

//描画
void Player::draw(int handle)
{
	//近接攻撃
	if (proximityAttack) {
		if (playerDirections) {
			DrawBox(playerPos.x - 65, playerPos.y + 10, playerPos.x  + 50, playerPos.y + 50, 0xff00ff, true);
			DrawString(playerPos.x - 65, playerPos.y + 10, "近接攻撃", 0x000000);
		}
		else {
			DrawBox(playerPos.x , playerPos.y + 10, playerPos.x + 115, playerPos.y + 50, 0xff00ff, true);
			DrawString(playerPos.x + 55, playerPos.y + 10, "近接攻撃", 0x000000);
		}
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


//近接攻撃の当たり判定
bool Player::proximityAttackCollision(const Vec2& pos)
{
	float enemyLeft = pos.x;
	float enemyTop = pos.y;
	float enemyRight = pos.x + 30;
	float enemyBottom = pos.y + 30;

	if (proximityAttack) {
		if (playerDirections) {
			if (enemyLeft > playerPos.x + 50)			return false;
			if (enemyRight < playerPos.x - 65)			return false;
			if (enemyTop <= playerPos.y + 10)			return false;
			if (enemyBottom <= playerPos.y + 50)		return false;
		}
		else if (!playerDirections) {
			if (enemyLeft > playerPos.x  + 115)			return false;
			if (enemyRight < playerPos.x)				return false;
			if (enemyTop <= playerPos.y + 10)			return false;
			if (enemyBottom <= playerPos.y + 50)		return false;
		}
		return true;
	}
	return false;
}


bool Player::repairSpace(const Vec2& pos)
{
	float spaceLeft = pos.x;
	float spaceTop = pos.y;
	float spaceRight = pos.x + 50;
	float spaceBottom = pos.y + 60;

	if (spaceLeft > playerPos.x + 50)				return false;
	if (spaceRight < playerPos.x)					return false;
	if (spaceTop > playerPos.y + 64)				return false;
	if (spaceBottom < playerPos.y)					return false;
		
	return true;

}

void Player::setRepair(int num)
{
	
	repair = num;
	if (repair) {
		DrawString(700, 0, "直せるよ", 0xffffff);
	}
	else {
		DrawString(700, 15, "直せないよ", 0xffffff);
	}
}

bool Player::playerFiledCollision(int y)
{
	float playerTop = playerPos.y;
	float playerBottom = playerPos.y + 78;

	float filedTop = y * Field::chipSize;
	float filedBottom = y * Field::chipSize + Field::chipSize;

	if (playerBottom < filedTop)return false;
	if (playerTop > filedBottom)return false;

	return true;
}