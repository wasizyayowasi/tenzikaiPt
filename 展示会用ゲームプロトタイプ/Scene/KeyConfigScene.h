#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

enum class KeySelect {
	padNext,
	padNextItem,
	padPrevItem,
	padPause,
	padAttack,
	padAbility,
	padPrev,
};

class KeyConfigScene : public SceneBase
{
public:
	KeyConfigScene(SceneManager& manager, const InputState& input);
	virtual ~KeyConfigScene();

	void update(const InputState& input);
	void draw();

	void firstUpdate(const InputState& input);
	void secondUpdate(const InputState& input);

	void bottanNum(int num);
private:
	int handle = -1;
	int bottanHandle = -1;
	int keyHandle = -1;
	int tempHandle = -1;

	int imgX = 1;
	int imgY = 2;

	int subjectFontHandle = -1;
	int fontHandle = -1;
	int fontHandle2 = -1;
	int subjectFontHandle2 = -1;
	int fontSize = 0;

	int X = 0;
	int keySelectNum = 0;

	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
	//���͈ꎞ�e�[�u��

	bool colorChange = false;

	bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	std::map<KeySelect, std::string> inputDeviceTable;

	void (KeyConfigScene::* updateFunc)(const InputState& input);
};

