#include "ObjectHp.h"
#include "DxLib.h"
#include "../game.h"

ObjectHp::ObjectHp()
{
	deathHpHandle = LoadGraph("data/objectGraph/heart2.png");
}

ObjectHp::~ObjectHp()
{
}

void ObjectHp::setObjectHp(float obJectHp)
{
	hp = obJectHp;
	displayHp = hp;
}

void ObjectHp::setWaveHp(int obJectMaxHp)
{
	maxHp = obJectMaxHp;
	magnification = 1000 / maxHp;
}

void ObjectHp::setObjectMaxHp(int obJectMaxHp)
{
	maxHp = obJectMaxHp;
	tempHp = maxHp;
	magnification = 1000 / maxHp;
}

//hp‚Ì•\Ž¦
void ObjectHp::draw(Vec2 pos, Vec2 offset)
{
	pos = pos + offset;

	DrawBox(pos.x - 2, pos.y - 25, pos.x + maxHp * 10 + 2, pos.y - 13, 0x999999, true);
	DrawBox(pos.x, pos.y - 23, pos.x + maxHp * 10, pos.y - 15, 0x000000, true);
	if (static_cast<float>(hp) < tempHp) {
		tempHp -= decreaseHp;
		decreaseHp += 0.03f;
	}
	if (tempHp <= hp) {
		decreaseHp = 0.08f;
	}
	if (tempHp < 0.0f) {
		tempHp = 0.0f;
	}
	DrawBoxAA(pos.x, pos.y - 23.0f, pos.x + tempHp * 10, pos.y - 15.0f, 0xDAC290, true);
	DrawBoxAA(pos.x, pos.y - 23, pos.x + hp * 10, pos.y - 15, 0x9B003F, true);
}

void ObjectHp::playerHpDraw(int handle)
{
	int increaseX = 0;
	for (int i = 0; i < maxHp; i++) {
		DrawRotaGraph(Game::kScreenWidth / 2 + 122 + increaseX, 998, 2.0f, 0.0f, deathHpHandle, true);
		increaseX += 26;
	}
	increaseX = 0;
	for (int i = 0; i < hp; i++) {
		DrawRotaGraph(Game::kScreenWidth / 2 + 122 + increaseX, 998, 2.0f, 0.0f, handle, true);
		increaseX += 26;
	}
}

void ObjectHp::waveHpDraw(Vec2 pos)
{
	DrawBox(pos.x, pos.y - 30, pos.x + maxHp * magnification, pos.y - 10, 0x000000, true);
	if (displayHp <= tempHp) {
		tempHp -= decreaseHp;
		decreaseHp += 0.03f;
	}
	if (tempHp <= displayHp) {
		decreaseHp = 0.08f;
	}
	DrawBoxAA(pos.x, pos.y - 30, pos.x + displayHp * magnification, pos.y - 10, 0x9B003F, true);

}

bool ObjectHp::chargeHp()
{
	if (displayHp <= maxHp) {
		displayHp += 0.1f;
		tempHp += 0.1f;
		return false;
	}
	return true;
}
