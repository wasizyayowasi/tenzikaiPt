#include "BugSpace.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Player.h"
#include "../Pad.h"

BugSpace::BugSpace(int spaceNum):num(spaceNum)
{
	for (auto& enemy : enemy_) {
		enemy = std::make_shared<Enemy>();
	}
}

//バグスペースの更新
void BugSpace::update()
{
	
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
	}
}

void BugSpace::draw()
{
	DrawBox(spacePos.x, spacePos.y, spacePos.x + 50, spacePos.y + 60, GetColor(255, 0, 0), true);
	DrawString(spacePos.x, spacePos.y, "空間", 0xffffff);

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
