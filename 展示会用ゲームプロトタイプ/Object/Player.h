#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class TimeLimit;
class PlayerThrowinAttack;

class Player
{
public:
	Player();
	~Player() {}

	Vec2 getPos()const { return pos; }

	void update();
	void draw();

	bool beHidden();

	void hiddenBlockTrue() { hidden = true; }
	void hiddenBlockFalse() { hidden = false; }
private:
	int playerDirections = 1;

	bool timeDisplay = false;
	bool hidden = false;
	bool hit = false;
	bool push = false;
	bool push2 = false;

	int aiu = 0;

	TimeLimit* limit;

	Vec2 pos = {100.0f,636.0f};
	Vec2 vec = { 0.0f,1.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;
};

