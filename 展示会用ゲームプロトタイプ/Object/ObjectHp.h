#pragma once
#include "../Vec2.h"

class ObjectHp
{
public:
	ObjectHp();
	virtual ~ObjectHp();

	void setObjectHp(int obJectHp);
	void setObjectMaxHp(int obJectMaxHp) { maxHp = obJectMaxHp,tempHp = maxHp; }

	void draw(Vec2 pos,Vec2 offset);
	void playerHpDraw(int handle);
	void waveHpDraw(Vec2 pos);

	int returnMaxHp() { return maxHp; }
	float returnTempHp() { return tempHp; }
private:
	
	float decreaseHp = 0.08f;

	float tempHp = 0.0f;
	int deathHpHandle = -1;

	int hp = 0;
	int maxHp = 0;
};

