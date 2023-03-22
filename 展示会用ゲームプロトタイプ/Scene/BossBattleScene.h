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

	void update(const InputState& input);
	void draw();

	void bossAppearanceUpdate(const InputState& input);

	//フェードインの時のupdate関数
	void fadeInUpdate(const InputState& input);
	//通常状態のupdate関数
	void normalUpdate(const InputState& input);

	void particleUpdate();
	void particleDraw();

	//フェードアウト時のupdate関数
	void gameoverFadeOutUpdate(const InputState& input);
	void clearFadeOutUpdate(const InputState& input);
	void clearUpdate(const InputState& input);
private:

	int enemyHandle = -1;
	int repairHandle = -1;
	int hpHandle = -1;
	int portionHandle = -1;
	int macheteHandle = -1;
	int guiHandle = -1;
	int coinHandle = -1;
	int signboardHandle = -1;
	int cheerSound = -1;

	int mainSound = -1;
	int collapseSound = -1;
	int footstepSound = -1;
	int soundVolume = 150;
	int soundCount = 20;
	int musicVolume = 0;

	float sinRate = 0.0f;
	int effectScreen = -1;
	Vec2 pos[257];
	Vec2 vec[257];

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;//黒矩形のブレンド具合
	int fadeColor = 0x000000;

	int imgX = 0;
	int ImgTime = 4;
	int startTime = 30;

	bool bossMove = false;
	int cheerCount = 720;
	int cheerTime = 6;

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

