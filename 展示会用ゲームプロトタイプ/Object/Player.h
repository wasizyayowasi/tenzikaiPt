#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class PlayerThrowinAttack;
class ObjectHp;
class PlayerMotion;
class InputState;

class Player
{
public:
	Player();
	~Player() { delete hp; }

	Vec2 getPos()const { return playerPos; }

	void update(Vec2 offset,const InputState& input);
	void draw(int handle,Vec2 offset);


	void damege();

	int enemyAttack(Vec2 enemyPos, Vec2 offset);


	bool beHidden();
	bool repairSpace(const Vec2& pos,Vec2 offset);


	bool proximityAttackCollision(const Vec2& pos);
	bool playerFiledCollision(int x,int y);
	bool objectCollision(int x,int y);
	bool ladderCollision(int x, int y);
	bool coinCollision(Vec2 pos, Vec2 offset);
	bool shopCollision(int x,int y, Vec2 offset);

	void updateField(Vec2 offset, const InputState& input);
	void updateDescent(Vec2 offset, const InputState& input);
	void updateLadder(Vec2 offset, const InputState& input);
	void updateDeath(Vec2 offset, const InputState& input);

	int returnSpaceHpDisplay() { return spaceHpDisplay; }
	int isEnable() { return isEnabled; }

	void  playerMove(bool temporaryMove) { move = temporaryMove; }
	void playerMotionUpdate(int num, bool directions);
	int nextScene() { return gameoverScene; }
private:
	int money = 0;

	int imgX = 0;

	bool collision = false;

	bool gameoverScene = false;

	bool playerDirections = false;
	int motionNum = 0;

	int playerHp = 10;
	int hpDisplayTime = 120;

	bool move = true;

	bool isEnabled = true;

	//hp表示
	bool hpDisplay = false;
	//hp表示の時間
	bool spaceHpDisplay = false;
	//オブジェクトに隠れているか
	bool hidden = false;

	bool hit = false;
	//隠れるボタンを押したか
	bool push = false;
	//敵に当たっているか
	bool enemyHit = false;
	//近接攻撃
	bool proximityAttack = false;

	int time = 0;

	int chipNo3 = 0;

	ObjectHp* hp;
	PlayerMotion* motion;

	Vec2 playerPos = {400.0f,600.0f};
	Vec2 vec = { 0.0f,10.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;

	void (Player::* updateFunc)(Vec2 offset,const InputState& input);
};

