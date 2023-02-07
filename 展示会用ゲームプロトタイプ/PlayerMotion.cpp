#include "PlayerMotion.h"
#include "DrawFunctions.h"

PlayerMotion::PlayerMotion()
{
	func = &PlayerMotion::idle;
}

PlayerMotion::~PlayerMotion()
{
}

void PlayerMotion::update(int num)
{
	switch (num) {
	case 0:
		func = &PlayerMotion::idle;
		imgY = 0;
		break;
	case 1:
		func = &PlayerMotion::walk;
		imgY = 1;
		break;
	case 2:
		func = &PlayerMotion::ascend;
		imgY = 16;
		break;
	}
	(this->*func)();
}

void PlayerMotion::idle()
{
	if (--motionTime < 0) {
		imgX++;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
	}
}

void PlayerMotion::walk()
{
	if (--motionTime < 0) {
		imgX++;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
	}
}

void PlayerMotion::ascend()
{
	if (--motionTime < 0) {
		imgX++;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
	}
}

void PlayerMotion::getOff()
{
	if (--motionTime < 0) {
		imgX--;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = maxImgX;
	}
}

void PlayerMotion::draw(Vec2 playerPos, int handle, bool trun)
{
	my::myDrawRectRotaGraph(playerPos.x + 15, playerPos.y + 15, imgX * assetSizeX, imgY * assetSizeY, assetSizeX, assetSizeY, 2.0f, 0.0f, handle, true, trun);
}


