#include "Enemy.h"
#include "DxLib.h"
#include "../game.h"
#include "Player.h"
#include "../ObjectHp.h"


Enemy::Enemy()
{
	hp = new ObjectHp;
	hp->setObjectMaxHp(enemyHp);
}

void Enemy::dispatch(const Vec2& pos)
{
	enemyPos = pos;
	isEnabled = true;
}

void Enemy::update()
{
	hp->setObjectHp(enemyHp);

	Vec2 targetPlayer{ 0,0 };

	targetPlayer.x = player->getPos().x - enemyPos.x;
	targetPlayer.y = player->getPos().y - enemyPos.y;

	
	//右の壁反射
	if (enemyPos.x + 30 > Game::kScreenWidth) {
		hit = true;
		vec.x = -vec.x;
	}

	//左の壁反射
	if (enemyPos.x < 0) {
		hit = false;
		vec.x = -vec.x;
	}

	//追いかけない場合
	if (enemyPos.y + 30 >= 700) {
		if (!chase) {
			if (!stop) {
				enemyPos += vec;
			}
		}
	}
	else {
		enemyPos.y += vec.y * 2;
	}
	
	//地面に並行して動く
	if (enemyPos.y + 30 > 700) {
		enemyPos.y = 670;
	}

	//プレイヤーが隠れているか
	hidden = player->beHidden();

	//追いかけるか追いかけないか
	if (!hidden) {
		if (targetPlayer.length() < 64) {
			stop = true;
			player->damege();
		}
		else if (targetPlayer.length() < 500) {
			float playerX = player->getPos().x;
			float playerY = player->getPos().y;

			Vec2 targetPlayer2{ 0.0f,0.0f };
			targetPlayer2.x = playerX - enemyPos.x;
			//targetPlayer2.y = playerY - enemyPos.y;

			targetPlayer2 = targetPlayer2.normalize();

			enemyPos += targetPlayer2 * 4;
			chase = true;
		}
		else {
			chase = false;
			stop = false;
		}
	}
	else {
		chase = false;
		hidden = false;
		stop = false;
	}

	if (player->enemyAttack(enemyPos)) {
		enemyHp = 0;
		isEnabled = false;
	}

}

void Enemy::draw()
{
	DrawBox(enemyPos.x - 70, enemyPos.y - 70, enemyPos.x + 100, enemyPos.y + 100, GetColor(255, 0, 0), false);
	DrawBox(enemyPos.x, enemyPos.y, enemyPos.x + 30, enemyPos.y + 30, GetColor(0, 0, 255), true);

	hp->draw(enemyPos);
}

bool Enemy::isEnable() const
{
	return isEnabled;
}

void Enemy::hitFlyingObject()
{

}



