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

	void init();

	void update();
	void draw();

	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void enemySetPlayer(int setHandle);

	void setPos(Vec2 pos) { spacePos = pos; }

	bool isEnable()const;
private:
	int handle = -1;

	int maxHp = 10;

	int num = 0;

	bool pushKey = false;
	bool isEnabled = false;

	Vec2 spacePos = { 0.0f,0.0f };

	Player* player;
	ObjectHp* hp;

	std::array<std::shared_ptr<Enemy>, 16>enemy_;
};

