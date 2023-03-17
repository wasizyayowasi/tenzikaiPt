#pragma once
#include "../Vec2.h"
#include <memory>
#include <array>

class PlayerThrowinAttack;
class ObjectHp;
class PlayerMotion;
class InputState;
class Inventory;

class Player
{
public:
	Player(int num);
	virtual~Player();

	Vec2 getPos()const { return playerPos; }
	void  setPos(Vec2 pos) { playerPos = pos; }

	void init();
	void update(Vec2 offset,const InputState& input);
	void draw(Vec2 offset);


	void damege(bool inversion);
	void bossDamege();

	int enemyAttack(Vec2 enemyPos, Vec2 offset);
	int bossEnemyAttack(Vec2 enemyPos, Vec2 offset);

	bool beHidden();
	bool setPush() { return push; }
	bool repairSpace(const Vec2& pos,Vec2 offset);


	bool proximityAttackCollision(const Vec2& pos);
	bool playerFiledCollision(int x,int y);
	bool objectCollision(int x,int y);
	bool ladderCollision(int x, int y);
	bool coinCollision(Vec2 pos, Vec2 offset);
	bool shopCollision(int x,int y, Vec2 offset);

	void tutorialUpdate(Vec2 offset, const InputState& input);
	void BossUpdate(Vec2 offset, const InputState& input);
	void updateField(Vec2 offset, const InputState& input);
	void updateDescent(Vec2 offset, const InputState& input);
	void updateLadder(Vec2 offset, const InputState& input);
	void updateDeath(Vec2 offset, const InputState& input);
	bool updateSwoon(Vec2 offset);
	bool updateResuscitation(Vec2 offset);

	int returnSpaceHpDisplay() { return spaceHpDisplay; }
	int isEnable() { return isEnabled; }

	int setMoneyPossessed() { return money; }

	int nextScene() { return gameoverScene; }

	void setItemControl(int num);
	void setMoney(int amount);

	void consumption();

	void setHandle(int pHandle, int mHandle,int gHandle,int hHandle,int rHandle,int cHandle) { portionHandle = pHandle, macheteHandle = mHandle,guiHandle = gHandle,hpHandle = hHandle,repairHandle = rHandle,coinHandle = cHandle; }
	int returnHp();

	void setMotion(bool start);

	int returnRepair() { return repairBlock; }

	int returnHpDisplay() { return spaceHpDisplay; }

	bool returnFlyingisEnabled();

	bool returnEnemyHit() { return enemyHit; }

	//void setMovementRestrictions(float limitNum) { movementRestrictions = limitNum; }

	bool rockCollision(Vec2 pos);

private:
	//所持アイテム
	int money = 3000;
	int repairBlock = 2;
	int recoveryItem = 0;

	//グラフィックハンドル
	int playerHandle = -1;
	int macheteHandle = -1;
	int portionHandle = -1;
	int guiHandle = -1;
	int hpHandle = -1;
	int repairHandle = -1;
	int coinHandle = -1;
	int smokeHandle = -1;
	int dieSound = -1;
	int cureSound = -1;
	int damageSound = -1;
	int attackSound = -1;
	int coinSound = -1;
	int walkSound = -1;
	int throwSound = -1;
	int fontHandle = -1;
	int ladderSound = -1;

	int sceneNum = 0;

	int soundCount = 1;

	int soundVolume = 150;

	int soundTime = 1;

	//スモークの画像の類
	int imgX = 0;
	int imgY = 3;

	bool motionStart = false;
	bool bat = false;
	bool bat2 = false;

	//無敵時間
	int ultimateTimer = 0;

	//次へのシーン切り替え
	bool gameoverScene = false;
	bool next = false;

	//プレイヤーが向いている方向
	bool playerDirections = false;

	//モーションの番号
	int motionNum = 0;

	//HP関連
	int playerHp = 3;
	int hpDisplayTime = 120;

	bool rightClosure = false;
	bool leftClosure = false;

	//生死
	bool isEnabled = true;

	//hp表示の時間
	bool spaceHpDisplay = false;

	//オブジェクトに隠れているか
	bool hidden = false;

	//隠れるボタンを押したか
	bool push = false;

	//敵に当たっているか
	bool enemyHit = false;

	//近接攻撃
	bool proximityAttack = false;

	int chipNo3 = 0;

	float movementRestrictions = 0.0f;

	int coinDisplayTime = 0;
	static constexpr int fadeInterval = 60;
	int fadeValue = 255;

	std::shared_ptr<ObjectHp> hp;
	std::shared_ptr<PlayerMotion> motion;
	std::shared_ptr<Inventory> inventory;

	Vec2 playerPos = {200.0f,700.0f};
	Vec2 vec = { 0.0f,10.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;

	void (Player::* updateFunc)(Vec2 offset,const InputState& input);
};

