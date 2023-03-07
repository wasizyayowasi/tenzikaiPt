#pragma once
#include "../Vec2.h"

class PlayerMotion
{
public:
	PlayerMotion();
	virtual ~PlayerMotion();

	void update(int num);

	void idle();
	void walk();
	void ascend();
	void getOff();
	void attack();
	void damege();
	void death();

	void draw(Vec2 playerPos,int handle,bool trun,Vec2 offset);
	void resuscitationDraw(Vec2 playerPos, int handle, bool trun);
	void deadDraw(Vec2 playerPos, int handle, bool trun);
	void deathDraw(Vec2 playerPos, int handle, bool trun);

	int setDead() { return isEnabled; }

	int returnMotionNum() { return imgX; }
	bool returnContinue() { return isContinue; }
private:
	int newImgX = 0;

	int deadImgX = 11;
	int deadImgY = 4;
	int deathImgX = 0;
	int deathImgY = 4;
	int time = 8;

	int imgX = 0;
	int imgY = 0;

	bool isEnabled = false;

	bool isContinue = false;;

	int maxImgX = 7;

	int motionTime = 4;

	int updateTime = 4;

private:
	void(PlayerMotion::* func)();
};

