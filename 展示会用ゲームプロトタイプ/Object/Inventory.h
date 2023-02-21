#pragma once
#include <map>
#include <string>

class InputState;

enum class InventoryChoice
{
	Inventory1,			/// 次へ行くボタン
	Inventory2,			/// 前に戻るボタン
	Inventory3,
};

class Inventory
{
public:
	Inventory();
	~Inventory();

	void update(const InputState& input);
	void draw();

	int setCurrentInputIndex() { return currentInputIndex; }
private:
	int currentInputIndex;//現在選択中のインデックス

	std::map<InventoryChoice, std::string> inventoryChoice;
};

