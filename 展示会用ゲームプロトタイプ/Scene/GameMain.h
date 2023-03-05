#pragma once
#include <memory>
#include <array>
#include "../Vec2.h"
#include "SceneBase.h"

class Player;
class BugSpace;
class Field;

class GameMain : public SceneBase
{
public:
	GameMain(SceneManager& manager);
	virtual ~GameMain();

	void init();

	void update(const InputState& input);
	void draw();

private:
	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;

	int enemyHandle = -1;
	int repairHandle = -1;
	int hpHandle = -1;
	int portionHandle = -1;
	int hacheteHandle = -1;
	int guiHandle = -1;
	int coinHandle = -1;
	int truckHandle = -1;
	
	int mainSound = -1;

	int soundVolume = 150;

	float truckPos = 0;

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

	int wave = 3;
	int waveCount = 0;

	bool configured = false;
	bool skip = false;

	unsigned int FadeColor = 0x000000;//フェード色黒

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void gameoverFadeOutUpdate(const InputState& input);
	void bossBattleSceneFadeOutUpdate(const InputState& input);

	Vec2 offset;
	Vec2 pos;
	Player* player;

	std::array<std::shared_ptr<BugSpace>, 3>space;
	std::shared_ptr<Field> field;

	using updateFunc_t = void (GameMain::*)(const InputState& input);
	updateFunc_t updateFunc = nullptr;
};

