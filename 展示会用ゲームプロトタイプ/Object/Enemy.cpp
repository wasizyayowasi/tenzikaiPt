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
	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {

			const int chipNo = FieldData::field[y][x];

			if (chipNo == 1) {
				if (filedCollision(x, y)) {
					landing = true;
					vec.y = 0.0f;
				}
			}
		}
	}

	//�����HP��ݒ肷��
	hp->setObjectHp(enemyHp);

	motionNum = 0;

	//HP���Ȃ��Ȃ莀�S�����ꍇ
	if (enemyHp == 0) {
		motionNum = 3;
	}

	//�v���C���[���B��Ă��邩
	hidden = player->beHidden();

	//�^�[�Q�b�g�̍��W��������
	targetPlayer = { 0.0f,0.0f };
	targetPlayer.x = player->getPos().x + 25 - enemyPos.x;
	targetPlayer.y = player->getPos().y + 44 - enemyPos.y;


	//���m�͈͓��������ꍇ
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
					//�摜�̌���
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

					//�摜�̌���
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
			//�摜�̌���
			if (vec.x > 0) {
				inversion = false;
			}
			else {
				inversion = true;

			}
			stop = false;
		}
	}

	//�E�̕ǔ���
	if (moveCount > 130) {
		vec.x = -vec.x;
		inversion = true;
	}
	//���̕ǔ���
	if (moveCount < 0) {
		vec.x = -vec.x;
		inversion = false;
	}

	//�ړ�
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
			else {
				//�~��
				enemyPos.y += vec.y * 4;
			}
		}
	}



	//�U��
	if (motionNum == 2) {
		player->damege();
	}

	//���������������ꍇ�̏���
	if (player->enemyAttack(enemyPos, offset)) {
		enemyHp = 0;
	}

	//�v���C���[�̋ߐڍU���Ƃ̓����蔻��
	if (enemyHp > 0) {
		if (--coolTime < 0) {
			if (player->proximityAttackCollision(enemyPos)) {
				enemyHp -= 3;
				hpDisplay = true;
				coolTime = 23;
			}
		}
	}

	//HP��`�悷��ۂ̏���
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

	//�G�l�~�[�̓������X�V
	if (landing) {
		motion->update(motionNum);
	}
}

void Enemy::normalDraw(Vec2 offset)
{
	Vec2 pos = enemyPos + offset;

	DrawCircle(pos.x, pos.y, 300, 0xff0000, false);
	DrawString(pos.x, pos.y - 15, "�G", 0xffffff);

	DrawFormatString(0, 15, 0xffffff, "%d", motionNum);

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
	vec = { 3.0f,1.0f };
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



