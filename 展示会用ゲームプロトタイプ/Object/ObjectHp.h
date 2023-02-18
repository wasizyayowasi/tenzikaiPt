#pragma once
#include "../Vec2.h"

class ObjectHp
{
public:
	ObjectHp(){}
	~ObjectHp(){}

	void setObjectHp(int obJectHp) { hp = obJectHp; }
	void setObjectMaxHp(int obJectMaxHp) { maxHp = obJectMaxHp; }

	void draw(Vec2 pos,Vec2 offset);
private:
	int hp = 0;
	int maxHp = 0;
};

