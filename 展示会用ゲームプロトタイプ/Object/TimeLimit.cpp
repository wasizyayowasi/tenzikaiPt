#include "TimeLimit.h"
#include "DxLib.h"

TimeLimit::TimeLimit()
{

}

void TimeLimit::update()
{
	maxX -= 10;
	if (maxX < x) {
		maxX = 500 ;
	}
}

void TimeLimit::draw()
{
	DrawBox(playerPos.x + 100, playerPos.y - 50, playerPos.x + 510, playerPos.y - 20, GetColor(255, 255, 255), true);
	DrawBox(playerPos.x + 105, playerPos.y - 45, playerPos.x + maxX, playerPos.y - 25, GetColor(255, 0, 0), true);
}

