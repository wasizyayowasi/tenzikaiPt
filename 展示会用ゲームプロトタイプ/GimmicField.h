#pragma once

#include "game.h"
#include "Vec2.h"

class GimmicField
{
public:
	GimmicField();
	virtual ~GimmicField();

	void update();
	void draw(Vec2 offset);

	bool examinationChip(int y, int x);

	int chipNumX(int width);
	int chipNumY(int height);
	int chipNum(int width, int height);
private:
	int groundHandle = -1;

	int gGroundGraphWidth = 0;
	int gGroundGraphHeight = 0;
};
