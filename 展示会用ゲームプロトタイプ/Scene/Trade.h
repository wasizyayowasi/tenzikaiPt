#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;

enum class PurchaseChoice
{
	kau,			/// 次へ行くボタン
	yameru,			/// 前に戻るボタン
};

class Trade : public SceneBase
{
public:
	Trade(SceneManager& manager, const InputState& input);
	~Trade();

	void update(const InputState& input);
	void draw();
private:
	int currentInputIndex;//現在選択中のインデックス
	//入力一時テーブル

	//bool isEditing = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	std::map<PurchaseChoice, std::string> purchaseChoiceTable;
};

