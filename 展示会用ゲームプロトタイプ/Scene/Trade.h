#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;
class Player;

enum class PurchaseChoice
{
	kau,			/// 次へ行くボタン
	yameru,			/// 前に戻るボタン
};

class Trade : public SceneBase
{
public:
	Trade(SceneManager& manager, const InputState& input, Player* dPlayer,int amountOfMoney,int num);
	~Trade();

	void update(const InputState& input);
	void draw();

	int setBuy() { return buy; }
private:
	int amount = 0;
	int textNum = 0;
	int textDisplayTime = 0;
	int currentInputIndex;//現在選択中のインデックス

	int itemNum;
	//入力一時テーブル

	bool buy = false;

	//bool isEditing = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	Player* player;

	std::map<PurchaseChoice, std::string> purchaseChoiceTable;
};

