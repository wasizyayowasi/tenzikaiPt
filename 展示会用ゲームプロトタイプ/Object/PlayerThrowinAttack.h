#pragma once
#include "../Vec2.h"


class PlayerThrowinAttack
{
public:
	PlayerThrowinAttack();
	~PlayerThrowinAttack(){}

	void attack(const Vec2& pos,bool directions);
	bool isEnable()const;

	void init();

	void update(Vec2 offset);
	void draw();

	bool playerCollision(const Vec2& pos,Vec2 offset);
	bool enemyCollision(const Vec2& pos, Vec2 offset);

	void deadFlyingObject();

	bool landing() { return landingObject; }

	bool filedCollision(int y);
private:
	int time = 50;
	bool playerDirections = false;
	float angle = 0.0f;

	float x = 0.0f;
	float y = 0.0f;
	float r = 8.0f;

	float up = 0.0f;

	bool isEnabled = false;
	bool landingObject = false;

	Vec2 normalize;
	Vec2 flyingObjectPos;
	Vec2 pos;
	Vec2 vec = {5.0f,0.0f};
};

