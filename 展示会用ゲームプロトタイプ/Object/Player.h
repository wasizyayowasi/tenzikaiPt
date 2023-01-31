#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class TimeLimit;
class PlayerThrowinAttack;
class ObjectHp;

class Player
{
public:
	Player();
	~Player() {}

	Vec2 getPos()const { return PlayerPos; }

	void update();
	void draw();

	bool beHidden();

	void hiddenBlockTrue() { hidden = true; }
	void hiddenBlockFalse() { hidden = false, push = false; }

	void damege();

	int enemyAttack(Vec2 enemyPos);

	bool proximityAttackCollision(const Vec2& pos);
private:
	int playerDirections = 1;

	int playerHp = 10;
	int hpDisplayTime = 120;

	//hp�\��
	bool hpDisplay = false;
	//hp�\���̎���
	bool timeDisplay = false;
	//�I�u�W�F�N�g�ɉB��Ă��邩
	bool hidden = false;

	bool hit = false;
	//�B���{�^������������
	bool push = false;
	bool push2 = false;
	//�G�ɓ������Ă��邩
	bool enemyHit = false;
	//�ߐڍU��
	bool proximityAttack = false;

	int aiu = 0;

	int time = 0;

	TimeLimit* limit;
	ObjectHp* hp;

	Vec2 PlayerPos = {100.0f,636.0f};
	Vec2 vec = { 0.0f,1.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;
};

