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

	int currentInputIndex;//現在選択中のインデックス
	//入力一時テーブル

	bool colorChange = false;

	bool isEditing = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	std::map<KeySelect, std::string> inputDeviceTable;

	void (KeyConfigScene::* updateFunc)(const InputState& input);
};

