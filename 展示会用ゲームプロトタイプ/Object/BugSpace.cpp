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
	breakSound = LoadSoundMem("data/soundEffect/break.mp3");
	breakSound2 = LoadSoundMem("data/soundEffect/break2.mp3");
}

BugSpace::~BugSpace()
{
	DeleteGraph(spaceHandle);
	DeleteSoundMem(breakSound);
	DeleteSoundMem(breakSound2);
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

	spacePos.x = static_cast<float>(setX * chipSize);
	spacePos.y = static_cast<float>(setY * chipSize);

	enemySpawnTime = 0;
	enemySpawnInterval = 600;

}

void BugSpace::tutorialUpdate(Vec2 offset)
{
	if (player->repairSpace(spacePos, offset)) {
		if (player->returnSpaceHpDisplay()) {
			if (--soundTime == 0) {
				ChangeVolumeSoundMem(150, breakSound);
				PlaySoundMem(breakSound, DX_PLAYTYPE_BACK);
				soundTime = 85;
			}
			player->setMotion(true);
			maxHp -= static_cast<float>(0.08);
		}
		else {
			player->setMotion(false);
		}
	}

	hp->setObjectHp(maxHp);

	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			enemy->update(offset);
		}
	}

	if (maxHp < 0) {
		ChangeVolumeSoundMem(150, breakSound2);
		PlaySoundMem(breakSound2, DX_PLAYTYPE_BACK);
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
			if (--soundTime == 0) {
				ChangeVolumeSoundMem(150, breakSound);
				PlaySoundMem(breakSound, DX_PLAYTYPE_BACK);
				soundTime = 85;
			}
			player->setMotion(true);
			maxHp -= static_cast<float>(0.008);
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
					enemySpawnInterval -= 30;
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

	if (maxHp < 0) {
		ChangeVolumeSoundMem(180, breakSound2);
		PlaySoundMem(breakSound2, DX_PLAYTYPE_BACK);
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

	DrawRectRotaGraphF(spacePos.x + offset.x + 50, spacePos.y + offset.y + 50, imgX * 100, imgY * 100, 100, 100, 3.0f, 0.0f, spaceHandle, true, false);

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
