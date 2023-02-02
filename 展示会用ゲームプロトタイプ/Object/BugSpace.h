#pragma once
#include "../Vec2.h"
#include "ObjectHp.h"
#include <memory>
#include <array>

class Player;
class Enemy;

class BugSpace
{
public:
	BugSpace(int spaceNum);
	~BugSpace() {}

	void update();
	void draw();

	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void enemySetPlayer();

	void setPos(Vec2 pos) { spacePos = pos; }
private:
	int num = 0;

	bool pushKey = false;

	Vec2 spacePos = { 0.0f,0.0f };

	Player* player;

	std::array<std::shared_ptr<Enemy>, 16>enemy_;
};

