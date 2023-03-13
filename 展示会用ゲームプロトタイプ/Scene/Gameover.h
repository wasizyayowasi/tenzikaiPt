#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;
class PlayerMotion;

enum class GameOverChoice
{
	continueMain,			/// 次へ行くボタン
	end,			/// 前に戻るボタン
};

class Gameover : public SceneBase
{
public:
	Gameover(SceneManager& manager, const InputState& input,int num);
	virtual ~Gameover();
	
	void update(const InputState& input);
	void draw();
private:
	int handle = -1;
	int lightHandle = -1;
	int fontHandle = -1;
	int fontHandle2 = -1;
	int fontHandle3 = -1;
	int uiSound = -1;
	int uiSound2 = -1;

	int musicVolume = 0;

	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	int sceneNum = 0;

	int fontSize = 0;

	int currentInputIndex;//現在選択中のインデックス

	unsigned int FadeColor = 0x000000;//フェード色黒

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);
	void continueFadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (Gameover::*)(const InputState&);
	UpdateFunc_t updateFunc_;

	const InputState& inputState;

	PlayerMotion* motion;

	std::map<GameOverChoice, std::string> gameOverChoiceTable;
};

