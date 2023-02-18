#pragma once
#include "SceneBase.h"


class KeyConfigScene : public SceneBase
{
public:
	KeyConfigScene(SceneManager& manager, const InputState& input);
	~KeyConfigScene();

	void update(const InputState& input);
	void draw();
private:
	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
	//���͈ꎞ�e�[�u��

	bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;
};

