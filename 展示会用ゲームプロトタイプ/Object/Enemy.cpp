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

Enemy::Enemy(int num) : sceneNum(num),drawFunc(&Enemy::normalDraw),updateFunc(&Enemy::updateDescent)
{
	hitHandle = my::myLoadGraph("data/effect/hit.png");
	windHandle = my::myLoadGraph("data/objectGraph/Wind.png");
	hitSound = LoadSoundMem("data/soundEffect/hit.mp3");
	deathSound = LoadSoundMem("data/soundEffect/enemy3.mp3");

	player = nullptr;

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
	delete motion;
	DeleteGraph(hitHandle);
	DeleteGraph(windHandle);
	DeleteSoundMem(hitSound);
	DeleteSoundMem(deathSound);
}

void Enemy::tutorialUpdate(Vec2 offset)
{
	vec.y = 0.0f;

	int underfootChipNoX = static_cast<int>(enemyPos.x + 20) / chipSize;
	int underfootChipNoY = static_cast<int>(enemyPos.y + chipSize) / chipSize;

	const int chipNo = groundData::tutorialGround[underfootChipNoY][underfootChipNoX];

	if (chipNo == 0) {
		if (filedCollision(underfootChipNoX, underfootChipNoY)) {
			updateFunc = &Enemy::updateDescent;
			return;
		}
	}

	motionNum = 0;

	//HPがなくなり死亡した場合
	if (hp->returnTempHp() <= 0.0f) {
		motionNum = 3;
	}

	//プレイヤーが隠れているか
	hidden = player->beHidden();

	//ターゲットの座標を見つける
	targetPlayer = { 0.0f,0.0f };
	targetPlayer.x = player->getPos().x + 18 - enemyPos.x;
	targetPlayer.y = player->getPos().y + 44 - enemyPos.y;

	//検知範囲内だった場合
	if (motionNum != 3) {
		if (!hidden) {
			if (landing) {
				if (targetPlayer.length() < 200) {

					targetPlayer2 = { 0.0f,0.0f };
					targetPlayer2.x = player->getPos().x + 18 - enemyPos.x;
					//targetPlayer2.y = player->getPos().y + 44 - enemyPos.y;

					targetPlayer2 = targetPlayer2.normalize() * 3;

					stop = true;

					motionNum = 1;

					if (enemyHp > 0) {
						if (!attackRange) {
							enemyPos += targetPlayer2;
						}
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

	//停止
	if (enemyPos.x > 2200) {
		vec.x = 0.0f;
		inversion = true;
	}

	//左の壁反射
	if (enemyPos.x < 600) {
		vec.x = 3.0f;
		inversion = false;
	}
	if (enemyPos.x > 1100 && enemyPos.x < 1400) {
		vec.x = -3.0f;
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

	//投擲が当たった場合の処理
	if (player->enemyAttack(enemyPos, offset)) {
		ChangeVolumeSoundMem(soundVolume, hitSound);
		PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
		enemyHp = 0;
		hpDisplay = true;
	}

	//プレイヤーの近接攻撃との当たり判定
	if (enemyHp > 0) {
		if (--coolTime < 0) {
			if (player->proximityAttackCollision(enemyPos)) {
				ChangeVolumeSoundMem(soundVolume, hitSound);
				PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
				enemyHp -= 3;
				hpDisplay = true;
				coolTime = 23;
				if (inversion) {
					enemyPos.x += 10.0f;
				}
				else {
					enemyPos.x -= 10.0f;
				}
			}
		}
	}

	//HPを描画する際の条件
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 180;
		}
	}

	if (motion->dead()) {
		updateFunc = &Enemy::coinUpdate;
		drawFunc = &Enemy::coinDraw;
	}

	//エネミーの動きを更新
	if (landing) {
		motion->update(motionNum,0);
	}

	//現状のHPを設定する
	hp->setObjectHp(enemyHp);

}

void Enemy::BossUpdate(Vec2 offset)
{
	if (enemyPos.y > 416) {
		enemyPos.y = 416;
	}

	//現状のHPを設定する
	hp->setObjectHp(enemyHp);

	landing = true;

	motionNum = 0;

	if (!player->returnEnemyHit()) {
		enemyPos.x += 3.7f;
	}
	
	
	//HPがなくなり死亡した場合
	if (enemyHp == 0) {
		motionNum = 4;
	}
	
	if (motion->dead()) {
		isEnabled = false;
	}

	if (!player->beHidden()) {
		if (enemyHp > 0) {
			if (450 + enemyPos.x> player->getPos().x) {
				motionNum = 2;
				//攻撃
				if (--sleepTime < 0) {
					player->bossDamege();
					sleepTime = 60;
				}
			}
		}
	}

	if (enemyHp > 0) {
		if (player->bossEnemyAttack(enemyPos, offset)) {
			ChangeVolumeSoundMem(180, deathSound);
			PlaySoundMem(deathSound, DX_PLAYTYPE_BACK);
			enemyHp = 0;
		}
	}

	checkSound = CheckSoundMem(deathSound);

	if (checkSound != 1) {
		if (landing) {
			motion->update(motionNum, 1);
		}
	}
}

void Enemy::normalUpdate(Vec2 offset)
{
	vec.y = 0.0f;

	int underfootChipNoX = static_cast<int>(enemyPos.x + 20) / chipSize;
	int underfootChipNoY = static_cast<int>(enemyPos.y + chipSize) / chipSize;

	const int chipNo = groundData::ground[underfootChipNoY][underfootChipNoX];

	if (chipNo == 16) {
		tempChipNoX = underfootChipNoX;
		tempChipNoY = underfootChipNoY;
	}

	if (chipNo == 0) {
		if (filedCollision(underfootChipNoX, underfootChipNoY)) {
			updateFunc = &Enemy::updateDescent;
		}
	}

	if (steamVentTime < 1) {
		steamVentTime = 1;
	}

	//宙を舞う
	if (--steamVentTime == 0) {
		if (chipNo == 16) {
			if (inversion) {
				jumpVec = { -1.7f,0.0f };
			}
			else {
				jumpVec = { 1.7f,0.0f };
			}
			windJump = true;
			updateFunc = &Enemy::jumpUpdate;
			steamVentTime = GetRand(40);
			return;
		}
	}

	motionNum = 0;

	//プレイヤーが隠れているか
	hidden = player->beHidden();

	//ターゲットの座標を見つける
	targetPlayer = { 0.0f,0.0f };
	targetPlayer.x = player->getPos().x + 18 - enemyPos.x;
	targetPlayer.y = player->getPos().y + 44 - enemyPos.y;

	//検知範囲内だった場合
	if (motionNum != 3) {
		if (!hidden) {
			if (landing) {
				if (targetPlayer.length() < 200) {

					targetPlayer2 = { 0.0f,0.0f };
					targetPlayer2.x = player->getPos().x + 18 - enemyPos.x;
					//targetPlayer2.y = player->getPos().y + 44 - enemyPos.y;

					targetPlayer2 = targetPlayer2.normalize() * 3;

					stop = true;

					motionNum = 1;

					if (--sleepTime < 0) {
						if (enemyHp > 0) {
							if (!attackRange) {
								if (--damageStopTime < 1) {
									enemyPos += targetPlayer2;
								}
							}
						}
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

	//HPがなくなり死亡した場合
	if (hp->returnTempHp() <= 0.0f) {
		motionNum = 3;
	}

	//右の壁反射
	if (enemyPos.x > Game::kScreenWidth * 2 - 60) {
		vec.x = -vec.x;
		inversion = true;
	}
	//左の壁反射
	if (enemyPos.x< 0) {
		vec.x = -vec.x;
		inversion = false;
	}

	//移動
	if (!stop) {
		if (landing) {
			if (enemyHp > 0) {
				enemyPos += vec;
			}
		}
	}

	//攻撃
	if (!player->beHidden()) {
		if (enemyHp > 0) {
			if (!inversion) {
				if (targetPlayer.length() < 60) {
					attackRange = true;
					vec.x = 0.0f;
					motionNum = 2;
					//攻撃
					if (--sleepTime < 0) {
						player->damege(inversion);
						sleepTime = 60;
					}
				}
			}
			else {
				if (targetPlayer.length() < 20) {
					attackRange = true;
					vec.x = 0.0f;
					motionNum = 2;
					//攻撃
					if (--sleepTime < 0) {
						player->damege(inversion);
						sleepTime = 60;
					}
				}
			}
		}
	}

	//投擲が当たった場合の処理
	if (player->enemyAttack(enemyPos, offset)) {
		ChangeVolumeSoundMem(soundVolume, hitSound);
		PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
		enemyHp = 0;
		hpDisplay = true;
	}

	//プレイヤーの近接攻撃との当たり判定
	if (enemyHp > 0.0f) {
		if (--coolTime < 0) {
			if (player->proximityAttackCollision(enemyPos)) {
				ChangeVolumeSoundMem(soundVolume, hitSound);
				PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);
				enemyHp -= 3;
				hpDisplay = true;
				coolTime = 20;
				time = 4;
				vec.x = 0.0f;
				damageStopTime = 20;
				if (inversion) {
					enemyPos.x += 10.0f;
				}
				else {
					enemyPos.x -= 10.0f;
				}
			}
		}
	}

	if (damageStopTime == 1) {
		if (!inversion) {
			vec.x = 3.0f;
		}
		else {
			vec.x = -3.0f;
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
		updateFunc = &Enemy::coinUpdate;
		drawFunc = &Enemy::coinDraw;
	}

	//エネミーの動きを更新
	if (landing) {
		motion->update(motionNum,0);
	}

	//現状のHPを設定する
	hp->setObjectHp(enemyHp);

	if (vec.x == 0.0f) {
		if (--stopOverTime > 0) {
			vec.x = 3.0f;
			stopOverTime = 60;
			attackRange = false;
		}
	}
}

void Enemy::normalDraw(Vec2 offset)
{
	Vec2 pos = enemyPos + offset;
	
	if (windJump) {
		if (--windTime == 0) {
			windImgX++;
			windTime = 8;
		}
		DrawRectRotaGraph(static_cast<int>(tempChipNoX * chipSize + 8 + offset.x), (tempChipNoY - 1) * chipSize - 10, windImgX * 48, 0, 48, 32, 2.0f, 4.6f, windHandle, true, false);
		if (windImgX > 11) {
			windImgX = 0;
			windJump = false;
		}
	}

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
		my::myDrawRectRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), imgX * 40, 0, 40, 40, 1.5f, 0.0f, hitHandle, true, false);
	}
}

void Enemy::BossDraw(Vec2 offset)
{

	Vec2 pos = enemyPos + offset;

	inversion = false;

	motion->draw(enemyPos, handle, inversion, offset,50.0f);

	if (hpDisplay) {
		hp->draw(enemyPos, offset);
	}

}

void Enemy::titleUpdate(Vec2 offset)
{
	landing = false;
	enemyPos += vec;
	motionNum = 1;

	if (enemyPos.y > 800) {
		enemyPos.y = 800;
		landing = true;
	}

	if (landing) {
		vec.y = jump;
		if (jump < 0) {
			jump += 0.3f;
		}
	}

	vec.y += 0.1f;

	if (enemyPos.x + 40 < 0) {
		isEnabled = false;
	}

	motion->update(motionNum, 0);

}

void Enemy::jumpUpdate(Vec2 offset)
{
	if (landing) {
		if (jumpVec.y > -25.0f) {
			jumpVec.y -= 0.8f;
			landing = true;
		}
		else {
			landing = false;
		}
	}

	if (!landing) {
		jumpVec.y += 1.0f;
	}

	enemyPos += jumpVec;

	int underfootChipNoX = static_cast<int>(enemyPos.x + 20) / chipSize;
	int underfootChipNoY = static_cast<int>(enemyPos.y + chipSize) / chipSize;

	for (int i = 0; i < 1; i++) {
		const int chipNo = groundData::ground[underfootChipNoY][underfootChipNoX];

		if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46||chipNo == 16||chipNo == 30) {
			enemyPos.y = static_cast<float>(underfootChipNoY - 1) * chipSize;
			landing = true;
			updateFunc = &Enemy::normalUpdate;
		}
	}
}

void Enemy::coinUpdate(Vec2 offset)
{
	if (--coinUpdateTime == 0) {
		coinVec.y = -coinVec.y;
		coinUpdateTime = 50;
	}

	enemyPos.y += coinVec.y;

	if (player->coinCollision(enemyPos,offset)) {
		isEnabled = false;
	}
}

void Enemy::coinDraw(Vec2 offset)
{
	DrawGraphF(enemyPos.x + offset.x, enemyPos.y + offset.y, coinHandle, true);
}

void Enemy::updateDescent(Vec2 offset)
{
	vec.y = 1.0f;
	enemyPos.y += vec.y * 4;

	int underfootChipNoX = static_cast<int>(enemyPos.x + 20) / chipSize;
	int underfootChipNoY = static_cast<int>(enemyPos.y + chipSize) / chipSize;

	switch (sceneNum) {
	case 0:
		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::ground[underfootChipNoY][underfootChipNoX];

			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				landing = true;
				updateFunc = &Enemy::normalUpdate;
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
				landing = true;
				updateFunc = &Enemy::normalUpdate;
			}
		}
		break;
	case 3:
		bunderfootChipNoX = static_cast<int>(enemyPos.x + 20) / chipSize;
		bunderfootChipNoY = static_cast<int>(enemyPos.y + chipSize) / chipSize;

		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::tutorialGround[underfootChipNoY + 12][underfootChipNoX];
			
			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				landing = true;
				updateFunc = &Enemy::BossUpdate;
			}
		}
		break;
	case 4:
		for (int i = 0; i < 1; i++) {
			const int chipNo = groundData::tutorialGround[underfootChipNoY][underfootChipNoX];

			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				landing = true;
				updateFunc = &Enemy::titleUpdate;
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
	hp->setObjectMaxHp(enemyHp);
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
	case 4:
		vec = { -3.0f,0.0f };
		jump = -3.0f;
		updateFunc = &Enemy::updateDescent;
		drawFunc = &Enemy::normalDraw;
		break;
	}
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

	int filedLeft = x * chipSize;
	int filedRight = x * chipSize + chipSize;
	int filedTop = y * chipSize;
	int filedBottom = y * chipSize + chipSize;

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



