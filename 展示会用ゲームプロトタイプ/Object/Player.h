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
	~Player() { delete hp; delete limit; }

	Vec2 getPos()const { return PlayerPos; }

	void update();
	void draw();

	bool beHidden();

	void setHidden(bool temporaryHidden) { hidden = temporaryHidden; }
	void setLadder(bool temporaryLadder) { ladder = temporaryLadder; }

	void damege();

	int enemyAttack(Vec2 enemyPos);

	bool proximityAttackCollision(const Vec2& pos);

	bool repairSpace(const Vec2& pos);

	void setRepair(/*bool temporaryRepair*/int num);
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
	bool ladder = false;
	bool upperLimit = false;

	bool hit = false;
	//�B���{�^������������
	bool push = false;
	bool push2 = false;
	//�G�ɓ������Ă��邩
	bool enemyHit = false;
	//�ߐڍU��
	bool proximityAttack = false;

	int repair = 0;
	//bool repair = false;

	int time = 0;

	TimeLimit* limit;
	ObjectHp* hp;

	Vec2 PlayerPos = {100.0f,636.0f};
	Vec2 vec = { 0.0f,10.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;
};

