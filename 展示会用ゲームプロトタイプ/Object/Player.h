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
	Player();
	virtual~Player();

	Vec2 getPos()const { return playerPos; }

	void init();
	void update(Vec2 offset,const InputState& input);
	void draw(Vec2 offset);


	void damege(bool inversion);

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

	void consumption();

	void setHandle(int pHandle, int mHandle,int gHandle) { portionHandle = pHandle, macheteHandle = mHandle,guiHandle = gHandle; }
private:
	//�����A�C�e��
	int money = 2000;
	int repairBlock = 3;
	int recoveryItem = 1;

	//�O���t�B�b�N�n���h��
	int playerHandle = -1;
	int macheteHandle = -1;
	int portionHandle = -1;
	int guiHandle = -1;

	//���G����
	int ultimateTimer_ = 0;

	//���ւ̃V�[���؂�ւ�
	bool gameoverScene = false;

	//�v���C���[�������Ă������
	bool playerDirections = false;

	//���[�V�����̔ԍ�
	int motionNum = 0;

	//HP�֘A
	int playerHp = 10;
	int hpDisplayTime = 120;

	//����
	bool isEnabled = true;

	//hp�\���̎���
	bool spaceHpDisplay = false;

	//�I�u�W�F�N�g�ɉB��Ă��邩
	bool hidden = false;

	//�B���{�^������������
	bool push = false;

	//�G�ɓ������Ă��邩
	bool enemyHit = false;

	//�ߐڍU��
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

