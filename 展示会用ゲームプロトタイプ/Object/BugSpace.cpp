#include "BugSpace.h"
#include "DxLib.h"
#include "Enemy.h"
#include "../Pad.h"

BugSpace::BugSpace()
{
	for (auto& enemy : enemy_) {
		enemy = std::make_shared<Enemy>();
	}
}

void BugSpace::update()
{
	
	if (Pad::isPress(PAD_INPUT_5)) {
		if (!pushKey) {
			pushKey = true;
			for (auto& enemy : enemy_) {
				if (!enemy->isEnable()) {
					enemy->dispatch(spacePos);
					a++;
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

}

void BugSpace::draw()
{
	DrawFormatString(0, 15, GetColor(255, 255, 255), "%d", a);
	DrawBox(spacePos.x, spacePos.y, spacePos.x + 50, spacePos.y + 60, GetColor(255, 0, 0), true);
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
