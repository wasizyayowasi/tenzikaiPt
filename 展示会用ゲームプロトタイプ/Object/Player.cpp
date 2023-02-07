#include "Player.h"
#include "../Pad.h"
#include "DxLib.h"
#include "PlayerThrowinAttack.h"
#include "ObjectHp.h"

namespace {
	constexpr int playerSizeX = 50;
	constexpr int playerSizeY = 64;

	constexpr int groundY = 700;
}

Player::Player()
{
	hp = new ObjectHp;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
}

void Player::update()
{
	//毎フレーム、プレイヤーのHPを確認する
	hp->setObjectHp(playerHp);

	//重力を足し続ける
	if (!ladder) {
		if (upperLimit) {
			vec.y = 20.0f;
		}
		else {
			vec.y = 10.0f;
		}
		playerPos += vec;
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
			playerPos.x -= 10;
			playerDirections = 1;
		}
		if (Pad::isPress(PAD_INPUT_RIGHT)) {
			playerPos.x += 10;
			playerDirections = 2;
		}
		if (ladder) {
			if (!upperLimit) {
				if (Pad::isPress(PAD_INPUT_UP)) {
					playerPos.y -= 10;
				}
			}
		}
		if (!hit) {
			if (Pad::isPress(PAD_INPUT_DOWN)) {
				playerPos.y += 10;
			}
		}
	}


	//地面との判定
	if (playerPos.y + playerSizeY > groundY) {
		playerPos.y = groundY - playerSizeY;
		hit = true;
		upperLimit = false;
	}
	else {
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

}

//描画
void Player::draw()
{
	//近接攻撃
	if (proximityAttack) {
		if (playerDirections == 1) {
			DrawBox(playerPos.x - 65, playerPos.y + 10, playerPos.x  + 50, playerPos.y + 50, 0xff00ff, true);
			DrawString(playerPos.x - 65, playerPos.y + 10, "近接攻撃", 0x000000);
		}
		else {
			DrawBox(playerPos.x , playerPos.y + 10, playerPos.x + 115, playerPos.y + 50, 0xff00ff, true);
			DrawString(playerPos.x + 55, playerPos.y + 10, "近接攻撃", 0x000000);
		}
	}
	
	if (!push) {
		if (playerDirections == 1) {
			DrawBox(playerPos.x - 20, playerPos.y, playerPos.x, playerPos.y + 10, GetColor(255, 0, 0), true);
			DrawString(playerPos.x - 20, playerPos.y - 15, "向き", 0xffffff);
		}
		else if (playerDirections == 2) {
			DrawBox(playerPos.x + 50, playerPos.y, playerPos.x + 70, playerPos.y + 10, GetColor(255, 0, 0), true);
			DrawString(playerPos.x + 50, playerPos.y - 15, "向き", 0xffffff);
		}
	}
	

	DrawBox(playerPos.x, playerPos.y, playerPos.x + playerSizeX, playerPos.y + playerSizeY, GetColor(255, 255, 255), true);
	DrawString(playerPos.x, playerPos.y + 30, "プレイヤー", 0xff00ff);
	DrawBox(playerPos.x+25, playerPos.y+32, playerPos.x + 27, playerPos.y + 34, 0x000000,true);

	//飛び道具
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	if (hpDisplay) {
		hp->draw(playerPos);
	}
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
		if (playerDirections == 1) {
			if (enemyLeft > playerPos.x + 50)			return false;
			if (enemyRight < playerPos.x - 65)			return false;
			if (enemyTop <= playerPos.y + 10)			return false;
			if (enemyBottom <= playerPos.y + 50)		return false;
		}
		else if (playerDirections == 2) {
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
	DrawString(600, 0, "aiu1", 0xffffff);
	if (spaceRight < playerPos.x)					return false;
	DrawString(600, 15, "aiu1", 0xffffff);
	if (spaceTop > playerPos.y + 64)				return false;
	DrawString(600, 30, "aiu1", 0xffffff);
	if (spaceBottom < playerPos.y)					return false;
	DrawString(600, 45, "aiu1", 0xffffff);
		
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
