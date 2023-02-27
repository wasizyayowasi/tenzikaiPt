#include "BugSpace.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../Pad.h"
#include "../field.h"

BugSpace::BugSpace(int spaceNum):num(spaceNum)
{
	hp = new ObjectHp;
	hp->setObjectMaxHp(maxHp);
	isEnabled = true;
	for (auto& enemy : enemy) {
		enemy = std::make_shared<Enemy>();
	}
}

std::array<std::shared_ptr<Enemy>, 16>& BugSpace::getEnemy()
{
	return enemy;
}

void BugSpace::init(int setX, int setY)
{
	if (num == 0) {
		spacePos.x = setX * chipSize;
		spacePos.y = setY * chipSize;
	}
	else if (num == 1) {
		spacePos.x = setX * chipSize;
		spacePos.y = setY * chipSize;
	}
	else if (num == 2) {
		spacePos.x = setX * chipSize;
		spacePos.y = setY * chipSize;
	}
}

//バグスペースの更新
void BugSpace::update(Vec2 offset)
{

	if (player->repairSpace(spacePos, offset)) {
		DrawString(600, 0, "siu", 0xffffff);
		if (player->returnSpaceHpDisplay()) {
			if (--time < 0) {
				maxHp--;
				time = 10;
			}
		}
	}
	

	hp->setObjectHp(maxHp);

	
	if (--enemySpawnTime < 0) {
		for (auto& enemy : enemy) {
			if (!enemy->isEnable()) {
				enemy->dispatch(spacePos);
				enemySpawnTime = enemySpawnInterval;
				enemySpawnInterval -= 10;
				break;
			}
		}
	}
	
	

	//デバッグ用
	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			DrawString(0, 0, "aiu", 0xffffff);
		}
	}

	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			enemy->update(offset);
		}
	}

	if (maxHp < 1) {
		player->consumption();
		isEnabled = false;
	}
}

void BugSpace::draw(Vec2 offset)
{
	
	DrawBox(spacePos.x + offset.x, spacePos.y + offset.y, spacePos.x + chipSize * 3 + offset.x, spacePos.y + chipSize * 3 + offset.y, 0xff0000, true);
	DrawFormatString(200, 0, 0xffffff, "%f : %f", enemyDeathPos.x, enemyDeathPos.y);
	if (player->repairSpace(spacePos,offset)) {
		if (player->returnSpaceHpDisplay()) {
			hp->draw({ spacePos.x,spacePos.y - 30 },offset);
		}
	}

	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			enemy->draw(offset);
		}
	}

	DrawFormatString(500, 0, 0xffffff, "%d", maxHp);
	
}

void BugSpace::move(int scrollX)
{
	spacePos.x += scrollX;
}

void BugSpace::enemySetPlayer(int handle,int coinHandle)
{
	for (auto& enemy : enemy) {
		enemy->setPlayer(player, handle,coinHandle);
	}
}

bool BugSpace::isEnable() const
{
	return isEnabled;
}

Vec2 BugSpace::setCoinPos()
{
	for (auto& enemy : enemy) {
		if (enemy->returnDeath()) {
			enemyDeathPos = enemy->deadPos();
			break;
		}
	}
	return enemyDeathPos;
}
