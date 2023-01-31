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
	~BugSpace() { delete player/*, delete enemy*/; }

	void update();
	void draw();

	void setPlayer(Player* dPlayer) { player = dPlayer; }

	void enemySetPlayer();
private:
	int a = 0;
	bool pushKey = false;

	Vec2 spacePos = { 800.0f,400.0f };

	//Enemy* enemy;
	Player* player;

	std::array<std::shared_ptr<Enemy>, 16>enemy_;
};

