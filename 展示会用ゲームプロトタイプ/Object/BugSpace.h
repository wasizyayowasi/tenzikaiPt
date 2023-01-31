#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class Player;
class Enemy;

class BugSpace
{
public:
	BugSpace();
	~BugSpace() {}

	void update();
	void draw();

	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void enemySetPlayer();

	void setPos(Vec2 pos) { spacePos = pos; }
private:
	bool pushKey = false;

	Vec2 spacePos = { 0.0f,0.0f };

	//Enemy* enemy;
	Player* player;

	std::array<std::shared_ptr<Enemy>, 16>enemy_;
};

