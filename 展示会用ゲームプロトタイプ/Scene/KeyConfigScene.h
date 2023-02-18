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
	int currentInputIndex;//現在選択中のインデックス
	//入力一時テーブル

	bool isEditing = false;//現在特定の入力は編集中です。
	const InputState& inputState;
};

