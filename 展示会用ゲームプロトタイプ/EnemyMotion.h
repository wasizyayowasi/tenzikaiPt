#pragma once
#include "Vec2.h"

class EnemyMotion
{
public:
	EnemyMotion();
	~EnemyMotion();

	void init();

	void update(int num);
	void walk();
	void rolling();
	
	void death();
	void attack();

	void draw(Vec2 enemyPos,int handle,bool trun);

	int dead() { return isEnabled; }
private:

	int imgX = 0;
	int imgY = 6;

	int maxImgX = 0;

	int updateTime = 4;

	bool isEnabled = false;
private:
	void(EnemyMotion::* func)();
};

