#include "Enemy.h"
#include "DxLib.h"
#include "../game.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../DrawFunctions.h"
#include "EnemyMotion.h"
#include "../field.h" 

Enemy::Enemy()
{
	hp = new ObjectHp;
	motion = new EnemyMotion;
	hp->setObjectMaxHp(enemyHp);
	updateFunc = &Enemy::normalUpdate;
	drawFunc = &Enemy::normalDraw;
}

void Enemy::normalUpdate(Vec2 offset)
{
	vec.y = 0.0f;

	int underfootChipNoX = (enemyPos.x + 20) / FieldData::chipSize;
	int underfootChipNoY = (enemyPos.y + FieldData::chipSize) / FieldData::chipSize;

	const int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX];

	if (chipNo == 0) {
		if (filedCollision(underfootChipNoX, underfootChipNoY)) {
			updateFunc = &Enemy::updateDescent;
		}
	}
	

	//現状のHPを設定する
	hp->setObjectHp(enemyHp);

	motionNum = 0;

	//HPがなくなり死亡した場合
	if (enemyHp == 0) {
		motionNum = 3;
	}

	//プレイヤーが隠れているか
	hidden = player->beHidden();

	//ターゲットの座標を見つける
	targetPlayer = { 0.0f,0.0f };
	targetPlayer.x = player->getPos().x + 25 - enemyPos.x;
	targetPlayer.y = player->getPos().y + 44 - enemyPos.y;

	//検知範囲内だった場合
	if (motionNum != 3) {
		if (!hidden) {
			if (landing) {
				if (targetPlayer.length() < 300) {

					targetPlayer2 = { 0.0f,0.0f };
					targetPlayer2.x = player->getPos().x + 25 - enemyPos.x;
					//targetPlayer2.y = player->getPos().y + 44 - enemyPos.y;

					targetPlayer2 = targetPlayer2.normalize() * 3;

					stop = true;

					motionNum = 1;

					if (--sleepTime < 0) {
						enemyPos += targetPlayer2;
					}

					//画像の向き
					if (targetPlayer.x > 0) {
						inversion = false;
					}
					else {
						inversion = true;

					}
				}
				else {

					//画像の向き
					if (vec.x > 0) {
						inversion = false;
					}
					else {
						inversion = true;

					}
					stop = false;
				}
			}
		}
		else {
			stop = false;
			//画像の向き
			if (vec.x > 0) {
				inversion = false;
			}
			else {
				inversion = true;

			}
			stop = false;
		}
	}

	//右の壁反射
	if (moveCount > 130) {
		vec.x = -vec.x;
		inversion = true;
	}
	//左の壁反射
	if (moveCount < 0) {
		vec.x = -vec.x;
		inversion = false;
	}

	//移動
	if (motionNum != 3) {
		if (!stop) {
			if (landing) {
				enemyPos += vec;
				if (vec.x > 0) {
					moveCount++;
				}
				else {
					moveCount--;
				}
			}
		}
	}

	if (targetPlayer.length() < 50) {
		motionNum = 2;
		//攻撃
		if (--sleepTime < 0) {
			player->damege(inversion);
			sleepTime = 60;
		}
	}

	//投擲が当たった場合の処理
	if (player->enemyAttack(enemyPos, offset)) {
		enemyHp = 0;
	}

	//プレイヤーの近接攻撃との当たり判定
	if (enemyHp > 0) {
		if (--coolTime < 0) {
			if (player->proximityAttackCollision(enemyPos)) {
				enemyHp -= 3;
				hpDisplay = true;
				coolTime = 23;
			}
		}
	}

	//HPを描画する際の条件
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}

	if (motion->dead()) {
		//motion->init();
		//isEnabled = false;
		updateFunc = &Enemy::coinUpdate;
		drawFunc = &Enemy::coinDraw;
	}

	//エネミーの動きを更新
	if (landing) {
		motion->update(motionNum);
	}
}

void Enemy::normalDraw(Vec2 offset)
{
	Vec2 pos = enemyPos + offset;

	{
		/*int underfootChipNoX = (enemyPos.x + 20) / FieldData::chipSize;
		int underfootChipNoY = (enemyPos.y + 32) / FieldData::chipSize;

		int color = 0x44ff44;

		if (updateFunc == &Enemy::updateDescent) {
			color = 0x4444ff;
		}
		DrawBox(underfootChipNoX * 32, underfootChipNoY * 32, underfootChipNoX * 32 + 32, underfootChipNoY * 32 + 32, color, true);*/

	}
	
	DrawCircle(pos.x, pos.y, 300, 0xff0000, false);
	DrawString(pos.x, pos.y - 15, "敵", 0xffffff);

	motion->draw(enemyPos, handle, inversion, offset);

	if (hpDisplay) {
		hp->draw(enemyPos, offset);
	}

}

void Enemy::coinUpdate(Vec2 offset)
{
	if (player->coinCollision(enemyPos,offset)) {
		isEnabled = false;
	}
}

void Enemy::coinDraw(Vec2 offset)
{
	DrawBox(enemyPos.x + offset.x, enemyPos.y + offset.y, enemyPos.x + 30 + offset.x, enemyPos.y + 30 + offset.y, 0xffffff, true);
}

void Enemy::updateDescent(Vec2 offset)
{
	vec.y = 1.0f;
	enemyPos.y += vec.y * 4;

	int underfootChipNoX = (enemyPos.x + 20) / FieldData::chipSize;
	int underfootChipNoY = (enemyPos.y + FieldData::chipSize) / FieldData::chipSize;

	const int chipNo = FieldData::field[underfootChipNoY][underfootChipNoX];

	if (chipNo == 1) {
		if (filedCollision(underfootChipNoX, underfootChipNoY)) {
			landing = true;
			updateFunc = &Enemy::normalUpdate;
		}
	}
}

void Enemy::dispatch(const Vec2& pos)
{
	isEnabled = true;
	hpDisplay = false;
	landing = false;
	stop = false;
	enemyPos = pos;
	enemyHp = 9;
	hpDisplayTime = 120;
	motionNum = 0;
	motion->init();
	vec = { 3.0f,0.0f };
	updateFunc = &Enemy::normalUpdate;
	drawFunc = &Enemy::normalDraw;
}

void Enemy::update(Vec2 offset)
{
	(this->*updateFunc)(offset);
}

void Enemy::draw(Vec2 offset)
{
	(this->*drawFunc)(offset);
}

bool Enemy::isEnable() const
{
	return isEnabled;
}

bool Enemy::filedCollision(int x,int y)
{

	float filedLeft = x * FieldData::chipSize;
	float filedRight = x * FieldData::chipSize + FieldData::chipSize;
	float filedTop = y * FieldData::chipSize;
	float filedBottom = y * FieldData::chipSize + FieldData::chipSize;

	if (enemyPos.x + 15< filedLeft)		return false;
	if (enemyPos.x > filedRight)		return false;
	if (enemyPos.y + 34 < filedTop)		return false;
	if (enemyPos.y > filedBottom)		return false;

	return true;
}

Vec2 Enemy::deadPos()
{
	return deathPos;
}



