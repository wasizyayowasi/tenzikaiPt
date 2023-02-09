#pragma once
#include "../Vec2.h"
#include <DxLib.h>

class Player;
class ObjectHp;
class EnemyMotion;

class Enemy
{
public:
	Enemy();
	~Enemy() { delete hp; }

	Vec2 getPos()const { return enemyPos; }
	void setPlayer(Player* dPlayer,int setHandle) { player = dPlayer,handle = setHandle; }

	void dispatch(const Vec2& pos);
	void update();
	void draw();

	bool isEnable()const;

	bool filedCollision(int x, int y);

	int motionaiu() { return motionNum; }
private:
	int coolTime = 0;

	int handle = -1;

	int moveCount = 0;

	int count = 0;

	int motionNum = 0;

	int enemyHp = 9;
	int hpDisplayTime = 120;

	bool inversion = false;

	bool hpDisplay = false;
	bool stop = false;

	bool landing = false;

	bool isEnabled = false;

	bool hidden = false;

	bool lock = false;

	Vec2 enemyPos = {800.0f,600.0f};
	Vec2 vec = {3.0f,1.0f};

	Vec2 targetPlayer = { 0.0f,0.0f };
	Vec2 targetPlayer2 = { 0.0f,0.0f };

	Vec2 territoryPos = { 0.0f,0.0f };

	Player* player;
	ObjectHp* hp;
	EnemyMotion* motion;
};

