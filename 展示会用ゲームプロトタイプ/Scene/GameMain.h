#pragma once
#include <memory>
#include <array>
#include<vector>
#include "../Vec2.h"
#include "SceneBase.h"

class Player;
class BugSpace;
class Field;
class ObjectHp;

class GameMain : public SceneBase
{
public:
	GameMain(SceneManager& manager);
	virtual ~GameMain();

	void init();

	void update(const InputState& input);
	void draw();

	void gameClearIntroduction(const InputState& input);
	
private:
	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;

	bool hitRock = false;

	float quakeY = 0;
	int tempScreenH;
	int quakeTimer = 0;
	int quakeCount = 0;

	int rockHandle = -1;
	int enemyHandle = -1;
	int repairHandle = -1;
	int hpHandle = -1;
	int portionHandle = -1;
	int macheteHandle = -1;
	int bottanHandle = -1;
	int guiHandle = -1;
	int coinHandle = -1;
	int truckHandle = -1;
	int UIFontHandle = -1;
	
	int mainSound = -1;
	int footstepSound = -1;
	int appearanceSound = -1;

	int footstepSoundVolume = 130;
	int soundCount = 20;
	int soundVolume = 255;
	int musicVolume = 0;

	float truckPos = 0;

	float rockHeight = 100;

	int scrollX = 0;

	int dataX = 0;
	int maxDataX = 0;
	int dataY = 0;

	bool hidden = false;

	int hiddenBlockX = 600;
	int hiddenBlockY = 600;

	int ladderBlockX = 300;
	int ladderBlockY = 200;

	float fieldX = 0.0f, fieldY = 700.0f;

	int clearCount = 0;

	int noLongerUsedX[6];
	int noLongerUsedY[6];

	int spacePosNum[3];

	float waveHp = 0.0f;
	int maxWave = 3;
	int wave = 1;
	int waveCount = 0;

	bool configured = false;
	bool skip = false;
	bool nextScene = false;

	bool EndOfRaid = false;
	bool bossWave = false;

	unsigned int FadeColor = 0x000000;

	bool startWave = false;

	int imgX = 0;
	int imgY = 0;
	int bottanImgX = 10;
	int bottanImgY = 14;
	int time = 0;

	Vec2 enemyPos = { 0,0 };

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void gameoverFadeOutUpdate(const InputState& input);
	void bossBattleSceneFadeOutUpdate(const InputState& input);

	Vec2 offset;
	Vec2 pos;

	Player* player;
	ObjectHp* hp;

	std::vector<std::shared_ptr<BugSpace>>space;
	std::shared_ptr<Field> field;

	using updateFunc_t = void (GameMain::*)(const InputState& input);
	updateFunc_t updateFunc = nullptr;
};

