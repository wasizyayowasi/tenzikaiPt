#pragma once
#include "SceneBase.h"
#include "../Vec2.h"
#include <memory>

class Player;
class Field;
class Enemy;
class BugSpace;

class TutorialScene : public SceneBase
{
public:
	TutorialScene(SceneManager& manager);
	virtual ~TutorialScene();

	void update(const InputState& input);
	void draw();

	//フェードインの時のupdate関数
	void fadeInUpdate(const InputState& input);
	//通常状態のupdate関数
	void normalUpdate(const InputState& input);
	//フェードアウト時のupdate関数
	void clearFadeOutUpdate(const InputState& input);

	void textDraw(int num);
private:

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;//黒矩形のブレンド具合
	int fadeColor = 0x000000;

	int enemyHandle = -1;
	int repairHandle = -1;
	int hpHandle = -1;
	int portionHandle = -1;
	int hacheteHandle = -1;
	int guiHandle = -1;
	int coinHandle = -1;
	int shopperHandle = -1;
	int bottanHandle = -1;
	int bottanHandle2 = -1;
	int bottanHandle3 = -1;
	int truckHandle = -1;
	int arrowHandle = -1;
	int descriptionHandle = -1;
	int descriptionHandle2 = -1;
	int descriptionHandle3 = -1;
	int descriptionHandle4 = -1;
	int fontHandle = -1;

	int graphWidth = 0;
	int graphHeight = 0;

	int personNum = 3;
	int count = 1;

	bool buy = false;

	int musicVolume = 120;

	int time = 1;
	int time2 = 1;
	int spownCount = 1;

	int textCount = 0;

	Vec2 targetPlayer = { 0.0f,0.0f };
	Vec2 targetPlayer2 = { 0.0f,0.0f };
	Vec2 targetPlayer3 = { 0.0f,0.0f };
	Vec2 targetPlayer4 = { 0.0f,0.0f };
	Vec2 targetPlayer5 = { 0.0f,0.0f };
	Vec2 targetPlayer6 = { 0.0f,0.0f };
	Vec2 targetPlayer7 = { 0.0f,0.0f };
	Vec2 offset;

	int truckPos = 3500;

	bool textUpdate = false;

	int imgX = 0;
	int imgX2 = 0;
	int imgY = 0;
	int imgY2 = 3;

	std::shared_ptr<Field> field;
	std::shared_ptr<Enemy> enemy;
	std::shared_ptr<BugSpace> space;

	Player* player;

	using updateFunc_t = void (TutorialScene::*)(const InputState& input);
	updateFunc_t updateFunc = nullptr;
};

