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

	int currentInputIndex = 0;//現在選択中のインデックス

	bool isEditing = false;//現在特定の入力は編集中です。
	bool selecting = false;//現在特定の入力は編集中です。
	const InputState& inputState;
	
	Vec2 offset;
};

