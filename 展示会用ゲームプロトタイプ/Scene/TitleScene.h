#pragma once
#include "SceneBase.h"
#include "../Vec2.h"
#include <memory>
#include <string>
#include <map>
#include <array>

class Enemy;
class Field;

enum class choice
{
	main,			/// 次へ行くボタン
	tutorial,		/// 前に戻るボタン
	option,
	end,			/// ポーズボタン
};

class TitleScene : public SceneBase
{
public:

	TitleScene(SceneManager& manager);
	~TitleScene();

	void update(const InputState& input);
	void draw();

	void choiceScene(const InputState& input);
	void choiceSceneDraw();

private:
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;//黒矩形のブレンド具合

	unsigned int FadeColor = 0x000000;//フェード色黒

	int currentInputIndex = 0;//現在選択中のインデックス

	//フェードインの時のupdate関数
	void fadeInUpdate(const InputState& input);
	//通常状態のupdate関数
	void normalUpdate(const InputState& input);
	//フェードアウト時のupdate関数
	void fadeOutUpdateGameMain(const InputState& input);
	void fadeOutUpdateTutorial(const InputState& input);

	int soundHandle = -1;
	int soundVolume = 150;

	int enemyHandle = -1;
	int collTime = 300;
	int luck = 0;
	int fontSize = 0;
	bool enemyGo = false;
	bool enemiesGo = false;
	Vec2 shift = {120.0f,800.0f};

	std::map<choice, std::string> sceneTable;

	std::shared_ptr<Enemy> enemy;
	std::shared_ptr<Field> field;

	std::array<std::shared_ptr<Enemy>, 16> enemies;

	//update用メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};

