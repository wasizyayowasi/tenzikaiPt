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
		DrawBox(pos.x, pos.y - 30, pos.x + maxHp * 10, pos.y - 10, GetColor(100, 100, 255), true);
		DrawBox(pos.x, pos.y - 25, pos.x + hp * 10, pos.y - 15, GetColor(255, 100, 100), true);
	}
}

void ObjectHp::playerHpDraw()
{
	int increaseX = 0;
	for (int i = 0; i < hp; i++) {
		DrawRotaGraph(Game::kScreenWidth / 2 + 122 + increaseX, 990, 2.0f, 0.0f, hpHandle, true);
		increaseX += 26;
	}
	int decreaseX = 1316;
	for (int i = maxHp - hp; i > 0; i--) {
		DrawRotaGraph(decreaseX, 990, 2.0f, 0.0f, deathHpHandle, true);
		decreaseX -= 26;
	}
}
