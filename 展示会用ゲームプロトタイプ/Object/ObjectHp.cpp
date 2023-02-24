#include "ObjectHp.h"
#include "DxLib.h"
#include "../game.h"

ObjectHp::ObjectHp()
{
	hpHandle = LoadGraph("data/heart.png");
	deathHpHandle = LoadGraph("data/heart2.png");
}

ObjectHp::~ObjectHp()
{
	DeleteGraph(hpHandle);
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

void ObjectHp::playerHpDraw()
{
	int increaseX = 0;
	for (int i = 0; i < hp; i++) {
		DrawRotaGraph(Game::kScreenWidth / 2 + 122 + increaseX, 998, 2.0f, 0.0f, hpHandle, true);
		increaseX += 26;
	}
	int decreaseX = 1316;
	for (int i = maxHp - hp; i > 0; i--) {
		DrawRotaGraph(decreaseX, 998, 2.0f, 0.0f, deathHpHandle, true);
		decreaseX -= 26;
	}
}
