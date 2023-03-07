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

void ObjectHp::setObjectHp(int obJectHp)
{
	hp = obJectHp;
	if (tempHp == hp) {
		tempHp = hp;
	}
}

//hp‚Ì•\Ž¦
void ObjectHp::draw(Vec2 pos, Vec2 offset)
{
	pos = pos + offset;

	DrawBox(pos.x, pos.y - 23, pos.x + maxHp * 10, pos.y - 15, 0x000000, true);
	if (static_cast<float>(hp) < tempHp) {
		tempHp -= decreaseHp;
		decreaseHp += 0.03f;
	}
	if (tempHp < 0.0f) {
		tempHp = 0.0f;
		decreaseHp = 0.08f;
	}
	DrawBoxAA(pos.x, pos.y - 23.0f, pos.x + tempHp * 10, pos.y - 15.0f, 0xDAC290, true);
	DrawBox(pos.x, pos.y - 23, pos.x + hp * 10, pos.y - 15, 0x9B003F, true);
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
	DrawBox(pos.x, pos.y - 23, pos.x + maxHp * 100, pos.y - 10, 0x000000, true);
	if (static_cast<float>(hp) < tempHp) {
		tempHp -= decreaseHp;
		decreaseHp += 0.03f;
	}
	if (tempHp < 0.0f) {
		tempHp = 0.0f;
		decreaseHp = 0.08f;
	}
	DrawBoxAA(pos.x, pos.y - 30.0f, pos.x + tempHp * 100, pos.y - 10.0f, 0xDAC290, true);
	DrawBox(pos.x, pos.y - 30, pos.x + hp * 100, pos.y - 10, 0x9B003F, true);
}
