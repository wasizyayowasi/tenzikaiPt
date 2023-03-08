#include "Enemy.h"
#include "DxLib.h"
#include "../game.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../DrawFunctions.h"
#include "EnemyMotion.h"
#include "../field.h" 
#include "../DrawFunctions.h"
#include <algorithm>

Enemy::Enemy(int num) : sceneNum(num)
{
	hitHandle = my::myLoadGraph("data/hit.png");

	hp = new ObjectHp;
	motion = new EnemyMotion;
	if (sceneNum == 3) {
		enemyHp = 1000;
	}
	hp->setObjectMaxHp(enemyHp);

}

Enemy::~Enemy()
{
	delete hp;
	DeleteGraph(hitHandle);
}

void Enemy::tutorialUpdate(Vec2 offset)
{
	vec.y = 0.0f;

	int underfootChipNoX = (enemyPos.x + 20) / chipSize;
	int underfootChipNoY = (enemyPos.y + chipSize) / chipSize;

	const int chipNo = groundData::tutorialGround[underfootChipNoY][underfootChipNoX];

	if (chipNo == 0) {
		if (filedCollision(underfootChipNoX, underfootChipNoY)) {
			updateFunc = &Enemy::updateDescent;
		}
	}

	DrawFormatString(0, 30, 0xffffff, "%f : %f", enemyPos.x, enemyPos.y);

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
				if (targetPlayer.length() < 200) {

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
	if (enemyPos.x > 3000) {
		vec.x = 0.0f;
	}
	//左の壁反射
	if (enemyPos.x > 700) {
		enemyPos.x = 2900;
		inversion = true;
	}

	//移動
	if (motionNum != 3) {
		if (!stop) {
			if (landing) {
				enemyPos += vec;
			}
		}
	}

	//if (!player->beHidden()) {
	//	if (enemyHp > 0) {
	//		if (targetPlayer.length() < 50) {
	//			motionNum = 2;
	//			//攻撃
	//			if (--sleepTime < 0) {
	//				player->damege(inversion);
	//				sleepTime = 60;
	//			}
	//		}
	//	}
	//}

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

void Enemy::BossUpdate(Vec2 offset)
{
	//現状のHPを設定する
	hp->setObjectHp(enemyHp);

	landing = true;

	motionNum = 0;

	enemyPos.x += 3.5f;

	
	if (!player->beHidden()) {
		if (enemyHp > 0) {
			if (470 + enemyPos.x> player->getPos().x) {
				motionNum = 2;
				//攻撃
				if (--sleepTime < 0) {
					player->bossDamege();
					sleepTime = 60;
				}
			}
		}
	}

	if (landing) {
		motion->bossWalk();
	}
}

void Enemy::normalUpdate(Vec2 offset)
{
	vec.y = 0.0f;

	int underfootChipNoX = (enemyPos.x + 20) / chipSize;
	int underfootChipNoY = (enemyPos.y + chipSize) / chipSize;

	const int chipNo = groundData::ground[underfootChipNoY][underfootChipNoX];

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
				if (targetPlayer.length() < 200) {

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
	if (enemyPos.x > Game::kScreenWidth * 2 - 60) {
		vec.x = -vec.x;
		inversion = true;
	}
	//左の壁反射
	if (enemyPos.x + offset.x < 0) {
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

	//攻撃
	if (!player->beHidden()) {
		if (enemyHp > 0) {
			if (targetPlayer.length() < 50) {
				motionNum = 2;
				//攻撃
				if (--sleepTime < 0) {
					player->damege(inversion);
					sleepTime = 60;
				}
			}
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
				time = 4;
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
	
	DrawCircle(pos.x, pos.y, 200, 0xff0000, false);
	DrawString(pos.x, pos.y - 15, "敵", 0xffffff);

	motion->draw(enemyPos, handle, inversion, offset,2.0f);

	if (hpDisplay) {
		hp->draw(enemyPos, offset);
		
	}

	if (--time == 0) {
		imgX++;
		time = 4;
		if (imgX > 3) {
			imgX = 0;
			time = 0;
		}
		my::myDrawRectRotaGraph(pos.x, pos.y, imgX * 40, 0, 40, 40, 1.5f, 0.0f, hitHandle, true, false);
	}

	DrawFormatString(1910, 0, 0xffffff, "%d", imgX);
	
}

void Enemy::BossDraw(Vec2 offset)
{

	Vec2 pos = enemyPos + offset;

	inversion = false;

	DrawCircle(pos.x, pos.y, 200, 0xff0000, false);
	DrawString(pos.x, pos.y - 15, "敵", 0xffffff);

	motion->draw(enemyPos, handle, inversion, offset,50.0f);

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
	DrawGraph(enemyPos.x + offset.x, enemyPos.y + offset.y, coinHandle, true);
}

void Enemy::updateDescent(Vec2 offset)
{
	vec.y = 1.0f;
	enemyPos.y += vec.y * 4;

	int underfootChipNoX = (enemyPos.x + 20) / chipSize;
	int underfootChipNoY = (enemyPos.y + chipSize) / chipSize;

	switch (sceneNum) {
	case 0:
		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::ground[underfootChipNoY][underfootChipNoX];

			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				if (filedCollision(underfootChipNoX, underfootChipNoY)) {
					landing = true;
					updateFunc = &Enemy::normalUpdate;
				}
			}
		}
		break;
	case 1:
		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::tutorialGround[underfootChipNoY][underfootChipNoX];

			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				landing = true;
				updateFunc = &Enemy::tutorialUpdate;
			}
		}
		break;
	case 2:
		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::ground[underfootChipNoY][underfootChipNoX];

			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				if (filedCollision(underfootChipNoX, underfootChipNoY)) {
					landing = true;
					updateFunc = &Enemy::normalUpdate;
				}
			}
		}
		break;
	case 3:
		bunderfootChipNoX = (enemyPos.x + 20) / chipSize;
		bunderfootChipNoY = (enemyPos.y + chipSize) / chipSize;

		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::tutorialGround[underfootChipNoY + 12][underfootChipNoX];
			
			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
					landing = true;
					updateFunc = &Enemy::BossUpdate;
			}
		}
		break;
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
	switch (sceneNum) {
	case 0:
		updateFunc = &Enemy::updateDescent;
		drawFunc = &Enemy::normalDraw;
		break;
	case 1:
		updateFunc = &Enemy::updateDescent;
		drawFunc = &Enemy::normalDraw;
		break;
	case 2:
		updateFunc = &Enemy::updateDescent;
		drawFunc = &Enemy::normalDraw;
		break;
	case 3:
		updateFunc = &Enemy::updateDescent;
		drawFunc = &Enemy::BossDraw;
		break;
	}
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

	float filedLeft = x * chipSize;
	float filedRight = x * chipSize + chipSize;
	float filedTop = y * chipSize;
	float filedBottom = y * chipSize + chipSize;

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



