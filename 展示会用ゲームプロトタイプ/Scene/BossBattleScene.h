#pragma once
#include "SceneBase.h"
#include <memory>
#include <array>
#include "../Vec2.h"

class Player;
class Enemy;
class Field;

class BossBattleScene : public SceneBase
{
public:
	BossBattleScene(SceneManager& manager);
	virtual ~BossBattleScene();

	void init();

	void update(const InputState& input);
	void draw();

	void bossAppearanceUpdate(const InputState& input);

	//�t�F�[�h�C���̎���update�֐�
	void fadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�update�֐�
	void normalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g����update�֐�
	void gameoverFadeOutUpdate(const InputState& input);
	void clearFadeOutUpdate(const InputState& input);
private:

	int enemyHandle = -1;
	int repairHandle = -1;
	int hpHandle = -1;
	int portionHandle = -1;
	int hacheteHandle = -1;
	int guiHandle = -1;
	int coinHandle = -1;
	int signboardHandle = -1;

	int mainSound = -1;
	int collapseSound = -1;
	int footstepSound = -1;
	int soundVolume = 150;
	int soundCount = 20;
	int musicVolume = 0;

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	float fadeValue = 255;//����`�̃u�����h�

	int imgX = 0;
	int ImgTime = 4;
	int startTime = 30;

	bool bossMove = false;

	float quakeY = 0;
	int tempScreenH;
	int quakeTimer = 0;
	int quakeCount = 0;

	Vec2 offset;

	std::shared_ptr<Field> field;

	Player* player;

	using updateFunc_t = void (BossBattleScene::*)(const InputState& input);
	updateFunc_t updateFunc = nullptr;

	std::shared_ptr<Enemy> bossEnemy;

};

