#include "Player.h"
#include "../Pad.h"
#include "TimeLimit.h"
#include "DxLib.h"
#include "TimeLimit.h"
#include "PlayerThrowinAttack.h"
#include "ObjectHp.h"

namespace {
	constexpr int playerSizeX = 50;
	constexpr int playerSizeY = 64;

	constexpr int groundY = 700;
}

Player::Player()
{
	limit = new TimeLimit;
	hp = new ObjectHp;
	flyingObject = std::make_shared<PlayerThrowinAttack>();
	hp->setObjectMaxHp(playerHp);
}

void Player::update()
{
	

	hp->setObjectHp(playerHp);

	if (!ladder) {
		if (upperLimit) {
			vec.y = 20.0f;
		}
		else {
			vec.y = 10.0f;
		}
		PlayerPos += vec;
	}

	if (Pad::isPress(PAD_INPUT_UP)) {
		if (!ladder) {
			upperLimit = true;
		}
	}

	//�ړ�
	if (Pad::isPress(PAD_INPUT_LEFT)) {
		PlayerPos.x -= 10;
		playerDirections = 1;
	}
	if (Pad::isPress(PAD_INPUT_RIGHT)) {
		PlayerPos.x += 10;
		playerDirections = 2;
	}
	if (ladder) {
		if (!upperLimit) {
			if (Pad::isPress(PAD_INPUT_UP)) {
				PlayerPos.y -= 10;
			}
		}
	}
	
	if (!hit) {
		if (Pad::isPress(PAD_INPUT_DOWN)) {
			PlayerPos.y += 10;
		}
	}

	//�n�ʂƂ̔���
	if (PlayerPos.y + playerSizeY > groundY) {
		PlayerPos.y = groundY - playerSizeY;
		hit = true;
		upperLimit = false;
	}
	else {
		hit = false;
	}


	//�C���^�C�}�[
	if (!flyingObject->isEnable()) {
		if (repair == 1) {
			if (Pad::isPress(PAD_INPUT_1)) {
				limit->update();
				timeDisplay = true;
			}
			else {
				timeDisplay = false;
			}
		}
	}


	//�ȉ��U����i�����\�� 
	//����
	if (Pad::isTrigger(PAD_INPUT_3)) {
		if (!flyingObject->isEnable()) {
			flyingObject->attack(PlayerPos,playerDirections);
		}
	}
	if (flyingObject->isEnable()) {
		flyingObject->update();
	}
	if (flyingObject->landing()) {
		if (flyingObject->playerCollision(PlayerPos)) {
			flyingObject->deadFlyingObject();
		}
	}


	//�ߐڍU��
	if (!flyingObject->isEnable()) {
		if (Pad::isTrigger(PAD_INPUT_2)) {
			proximityAttack = true;
		}
		else {
			proximityAttack = false;
		}
	}

	
	//HP�̕\��
	if (hpDisplay) {
		if (--hpDisplayTime < 0) {
			hpDisplay = false;
			hpDisplayTime = 120;
		}
	}
	
	limit->playerGetPos(PlayerPos);

	repair = 0;

}

//�`��
void Player::draw()
{
	//�ߐڍU��
	if (proximityAttack) {
		if (playerDirections == 1) {
			DrawBox(PlayerPos.x - 65, PlayerPos.y + 10, PlayerPos.x - 5, PlayerPos.y + 50, 0xff00ff, true);
			DrawString(PlayerPos.x - 65, PlayerPos.y + 10, "�ߐڍU��", 0x000000);
		}
		else {
			DrawBox(PlayerPos.x + 55, PlayerPos.y + 10, PlayerPos.x + 115, PlayerPos.y + 50, 0xff00ff, true);
			DrawString(PlayerPos.x + 55, PlayerPos.y + 10, "�ߐڍU��", 0x000000);
		}
	}
	
	if (!push) {
		if (playerDirections == 1) {
			DrawBox(PlayerPos.x - 20, PlayerPos.y, PlayerPos.x, PlayerPos.y + 10, GetColor(255, 0, 0), true);
			DrawString(PlayerPos.x - 20, PlayerPos.y - 15, "����", 0xffffff);
		}
		else if (playerDirections == 2) {
			DrawBox(PlayerPos.x + 50, PlayerPos.y, PlayerPos.x + 70, PlayerPos.y + 10, GetColor(255, 0, 0), true);
			DrawString(PlayerPos.x + 50, PlayerPos.y - 15, "����", 0xffffff);
		}
	}
	
	DrawBox(PlayerPos.x, PlayerPos.y, PlayerPos.x + playerSizeX, PlayerPos.y + playerSizeY, GetColor(255, 255, 255), true);
	DrawString(PlayerPos.x, PlayerPos.y + 30, "�v���C���[", 0xff00ff);

	//�^�C�}�[
	if (!flyingObject->isEnable()) {
		if (timeDisplay) {
			limit->draw();
		}
	}

	//��ѓ���
	if (flyingObject->isEnable()) {
		flyingObject->draw();
	}

	if (hpDisplay) {
		hp->draw(PlayerPos);
	}
}

//�B���
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


//�ߐڍU���̓����蔻��
bool Player::proximityAttackCollision(const Vec2& pos)
{
	float enemyLeft = pos.x;
	float enemyTop = pos.y;
	float enemyRight = pos.x + 30;
	float enemyBottom = pos.y + 30;

	if (proximityAttack) {
		if (playerDirections == 1) {
			if (enemyLeft > PlayerPos.x - 5)			return false;
			DrawString(600, 0, "aiu1", 0xffffff);
			if (enemyRight < PlayerPos.x - 65)			return false;
			DrawString(600, 15, "aiu1", 0xffffff);
			if (enemyTop <= PlayerPos.y + 10)			return false;
			DrawString(600, 30, "aiu1", 0xffffff);
			if (enemyBottom <= PlayerPos.y + 50)		return false;
			DrawString(600, 45, "aiu1", 0xffffff);
		}
		else if (playerDirections == 2) {
			if (enemyLeft > PlayerPos.x + 115)			return false;
			DrawString(600, 0, "aiu1", 0xffffff);
			if (enemyRight < PlayerPos.x + 55)			return false;
			DrawString(600, 15, "aiu1", 0xffffff);
			if (enemyTop <= PlayerPos.y + 10)			return false;
			DrawString(600, 30, "aiu1", 0xffffff);
			if (enemyBottom <= PlayerPos.y + 50)		return false;
			DrawString(600, 45, "aiu1", 0xffffff);
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

	if (spaceLeft > PlayerPos.x + 50)				return false;
	DrawString(600, 0, "aiu1", 0xffffff);
	if (spaceRight < PlayerPos.x)					return false;
	DrawString(600, 15, "aiu1", 0xffffff);
	if (spaceTop > PlayerPos.y + 64)				return false;
	DrawString(600, 30, "aiu1", 0xffffff);
	if (spaceBottom < PlayerPos.y)					return false;
	DrawString(600, 45, "aiu1", 0xffffff);
		
	return true;

}

void Player::setRepair(/*bool temporaryRepair*/int num)
{
	
	repair = num;
	if (repair) {
		DrawString(700, 0, "�������", 0xffffff);
	}
	else {
		DrawString(700, 15, "�����Ȃ���", 0xffffff);
	}
}
