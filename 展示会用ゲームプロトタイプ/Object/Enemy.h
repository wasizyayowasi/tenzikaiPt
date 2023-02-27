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
	virtual ~Enemy() { delete hp; }

	Vec2 getPos()const { return enemyPos; }
	void setPlayer(Player* dPlayer,int setHandle,int cHandle) { player = dPlayer,handle = setHandle,coinHandle = cHandle; }

	void normalUpdate(Vec2 offset);
	void normalDraw(Vec2 offset);

	void coinUpdate(Vec2 offset);
	void coinDraw(Vec2 offset);

	void updateDescent(Vec2 offset);

	void dispatch(const Vec2& pos);
	void update(Vec2 offset);
	void draw(Vec2 offset);

	bool isEnable()const;

	bool filedCollision(int x, int y);

	int motionaiu() { return motionNum; }

	Vec2 deadPos();

	int returnDeath() { return death; }
private:
	int sleepTime = 0;

	bool death = false;

	int coolTime = 0;

	int handle = -1;
	int coinHandle = -1;

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
	Vec2 deathPos = { 0.0f,0.0f };
	Vec2 vec = {1.0f,0.0f};

	Vec2 targetPlayer = { 0.0f,0.0f };
	Vec2 targetPlayer2 = { 0.0f,0.0f };

	Player* player;
	ObjectHp* hp;
	EnemyMotion* motion;

	void (Enemy::* updateFunc)(Vec2 offset);
	void (Enemy::* drawFunc)(Vec2 offset);
};

