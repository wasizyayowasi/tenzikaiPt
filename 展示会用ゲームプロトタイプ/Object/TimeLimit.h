#pragma once
#include "../Vec2.h"

class TimeLimit
{
public:
	TimeLimit();
	~TimeLimit() {}

	void update();
	void draw();

	Vec2 playerGetPos(Vec2 pos) { return playerPos = pos; }

private:
	int x = 200,y = 600;
	int maxX = 500;

	Vec2 playerPos;
};

