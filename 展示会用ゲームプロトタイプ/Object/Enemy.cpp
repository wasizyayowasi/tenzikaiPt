#include "Enemy.h"
#include "DxLib.h"
#include "../game.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../DrawFunctions.h"
#include "../EnemyMotion.h"
#include "../field.h"


Enemy::Enemy()
{
	hp = new ObjectHp;
	motion = new EnemyMotion;
	hp->setObjectMaxHp(enemyHp);
}

void Enemy::dispatch(const Vec2& pos)
{
	isEnabled = true;
	hpDisplay = false;
	landing = false;
	enemyPos = pos;
	enemyHp = 9;
	hpDisplayTime = 120;
	motionNum = 0;
	motion->init();
	vec = { 3.0f,1.0f };
}

void Enemy::update()
{

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 1) {
				if (filedCollision(y)) {
					landing = true;
					vec.y = 0.0f;
				}
			}
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

	

	//追いかけない場合
	if(!landing){
		enemyPos.y += vec.y * 4;
	}


	//検知範囲内だった場合
	if (motionNum != 3) {
		if (!hidden) {
			if (landing) {
				if (targetPlayer.length() < 400) {

					targetPlayer2 = { 0.0f,0.0f };
					targetPlayer2.x = player->getPos().x + 25 - enemyPos.x;
					targetPlayer2.y = player->getPos().y + 44 - enemyPos.y;

					targetPlayer2 = targetPlayer2.normalize() * 3;

					stop = true;

					motionNum = 1;
					//画像の向き
					if (targetPlayer.x > 0) {
						inversion = false;
					}
					else {
						inversion = true;

					}

					if (targetPlayer.length() < 50) {
						motionNum = 2;
					}
					else {
						enemyPos += targetPlayer2;
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
	if (enemyPos.x + 30 > Game::kScreenWidth) {
		vec.x = -vec.x;
		inversion = true;
	}
	//左の壁反射
	if (enemyPos.x < 0) {
		vec.x = -vec.x;
		inversion = false;
	}

	//移動
	if (motionNum != 3) {
		if (!stop) {
			if (landing) {
				enemyPos += vec;
			}
		}
	}
	

	//攻撃
	if (motionNum == 2) {
		player->damege();
	}

	//投擲が当たった場合の処理
	if (player->enemyAttack(enemyPos)) {
		enemyHp = 0;
		isEnabled = false;
	}

	//プレイヤーの近接攻撃との当たり判定
	if (enemyHp > 0) {
		if (player->proximityAttackCollision(enemyPos)) {
			enemyHp -= 3;
			hpDisplay = true;
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
		isEnabled = false;
	}

	//エネミーの動きを更新
	if (landing) {
		motion->update(motionNum);
	}
}

void Enemy::draw()
{
	DrawCircle(enemyPos.x, enemyPos.y, 400, 0xff0000, false);
	DrawString(enemyPos.x, enemyPos.y - 15, "敵", 0xffffff);

	DrawFormatString(0, 15, 0xffffff, "%d", motionNum);

	motion->draw(enemyPos,handle,inversion);

	if (hpDisplay) {
		hp->draw(enemyPos);
	}
}

bool Enemy::isEnable() const
{
	return isEnabled;
}

bool Enemy::filedCollision(int y)
{
	float playerTop = enemyPos.y;
	float playerBottom = enemyPos.y + 34;

	float filedTop = y * Field::chipSize;
	float filedBottom = y * Field::chipSize + Field::chipSize;

	if (playerBottom < filedTop)return false;
	if (playerTop > filedBottom)return false;

	return true;
}



