#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>
#include "../PlayerMotion.h"

class PlayerThrowinAttack;
class ObjectHp;

class Player
{
public:
	Player();
	~Player() { delete hp; }

	Vec2 getPos()const { return playerPos; }

	void update();
	void draw(int handle);

	bool beHidden();

	void damege();

	int enemyAttack(Vec2 enemyPos);

	bool proximityAttackCollision(const Vec2& pos);

	bool repairSpace(const Vec2& pos);

	int returnSpaceHpDisplay() { return spaceHpDisplay; }

	bool playerFiledCollision(int x,int y);

	bool objectCollision(int x,int y);

	bool ladderCollision(int x, int y);

	void updateField();

	void updateDescent();

	void updateLadder();

	void updateDeath();

	int isEnable() { return isEnabled; }

	void  playerMove(bool temporaryMove) { move = temporaryMove; }
	void playerMotionUpdate(int num, bool directions);
private:

	bool collision = false;

	bool playerDirections = false;
	int motionNum = 0;

	int playerHp = 10;
	int hpDisplayTime = 120;

	bool move = true;

	bool isEnabled = true;

	//hp�\��
	bool hpDisplay = false;
	//hp�\���̎���
	bool spaceHpDisplay = false;
	//�I�u�W�F�N�g�ɉB��Ă��邩
	bool hidden = false;

	bool hit = false;
	//�B���{�^������������
	bool push = false;
	//�G�ɓ������Ă��邩
	bool enemyHit = false;
	//�ߐڍU��
	bool proximityAttack = false;

	int time = 0;

	int chipNo3 = 0;

	ObjectHp* hp;
	PlayerMotion* motion;

	Vec2 playerPos = {600.0f,248.0f};
	Vec2 vec = { 0.0f,10.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;

	void (Player::* updateFunc)();
};

