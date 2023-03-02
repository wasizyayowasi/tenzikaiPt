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

	int returnSpaceHpDisplay() { return spaceHpDisplay; }
	int isEnable() { return isEnabled; }

	int setMoneyPossessed() { return money; }

	int nextScene() { return gameoverScene; }

	void setItemControl(int num);
	void setMoney(int amount);

	void giftMoney() { money = 5000; }

	void consumption();

	void setHandle(int pHandle, int mHandle,int gHandle,int hHandle,int rHandle,int cHandle) { portionHandle = pHandle, macheteHandle = mHandle,guiHandle = gHandle,hpHandle = hHandle,repairHandle = rHandle,coinHandle = cHandle; }
	int returnHp() { return playerHp; }

	void setMotion(bool start);

	int returnRepair() { return repairBlock; }

	int returnHpDisplay() { return spaceHpDisplay; }

	bool returnFlyingisEnabled();
private:
	//所持アイテム
	int money = 0;
	int repairBlock = 0;
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

	int sceneNum = 0;

	//スモークの画像の類
	int imgX = 0;
	int imgY = 3;

	bool motionStart = false;
	bool bat = false;
	bool bat2 = false;

	//無敵時間
	int ultimateTimer_ = 0;

	//次へのシーン切り替え
	bool gameoverScene = false;

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

	ObjectHp* hp;
	PlayerMotion* motion;
	Inventory* inventory;

	Vec2 playerPos = {400.0f,300.0f};
	Vec2 vec = { 0.0f,10.0f };

	std::shared_ptr<PlayerThrowinAttack> flyingObject;

	void (Player::* updateFunc)(Vec2 offset,const InputState& input);
};

