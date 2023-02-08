#pragma once
#include <memory>
#include <array>

class Player;
class BugSpace;

class GameMain
{
public:
	GameMain();
	~GameMain();

	void init();

	void update();
	void draw();

	bool hiddenPlayer();

	bool ladderCollision();

	bool playerFiledCollision(int x,int y);
private:
	int enemyHandle = -1;
	int playerHandle = -1;

	bool hidden = false;

	int hiddenBlockX = 600;
	int hiddenBlockY = 600;

	int ladderBlockX = 300;
	int ladderBlockY = 200;

	float fieldX = 0.0f, fieldY = 700.0f;

	Player* player;

	std::array<std::shared_ptr<BugSpace>, 3>space;
};

