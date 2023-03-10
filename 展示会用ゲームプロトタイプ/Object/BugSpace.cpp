#include "BugSpace.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../Pad.h"
#include "../field.h"
#include "../DrawFunctions.h"

BugSpace::BugSpace()
{
	spaceHandle = my::myLoadGraph("data/objectGraph/portal2.png");

}

BugSpace::~BugSpace()
{
	DeleteGraph(spaceHandle);
	delete hp;
}

std::array<std::shared_ptr<Enemy>, enemyNum>& BugSpace::getEnemy()
{
	return enemy;
}

void BugSpace::init(int setX, int setY)
{
	maxHp = 10;
	hp = new ObjectHp;
	hp->setObjectMaxHp(maxHp);
	isEnabled = true;
	for (auto& enemy : enemy) {
		enemy = std::make_shared<Enemy>(0);
	}

	spacePos.x = setX * chipSize;
	spacePos.y = setY * chipSize;
}

void BugSpace::tutorialUpdate(Vec2 offset)
{
	if (player->repairSpace(spacePos, offset)) {
		if (player->returnSpaceHpDisplay()) {
			if (--time < 0) {
				player->setMotion(true);
				maxHp--;
				time = 10;
			}
		}
		else {
			player->setMotion(false);
		}
	}


	hp->setObjectHp(maxHp);


	/*if (--enemySpawnTime < 0) {
		for (auto& enemy : enemy) {
			if (!enemy->isEnable()) {
				enemy->dispatch({ spacePos.x + 50,spacePos.y + 40 });
				enemySpawnTime = enemySpawnInterval;
				enemySpawnInterval -= 10;
				break;
			}
		}
	}*/

	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			enemy->update(offset);
		}
	}

	if (maxHp < 1) {
		player->consumption();
		isEnabled = false;
		player->setMotion(false);
	}
}

//バグスペースの更新
void BugSpace::update(Vec2 offset)
{

	if (player->repairSpace(spacePos, offset)) {
		if (player->returnSpaceHpDisplay()) {
			if (--time < 0) {
				player->setMotion(true);
				maxHp--;
				time = 10;
			}
		}
		else {
			player->setMotion(false);
		}
	}
	

	hp->setObjectHp(maxHp);

	if (!player->returnHpDisplay()) {
		if (--enemySpawnTime < 0) {
			for (auto& enemy : enemy) {
				if (!enemy->isEnable()) {
					enemy->dispatch({ spacePos.x + 50,spacePos.y + 40 });
					enemySpawnTime = enemySpawnInterval;
					enemySpawnInterval -= 20;
					break;
				}
			}
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
		player->setMotion(false);
	}
}

void BugSpace::draw(Vec2 offset)
{
	imgX++;
	if (imgX > 6) {
		imgY++;
		imgX = 0;
	}
	if (imgY == 5 && imgX > 5) {
		imgY = 0;
		imgX = 0;
	}

	DrawRectRotaGraph(spacePos.x + offset.x + 50, spacePos.y + offset.y + 50, imgX * 100, imgY * 100, 100, 100, 3.0f, 0.0f, spaceHandle, true, false);

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
