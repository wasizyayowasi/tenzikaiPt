#pragma once
#include "SceneBase.h"
#include "../Vec2.h"

class InputState;
class SceneManager;

class TextScene : public SceneBase
{
public:
	TextScene(SceneManager& manager, const InputState& input,int num,Vec2 tempOffset,int handle);
	~TextScene();

	void update(const InputState& input);
	void draw();
private:

	int bottanHandle = -1;
	int bottanHandle2 = -1;
	
	
	int time = 8;
	int time2 = 8;

	int imgX = 0;
	int imgY = 0;
	int imgY2 = 3;

	int textNum = 0;
	int textCount = 0;
	int textMaxCount = 0;

	int currentInputIndex = 0;//���ݑI�𒆂̃C���f�b�N�X

	bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	bool selecting = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;
	
	Vec2 offset;
};

