#include "Enemy.h"
#include "DxLib.h"
#include "../game.h"
#include "Player.h"


Enemy::Enemy()
{
}

void Enemy::dispatch(const Vec2& pos)
{
	enemyPos = pos;
	isEnabled = true;
}

void Enemy::update()
{

	Vec2 targetPlayer{ 0,0 };

	targetPlayer.x = player->getPos().x - enemyPos.x;
	targetPlayer.y = player->getPos().y - enemyPos.y;

	if (enemyPos.x + 30 > Game::kScreenWidth) {
		hit = true;
		vec.x = -vec.x;
	}

	if (enemyPos.x < 0) {
		hit = false;
		vec.x = -vec.x;
	}

	if (!chase) {
		enemyPos += vec;
	}
	
	if (enemyPos.y + 64 > 700) {
		enemyPos.y = 680 - 30;
	}

	hidden = player->beHidden();

	if (!hidden) {
		if (targetPlayer.length() < 500) {
			float playerX = player->getPos().x;
			float playerY = player->getPos().y;

			Vec2 targetPlayer2{ 0.0f,0.0f };
			targetPlayer2.x = playerX - enemyPos.x;
			targetPlayer2.y = playerY - enemyPos.y;

			targetPlayer2 = targetPlayer2.normalize();

			enemyPos += targetPlayer2 * 4;
			chase = true;
		}
		else {
			chase = false;
		}
	}
	else {
		chase = false;
		hidden = false;
	}
}

void Enemy::draw()
{
	DrawBox(enemyPos.x - 70, enemyPos.y - 70, enemyPos.x + 100, enemyPos.y + 100, GetColor(255, 0, 0), false);
	DrawBox(enemyPos.x, enemyPos.y, enemyPos.x + 30, enemyPos.y + 30, GetColor(0, 0, 255), true);
}

bool Enemy::isEnable() const
{
	return isEnabled;
}

void Enemy::hitFlyingObject()
{

}


