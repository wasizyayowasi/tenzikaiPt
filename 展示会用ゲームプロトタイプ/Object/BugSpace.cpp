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
		spacePos = { 200.0f,600.0f };
	}
	else if (num == 1) {
		spacePos = { 200.0f,100.0f };
	}
	else {
		spacePos = { 800.0f,600.0f };
	}
}

//バグスペースの更新
void BugSpace::update()
{
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
	DrawFormatString(spacePos.x, spacePos.y, 0xffffff, "空間:%d", num);
	
	for (auto& enemy : enemy) {
		DrawFormatString(0, 45, 0xffffff, "%d", enemy->motionaiu());
	}

	if (player->repairSpace(spacePos)) {
		if (player->returnSpaceHpDisplay()) {
			hp->draw(spacePos);
		}
	}

	for (auto& enemy : enemy) {
		if (enemy->isEnable()) {
			enemy->draw();
		}
	}
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
