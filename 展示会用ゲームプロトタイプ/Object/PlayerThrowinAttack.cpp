#include "PlayerThrowinAttack.h"
#include "../game.h"
#include "../Pad.h"
#include <math.h>
#include "../field.h"

PlayerThrowinAttack::PlayerThrowinAttack()
{
}

void PlayerThrowinAttack::attack(const Vec2& playerPos, bool directions)
{
	pos = playerPos;
	isEnabled = true;
	//����
	playerDirections = directions;
}

bool PlayerThrowinAttack::isEnable() const
{
	return isEnabled;
}

void PlayerThrowinAttack::init()
{
	landingObject = false;
	isEnabled = false;
	vec = { 5.0f,0.0f };
	up = 0;
}

void PlayerThrowinAttack::update(Vec2 offset)
{
	flyingObjectPos = pos + offset;

	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {

			const int chipNo = FieldData::field[y][x];

			if (chipNo == 1) {
				if (filedCollision(y)) {
					landingObject = true;
				}
			}
		}
	}

	//���ĕ����n�ʂɒ��n�����Ƃ�or���Ē��̎�
	/*if (pos.y >= 680) {
		vec = { 0.0f,0.0f };
		
		pos.y = 680;
	}*/
	
	if (!landingObject) {
		x = pos.y + r + cos(45);
		y = pos.y + r + sin(45);

		normalize.x = x;
		normalize.y = -y;

		vec = normalize.normalize() * 2;

		up += 0.3f;

		vec.y += up;

		vec.x *= 15;
	}

	//�v���C���[�̌��������ɂ����object���΂�������ω�������
	if (!landingObject) {
		if (playerDirections) {
			pos.x -= vec.x;
			pos.y += vec.y;
		}
		else if (!playerDirections) {
			pos += vec;
		}
	}

	//�͈͊O�ŏ�����
	if (pos.x < -30) {
		init();
	}
	if (pos.x - 20 > Game::kScreenWidth * 2) {
		init();
	}
	if (pos.y < 0) {
		init();
	}
	if (pos.y > Game::kScreenHeight) {
		init();
	}

}

void PlayerThrowinAttack::draw()
{
	DrawBox(flyingObjectPos.x, flyingObjectPos.y, flyingObjectPos.x + 20, flyingObjectPos.y + 20, GetColor(252, 15, 192), true);
	DrawString(flyingObjectPos.x, flyingObjectPos.y - 10, "�e", 0xffffff);
}


//�v���C���[�Ƃ̓����蔻��
bool PlayerThrowinAttack::playerCollision(const Vec2& pos, Vec2 offset)
{
	float playerLeft = pos.x + offset.x;
	float playerTop = pos.y + offset.y;
	float playerRight = pos.x + 50 + offset.x;
	float playerBottom = pos.y + 74 + offset.y;

	if (playerLeft > flyingObjectPos.x + 20)	return false;
	if (playerRight < flyingObjectPos.x)		return false;
	if (playerTop >= flyingObjectPos.y + 20)	return false;
	if (playerBottom <= flyingObjectPos.y)		return false;

	return true;
}


//�������Ƃ̓����蔻��
bool PlayerThrowinAttack::enemyCollision(const Vec2& pos, Vec2 offset)
{
	
	if (isEnabled) {
		if (!landingObject) {
			float enemyLeft = pos.x + offset.x;
			float enemyTop = pos.y + offset.y;
			float enemyRight = pos.x + 30 + offset.x;
			float enemyBottom = pos.y + 30 + offset.y;

			if (enemyLeft > flyingObjectPos.x + 20)		return false;
			if (enemyRight < flyingObjectPos.x)			return false;
			if (enemyTop >= flyingObjectPos.y + 20)		return false;
			if (enemyBottom <= flyingObjectPos.y)		return false;

			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

}


void PlayerThrowinAttack::deadFlyingObject()
{
	init();
}

bool PlayerThrowinAttack::filedCollision(int y)
{
	float flyingObjectTop = flyingObjectPos.y;
	float flyingObjectBottom = flyingObjectPos.y + 24;

	float filedTop = y * FieldData::chipSize;
	float filedBottom = y * FieldData::chipSize + FieldData::chipSize;

	if (flyingObjectBottom < filedTop)return false;
	if (flyingObjectTop > filedBottom)return false;

	return true;
}