#pragma once
#include "Vec2.h"

class PlayerMotion
{
public:
	PlayerMotion();
	~PlayerMotion();

	void update(int num);

	void idle();
	void walk();
	void ascend();
	void getOff();
	void attack();
	void damege();
	void death();

	void draw(Vec2 playerPos,int handle,bool trun);

	int setDead() { return isEnabled; }
private:
	int imgX = 0;
	int imgY = 0;

	bool isEnabled = false;

	int maxImgX = 7;

	int motionTime = 4;

	int updateTime = 4;

	int assetSizeX = 115;
	int assetSizeY = 84;
private:
	void(PlayerMotion::* func)();
};

