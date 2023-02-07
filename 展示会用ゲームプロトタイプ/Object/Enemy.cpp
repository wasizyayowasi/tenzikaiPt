#include "Enemy.h"
#include "DxLib.h"
#include "../game.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../DrawFunctions.h"
#include "../EnemyMotion.h"


Enemy::Enemy()
{
	hp = new ObjectHp;
	motion = new EnemyMotion;
	hp->setObjectMaxHp(enemyHp);
}

void Enemy::dispatch(const Vec2& pos)
{
	enemyPos = pos;
	isEnabled = true;
	enemyHp = 9;
	hpDisplay = false;
	hpDisplayTime = 120;
}

void Enemy::update()
{
	//�����HP��ݒ肷��
	hp->setObjectHp(enemyHp);

	motionNum = 0;

	//�v���C���[���B��Ă��邩
	hidden = player->beHidden();

	//�^�[�Q�b�g�̍��W��������
	targetPlayer = { 0.0f,0.0f };
	targetPlayer.x = player->getPos().x + 25 - enemyPos.x;
	targetPlayer.y = player->getPos().y + 32 - enemyPos.y;

	

	//�ǂ������Ȃ��ꍇ
	if (enemyPos.y + 30 >= 700) {
		enemyPos.y = 700 - 30;
		landing = true;
	}
	if(!landing){
		enemyPos.y += vec.y * 4;
	}


	//���m�͈͓��������ꍇ
	if (!hidden) {
		if (landing) {
			if (targetPlayer.length() < 400) {

				targetPlayer2 = { 0.0f,0.0f };
				targetPlayer2.x = player->getPos().x + 25 - enemyPos.x;
				targetPlayer2.y = player->getPos().y + 32 - enemyPos.y;

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
	
	
	//�E�̕ǔ���
	if (enemyPos.x + 30 > Game::kScreenWidth) {
		vec.x = -vec.x;
		inversion = true;
	}
	//���̕ǔ���
	if (enemyPos.x < 0) {
		vec.x = -vec.x;
		inversion = false;
	}

	//�ړ�
	if (!stop) {
		if (landing) {
			enemyPos += vec;
		}
	}
	

	//�U��
	if (motionNum == 2) {
		player->damege();
	}

	//���������������ꍇ�̏���
	if (player->enemyAttack(enemyPos)) {
		enemyHp = 0;
		isEnabled = false;
	}

	//�v���C���[�̋ߐڍU���Ƃ̓����蔻��
	if (enemyHp > 0) {
		if (player->proximityAttackCollision(enemyPos)) {
			enemyHp -= 3;
			hpDisplay = true;
		}
	}

	//HP��`�悷��ۂ̏���
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}

	//HP���Ȃ��Ȃ莀�S�����ꍇ
	if (enemyHp == 0) {
		motionNum = 3;
	}
	
	if (motion->dead()) {
		isEnabled = false;
	}

	//�G�l�~�[�̓������X�V
	if (landing) {
		motion->update(motionNum);
	}
}

void Enemy::draw()
{
	DrawCircle(enemyPos.x, enemyPos.y, 400, 0xff0000, false);
	DrawString(enemyPos.x, enemyPos.y - 15, "�G", 0xffffff);

	DrawFormatString(0, 15, 0xffffff, "%d", enemyHp);

	motion->draw(enemyPos,handle,inversion);

	if (hpDisplay) {
		hp->draw(enemyPos);
	}
}

bool Enemy::isEnable() const
{
	return isEnabled;
}




