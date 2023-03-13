#pragma once
#include "../Vec2.h"

class ObjectHp
{
public:
	ObjectHp();
	virtual ~ObjectHp();

	void setObjectHp(int obJectHp);
	void setWaveHp(int obJectMaxHp);
	void setObjectMaxHp(int obJectMaxHp);

	void draw(Vec2 pos,Vec2 offset);
	void playerHpDraw(int handle);
	void waveHpDraw(Vec2 pos);

	int returnMaxHp() { return maxHp; }
	float returnTempHp() { return tempHp; }

	bool chargeHp();
private:
	int magnification = 0;

	float decreaseHp = 0.08f;

	float tempHp = 0.0f;
	int deathHpHandle = -1;

	int hp = 0;
	float displayHp = 0.0f;
	int maxHp = 0;
};

