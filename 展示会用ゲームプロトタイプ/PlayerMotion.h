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

	void draw(Vec2 playerPos,int handle,bool trun);

	
private:
	int imgX = 0;
	int imgY = 0;

	int maxImgX = 7;

	int motionTime = 4;

	int updateTime = 4;

	int assetSizeX = 115;
	int assetSizeY = 84;
private:
	void(PlayerMotion::* func)();
};

