#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class Player;
class Enemy;
class ObjectHp;

namespace {
	constexpr int enemyNum = 5;
}

class BugSpace
{
public:
	BugSpace();
	virtual ~BugSpace();

	std::array<std::shared_ptr<Enemy>, enemyNum>& getEnemy();

	void init(int setX,int setY);

	void tutorialUpdate(Vec2 offset);
	void update(Vec2 offset);
	void draw(Vec2 offset);

	void move(int scrollX);

	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void enemySetPlayer(int setHandle,int coinHandle);

	void setPos(Vec2 pos) { spacePos = pos; }

	bool isEnable()const;

	Vec2 setCoinPos();

	int returnHp() { return maxHp; }

private:
	int handle = -1;
	int time = 0;

	int imgX = 0;
	int imgY = 0;

	int spaceHandle = -1;

	int enemySpawnTime = 0;
	int enemySpawnInterval = 600;

	int maxHp = 10;

	int num = 0;

	bool isEnabled = false;

	Vec2 spacePos = { 0.0f,0.0f };
	Vec2 enemyDeathPos = { 0.0f,0.0f };

	Player* player = nullptr;
	ObjectHp* hp;

	std::array<std::shared_ptr<Enemy>, enemyNum>enemy;
};

