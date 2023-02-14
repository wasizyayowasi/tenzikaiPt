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
		maxImgX = 7;
		break;
	case 1:
		func = &PlayerMotion::walk;
		imgY = 1;
		maxImgX = 7;
		break;
	case 2:
		func = &PlayerMotion::ascend;
		imgY = 16;
		maxImgX = 7;
		break;
	case 3:
		func = &PlayerMotion::attack;
		maxImgX = 3;
		break;
	case 4:
		func = &PlayerMotion::death;
		maxImgX = 11;
		break;
	case 5:
		func = &PlayerMotion::damege;
		imgY = 3;
		maxImgX = 3;
		break;
	}
	(this->*func)();
}

//ë“ã@
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

//ï‡Ç≠
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

//ìoÇÈ
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

//â∫ÇÈ
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

//ãﬂê⁄çUåÇ
void PlayerMotion::attack()
{

	if (!(imgY == 8 || imgY == 9 || imgY == 10)) {
		imgY = 8;
	}
	if (--motionTime < 0) {
		imgX++;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
		imgY++;
	}
	if (imgY > 10) {
		imgY = 8;
	}

}

void PlayerMotion::damege()
{
	if (--motionTime < 0) {
		imgX++;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
	}
}

void PlayerMotion::death()
{
	imgY = 4;
	if (--motionTime < 0) {
		imgX++;
		motionTime = 4;
	}
	if (imgX > maxImgX) {
		isEnabled = true;
	}
}

void PlayerMotion::draw(Vec2 playerPos, int handle, bool trun)
{
	my::myDrawRectRotaGraph(playerPos.x + 15, playerPos.y + 15, imgX * assetSizeX, imgY * assetSizeY, assetSizeX, assetSizeY, 2.0f, 0.0f, handle, true, trun);
}


