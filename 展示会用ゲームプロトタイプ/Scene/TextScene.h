#pragma once
#include "SceneBase.h"
#include "../Vec2.h"
#include <map>
#include <string>

class InputState;
class SceneManager;

enum class textNum1 {
	textCount1,
	textCount2,
	textCount3,
};

enum class textNum2 {
	textCount1,
	textCount2,
};

enum class textNum3 {
	textCount1,
	textCount2,
	textCount3,
	textCount4,
	textCount5,
};

enum class textNum4 {
	textCount1,
	textCount2,
	textCount3,
	textCount4,
	textCount5,
};

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
	
	int textHandle = -1;
	
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

	std::map<textNum1, std::string> text1;
	std::map<textNum2, std::string> text2;
	std::map<textNum3, std::string> text3;
	std::map<textNum4, std::string> text4;
};

