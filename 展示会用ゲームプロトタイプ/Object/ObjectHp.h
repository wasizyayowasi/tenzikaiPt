#pragma once
#include "../Vec2.h"

class ObjectHp
{
public:
	ObjectHp();
	virtual ~ObjectHp();

	void setObjectHp(float obJectHp);
	void setWaveHp(int obJectMaxHp);
	void setObjectMaxHp(float obJectMaxHp);

	void draw(Vec2 pos,Vec2 offset);
	void playerHpDraw(int handle);
	void waveHpDraw(Vec2 pos);

	int returnMaxHp() { return maxHp; }
	float returnTempHp() { return tempHp; }

	bool chargeHp(int num);
private:
	int magnification = 0;

	float decreaseHp = 0.08f;

	float tempHp = 0.0f;
	int deathHpHandle = -1;

	float hp = 0.0f;
	float displayHp = 0.0f;
	int maxHp = 0;
};

