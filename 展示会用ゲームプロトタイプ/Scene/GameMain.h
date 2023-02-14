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

	void drawMap(int offsetX);

private:
	int enemyHandle = -1;
	int playerHandle = -1;

	int scrollX = 0;

	int dataX = 0;
	int maxDataX = 0;
	int dataY = 0;

	bool hidden = false;

	int hiddenBlockX = 600;
	int hiddenBlockY = 600;

	int ladderBlockX = 300;
	int ladderBlockY = 200;

	float fieldX = 0.0f, fieldY = 700.0f;

	Player* player;

	std::array<std::shared_ptr<BugSpace>, 3>space;
};

