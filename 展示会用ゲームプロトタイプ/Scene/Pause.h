#pragma once
#include "SceneBase.h"
#include<map>
#include <string>

class InputState;
class SceneManager;

enum class Choice
{
	keyConfig,			/// ���֍s���{�^��
	title,			/// �O�ɖ߂�{�^��
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
	int uiSound = -1;
	int uiSound2 = -1;

	int titleFont = -1;
	int fontHandle = -1;
	int titleFont2 = -1;
	int fontHandle2 = -1;
	int titleWidth = 0;
	int titleWidth2 = 0;
	int fontSize = 0;

	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;

	//bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	std::map<Choice, std::string> choiceTable;
};

