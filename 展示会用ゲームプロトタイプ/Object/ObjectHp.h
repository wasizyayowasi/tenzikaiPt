#pragma once
#include "../Vec2.h"

class ObjectHp
{
public:
	ObjectHp();
	virtual ~ObjectHp();

	void setObjectHp(int obJectHp) { hp = obJectHp; }
	void setObjectMaxHp(int obJectMaxHp) { maxHp = obJectMaxHp; }

	void draw(Vec2 pos,Vec2 offset);
	void playerHpDraw();
private:
	int hpHandle = -1;
	int deathHpHandle = -1;

	int hp = 0;
	int maxHp = 0;
};

