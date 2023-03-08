#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

enum class KeySelect {
	keyNext,
	keyNextItem,
	keyPrevItem,
	keyPause,
	keyAttack,
	keyAbility,

	padNext,
	padNextItem,
	padPrevItem,
	padPause,
	padAttack,
	padAbility,
};

class KeyConfigScene : public SceneBase
{
public:
	KeyConfigScene(SceneManager& manager, const InputState& input);
	virtual ~KeyConfigScene();

	void update(const InputState& input);
	void draw();

	void bottanNum(int num);
private:
	int handle = -1;
	int bottanHandle = -1;
	int keyHandle = -1;

	int imgX = 1;
	int imgY = 2;

	int subjectFontHandle = -1;
	int fontHandle = -1;
	int fontSize = 0;

	int X = 0;
	int Y = 0;

	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
	//���͈ꎞ�e�[�u��

	bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	std::map<KeySelect, std::string> inputDeviceTable;
};

