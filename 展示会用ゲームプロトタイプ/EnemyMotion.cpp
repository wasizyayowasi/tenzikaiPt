#include "EnemyMotion.h"
#include "DrawFunctions.h"

EnemyMotion::EnemyMotion()
{
}

EnemyMotion::~EnemyMotion()
{
}

void EnemyMotion::init()
{
	func = &EnemyMotion::walk;
	imgY = 6;
	imgX = 0;
	isEnabled = false;
}

void EnemyMotion::update(int num)
{
	switch (num) {
	case 0:
		func = &EnemyMotion::walk;
		break;
	case 1:
		func = &EnemyMotion::rolling;
		break;
	case 2:
		func = &EnemyMotion::attack;
		break;
	case 3:
		func = &EnemyMotion::death;
		break;
	}
	//Šeƒ‚[ƒVƒ‡ƒ“‚Ì”‚ðŒˆ‚ß‚é
	switch (imgY) {
	case 0:
		maxImgX = 10;
		break;
	case 1:
		maxImgX = 13;
		break;
	case 2:
		maxImgX = 9;
		break;
	case 3:
		maxImgX = 11;
		break;
	case 4:
		maxImgX = 10;
		break;
	case 5:
		maxImgX = 3;
		break;
	case 6:
		maxImgX = 7;
		break;
	case 7:
		maxImgX = 6;
		break;
	case 8:
		maxImgX = 6;
		break;
	}
	(this->*func)();
}

void EnemyMotion::walk()
{
	if (!(imgY == 6)) {
		imgY = 6;
	}

	if (--updateTime == 0) {
		imgX++;
		updateTime = 4;
	}

	if (imgX > maxImgX) {
		imgX = 0;
	}
	
}

void EnemyMotion::rolling()
{

	if (!(imgY == 2 || imgY == 3 || imgY == 4)) {
		imgY = 2;
	}

	if (--updateTime == 0) {
		imgX++;
		updateTime = 4;
	}
	if (imgY == 3 && imgX > maxImgX) {
		imgX = 8;
	}
	if (imgX > maxImgX) {
		imgX = 0;
		imgY++;
	}
}

void EnemyMotion::death()
{
	if (!(imgY == 0)) {
		if (imgY == 1) {
			imgY = 1;
			maxImgX = 13;
		}
		else {
			imgY = 0;
		}
	}
	
	
	if (--updateTime == 0) {
		imgX++;
		updateTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
		imgY++;
	}
	if (imgY == 1 && imgX > maxImgX - 1 ) {
		isEnabled = true;
	}
}

void EnemyMotion::attack()
{
	if (!(imgY == 7 || imgY == 8)) {
		imgY = 7;
	}
	if (--updateTime == 0) {
		imgX++;
		updateTime = 4;
	}
	if (imgX > maxImgX) {
		imgX = 0;
		imgY++;
	}
	if (imgY > 8) {
		imgY = 7;
	}
}

void EnemyMotion::draw(Vec2 enemyPos,int handle, bool trun)
{
	my::myDrawRectRotaGraph(enemyPos.x + 15, enemyPos.y + 15, imgX * 40, imgY * 40, 40, 40, 2.0f, 0.0f, handle, true, trun);
}
