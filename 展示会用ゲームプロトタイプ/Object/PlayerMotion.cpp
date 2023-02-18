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

//待機
void PlayerMotion::idle()
{
	frameSpeed = 5;
}

//歩く
void PlayerMotion::walk()
{
	frameSpeed = 10;
}

//登る
void PlayerMotion::ascend()
{
	frameSpeed = 8;
}

//下る
void PlayerMotion::getOff()
{
	frameSpeed = 8;
	imgX = (imgX - 1) % (frameSpeed * maxImgX);
}

//近接攻撃
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
	if (imgY == 4 && newImgX + 1 == maxImgX) {
		isEnabled = true;
	}
}

void PlayerMotion::draw(Vec2 playerPos, int handle, bool trun, Vec2 offset)
{
	Vec2 pos = playerPos + offset;
	newImgX = (imgX / frameSpeed);
	my::myDrawRectRotaGraph(pos.x + 15, pos.y + 15, newImgX * assetSizeX, imgY * assetSizeY, assetSizeX, assetSizeY, 2.0f, 0.0f, handle, true, trun);
//	DrawFormatString(0, 0, 0xffffff, "newImgX : %d", newImgX);
//	DrawFormatString(0, 15, 0xffffff, "imgX : %d", imgX);
}


