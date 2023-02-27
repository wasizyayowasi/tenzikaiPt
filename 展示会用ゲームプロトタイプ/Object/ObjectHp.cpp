#include "ObjectHp.h"
#include "DxLib.h"
#include "../game.h"

ObjectHp::ObjectHp()
{
	deathHpHandle = LoadGraph("data/heart2.png");
}

ObjectHp::~ObjectHp()
{
	
}

//hp‚Ì•\Ž¦
void ObjectHp::draw(Vec2 pos, Vec2 offset)
{
	pos = pos + offset;

	if (hp > 0) {
		DrawBox(pos.x, pos.y - 23, pos.x + maxHp * 10, pos.y - 20, 0x000000, true);
		DrawBox(pos.x, pos.y - 23, pos.x + hp * 10, pos.y - 20, 0x9B003F, true);
	}
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
