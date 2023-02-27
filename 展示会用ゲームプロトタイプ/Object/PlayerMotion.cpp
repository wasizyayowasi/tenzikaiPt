#include "PlayerMotion.h"
#include "../DrawFunctions.h"
#include "DxLib.h"

 int frameSpeed = 5;
constexpr int assetSizeX = 115;
constexpr int assetSizeY = 84;


PlayerMotion::PlayerMotion()
{
	func = &PlayerMotion::idle;
}

PlayerMotion::~PlayerMotion()
{
}

void PlayerMotion::update(int num)
{
	imgX = (imgX + 1) % (frameSpeed * maxImgX);

	switch (num) {
	case 0:
		func = &PlayerMotion::idle;
		imgY = 0;
		maxImgX = 8;
		break;
	case 1:
		func = &PlayerMotion::walk;
		imgY = 1;
		maxImgX = 8;
		break;
	case 2:
		func = &PlayerMotion::ascend;
		imgY = 16;
		maxImgX = 7;
		break;
	case 3:
		func = &PlayerMotion::attack;
		imgY = 8;
		maxImgX = 3;
		break;
	case 4:
		func = &PlayerMotion::death;
		if (imgY != 4) {
			imgX = 0;
		}
		imgY = 4;
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

//‘Ò‹@
void PlayerMotion::idle()
{
	frameSpeed = 8;
}

//•à‚­
void PlayerMotion::walk()
{
	frameSpeed = 8;
}

//“o‚é
void PlayerMotion::ascend()
{
	frameSpeed = 8;
}

//‰º‚é
void PlayerMotion::getOff()
{
	frameSpeed = 8;
	imgX = (imgX - 1) % (frameSpeed * maxImgX);
}

//‹ßÚUŒ‚
void PlayerMotion::attack()
{
	frameSpeed = 15;
	if (imgX - 1 > maxImgX) {
		imgY++;
	}
}

void PlayerMotion::damege()
{
	frameSpeed = 8;
	
}

void PlayerMotion::death()
{
	frameSpeed = 10;
	if (imgY == 4 && newImgX == 1150) {
		isEnabled = true;
	}
}

void PlayerMotion::draw(Vec2 playerPos, int handle, bool trun, Vec2 offset)
{
	Vec2 pos = playerPos + offset;
	newImgX = (imgX / frameSpeed) * assetSizeX;
	my::myDrawRectRotaGraph(pos.x + 15, pos.y + 15, newImgX, imgY * assetSizeY, assetSizeX, assetSizeY, 2.0f, 0.0f, handle, true, trun);
}


