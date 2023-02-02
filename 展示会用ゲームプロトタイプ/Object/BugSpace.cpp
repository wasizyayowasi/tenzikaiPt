#include "BugSpace.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "ObjectHp.h"
#include "../Pad.h"

BugSpace::BugSpace(int spaceNum):num(spaceNum)
{
	hp = new ObjectHp;
	hp->setObjectMaxHp(maxHp);
	isEnabled = true;
	for (auto& enemy : enemy_) {
		enemy = std::make_shared<Enemy>();
	}
}

//バグスペースの更新
void BugSpace::update()
{
	hp->setObjectHp(maxHp);

	if (Pad::isPress(PAD_INPUT_5)) {
		if (!pushKey) {
			pushKey = true;
			for (auto& enemy : enemy_) {
				if (!enemy->isEnable()) {
					enemy->dispatch(spacePos);
					break;
				}
			}
		}
	}
	else {
		pushKey = false;
	}
	

	for (auto& enemy : enemy_) {
		if (enemy->isEnable()) {
			enemy->update();
		}
	}

	if (player->repairSpace(spacePos)) {
		player->setRepair(1);
		if (player->returnSpaceHpDisplay()) {
			maxHp--;
		}
	}

	if (maxHp < 0) {
		isEnabled = false;
	}
	
}

void BugSpace::draw()
{
	DrawBox(spacePos.x, spacePos.y, spacePos.x + 50, spacePos.y + 60, GetColor(255, 0, 0), true);
	DrawString(spacePos.x, spacePos.y, "空間", 0xffffff);
	

	if (player->repairSpace(spacePos)) {
		if (player->returnSpaceHpDisplay()) {
			hp->draw(spacePos);
		}
	}

	for (auto& enemy : enemy_) {
		if (enemy->isEnable()) {
			enemy->draw();
		}
	}
}

void BugSpace::enemySetPlayer()
{
	for (auto& enemy : enemy_) {
		enemy->setPlayer(player);
	}
}

bool BugSpace::isEnable() const
{
	return isEnabled;
}
