#pragma once
#include "../Vec2.h"
#include <DxLib.h>

class Player;
class ObjectHp;

class Enemy
{
public:
	Enemy();
	~Enemy() { delete hp; }

	Vec2 getPos()const { return enemyPos; }
	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void dispatch(const Vec2& pos);
	void update();
	void draw();

	bool isEnable()const;

	void hitFlyingObject();
private:
	int enemyHp = 9;
	int hpDisplayTime = 120;

	bool hit = false;
	bool chase = false;
	bool hpDisplay = false;
	bool stop = false;

	bool isEnabled = false;

	bool hidden = false;

	Vec2 enemyPos = {800.0f,600.0f};
	Vec2 vec = {5.0f,1.0f};

	Player* player;
	ObjectHp* hp;
};

