#pragma once
#include <memory>
#include <array>

class Player;
class BugSpace;
class TimeLimit;

class GameMain
{
public:
	GameMain();
	~GameMain() { delete player; }

	void init();

	void update();
	void draw();

	bool hiddenPlayer();
private:
	bool hidden = false;

	int hiddenBlockX = 600;
	int hiddenBlockY = 600;

	float x = 100.0f,y = 200.0f;

	float fieldX = 0.0f, fieldY = 700.0f;

	Player* player;
	TimeLimit* limit;

	std::array<std::shared_ptr<BugSpace>, 3>space;
};

