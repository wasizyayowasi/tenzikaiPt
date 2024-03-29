#pragma once
#include "SceneBase.h"
#include<map>
#include <string>

class InputState;
class SceneManager;

enum class Choice
{
	keyConfig,			/// 次へ行くボタン
	title,			/// 前に戻るボタン
	prev,
};

class Pause : public SceneBase
{
public:
	Pause(SceneManager& manager, const InputState& input);
	virtual ~Pause();

	virtual void update(const InputState& input)override;
	void normalUpdate(const InputState& input);
	void draw();

	void bottanNum(int num);

	void fadeOutUpdate(const InputState& input);
private:
	int uiSound = -1;
	int uiSound2 = -1;

	int bottanHandle = -1;

	int titleFont = -1;
	int fontHandle = -1;
	int titleFont2 = -1;
	int fontHandle2 = -1;
	int titleWidth = 0;
	int titleWidth2 = 0;
	int fontSize = 0;

	int imgX = 2;
	int imgY = 0;
	int time = 8;

	int moveImgX = 2;
	int moveImgY = 0;

	int nextId = 0;
	int prevId = 0;

	int currentInputIndex;//現在選択中のインデックス

	int ultimateTimer = 0;

	static constexpr int fadeInterval = 60;
	int fadeTimer = 0;
	int fadeValue = 0;
	unsigned int FadeColor = 0x000000;

	//bool isEditing = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	std::map<Choice, std::string> choiceTable;

	void(Pause::*updateFunc)(const InputState& input);
};

