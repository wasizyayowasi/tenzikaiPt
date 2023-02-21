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
	void draw();

private:
	int currentInputIndex;//現在選択中のインデックス

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;

	//bool isEditing = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	std::map<Choice, std::string> choiceTable;
};

