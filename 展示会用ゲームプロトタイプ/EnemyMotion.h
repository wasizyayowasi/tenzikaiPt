#pragma once
#include "Vec2.h"

class EnemyMotion
{
public:
	EnemyMotion();
	~EnemyMotion();

	void update(int num);
	void walk();
	void rolling();
	
	void death();
	void attack();

	void draw(Vec2 enemyPos,int handle,bool trun);

	int dead() { return isEnable; }
private:
	int enemyHandle = -1;

	int imgX = 0;
	int imgY = 6;

	int maxImgX = 0;

	int updateTime = 4;

	bool isEnable = false;
private:
	void(EnemyMotion::* func)();
};

