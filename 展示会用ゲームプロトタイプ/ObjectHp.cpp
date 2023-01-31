#include "ObjectHp.h"
#include "DxLib.h"

//hp‚Ì•\Ž¦
void ObjectHp::draw(Vec2 pos)
{
	if (hp > 0) {
		DrawBox(pos.x, pos.y - 30, pos.x + maxHp * 10, pos.y - 10, GetColor(100, 100, 255), true);
		DrawBox(pos.x, pos.y - 25, pos.x + hp * 10, pos.y - 15, GetColor(255, 100, 100), true);
	}
}
