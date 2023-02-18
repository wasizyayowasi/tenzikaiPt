#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class Player;
class Enemy;
class ObjectHp;

class BugSpace
{
public:
	BugSpace(int spaceNum);
	~BugSpace() {}

	std::array<std::shared_ptr<Enemy>, 16>& getEnemy();

	void init(int setX,int setY);

	void update(Vec2 offset);
	void draw(Vec2 offset);

	void move(int scrollX);

	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void enemySetPlayer(int setHandle);

	void setPos(Vec2 pos) { spacePos = pos; }

	bool isEnable()const;

	Vec2 setCoinPos();

private:
	int handle = -1;
	int time = 0;

	int maxHp = 10;

	int num = 0;

	bool isEnabled = false;

	Vec2 spacePos = { 0.0f,0.0f };
	Vec2 enemyDeathPos = { 0.0f,0.0f };

	Player* player;
	ObjectHp* hp;

	std::array<std::shared_ptr<Enemy>, 16>enemy;
};

