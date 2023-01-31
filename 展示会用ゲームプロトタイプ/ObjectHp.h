#pragma once
#include "Vec2.h"

class ObjectHp
{
public:
	ObjectHp();
	~ObjectHp();

	void setObjectHp(int obJectHp) { hp = obJectHp; }
	void setObjectMaxHp(int obJectMaxHp) { maxHp = obJectMaxHp; }

	void update();
	void draw(Vec2 pos);
private:
	int hp = 0;
	int maxHp = 0;
};

