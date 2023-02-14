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
	// TODO: return ステートメントをここに挿入します
	return enemy;
}

void BugSpace::init()
{
	if (num == 0) {
		spacePos.x = 24 * Field::chipSize;
		spacePos.y = 22 * Field::chipSize;
	}
	else if(num == 1) {
		spacePos.x = 6 * Field::chipSize;
		spacePos.y = 22 * Field::chipSize;
	}
	else if (num == 2) {
		spacePos.x = 20 * Field::chipSize;
		spacePos.y = 6 * Field::chipSize;
	}
}

//バグスペースの更新
void BugSpace::update()
{

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 4) {
				if (player->repairSpace(spacePos)) {
					if (player->returnSpaceHpDisplay()) {
						if (--time < 0) {
							maxHp--;
							time = 180;
						}
					}
				}
			}
		}
	}

	hp->setObjectHp(maxHp);

	if (Pad::isTrigger(PAD_INPUT_5)) {
		for (auto& enemy : enemy) {
			if (!enemy->isEnable()) {
				enemy->dispatch(spacePos);
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
			enemy->update();
		}
	}

	if (maxHp < 1) {
		isEnabled = false;
	}
}

void BugSpace::draw()
{
	DrawFormatString(spacePos.x, spacePos.y, 0xffffff, "空間:%d", num);
	DrawFormatString(0, 15, 0xffffff, "%d", time);

	DrawBox(spacePos.x, spacePos.y, spacePos.x + Field::chipSize * 3, spacePos.y + Field::chipSize * 3, 0xff0000, true);

	if (player->repairSpace(spacePos)) {
		if (player->returnSpaceHpDisplay()) {
			hp->draw({ spacePos.x,spacePos.y - 30 });
		}
	}

	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			enemy->draw();
		}
	}
}

void BugSpace::move(int scrollX)
{
	spacePos.x += scrollX;
}

void BugSpace::enemySetPlayer(int handle)
{
	for (auto& enemy : enemy) {
		enemy->setPlayer(player, handle);
	}
}

bool BugSpace::isEnable() const
{
	return isEnabled;
}
