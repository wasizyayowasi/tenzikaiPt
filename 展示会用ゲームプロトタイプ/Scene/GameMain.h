#pragma once

class Player;
class BugSpace;
class TimeLimit;

class GameMain
{
public:
	GameMain();
	~GameMain() { delete player, delete space; }

	void update();
	void draw();

	bool hiddenPlayer();
private:
	bool hidden = false;

	int hiddenBlockX = 600;
	int hiddenBlockY = 600;

	Player* player;
	BugSpace* space;
	TimeLimit* limit;
};

