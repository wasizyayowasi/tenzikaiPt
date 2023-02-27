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
	void playerHpDraw(int handle);

	int returnMaxHp() { return maxHp; }
private:
	
	int deathHpHandle = -1;

	int hp = 0;
	int maxHp = 0;
};

