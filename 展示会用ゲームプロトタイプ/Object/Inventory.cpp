#include "Inventory.h"
#include "DxLib.h"
#include "../InputState.h"
#include "../game.h"

Inventory::Inventory()
{
	inventoryChoice[InventoryChoice::Inventory1] = "Inventory1";
	inventoryChoice[InventoryChoice::Inventory2] = "Inventory2";
	inventoryChoice[InventoryChoice::Inventory3] = "Inventory3";
}

Inventory::~Inventory()
{
}

void Inventory::update(const InputState& input)
{
	const int nameCount = inventoryChoice.size();

	if (input.isTriggered(InputType::prevItem)) {
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::nextItem)) {
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}

}

void Inventory::draw()
{

	constexpr int InventoryX = 200;
	constexpr int InventoryY = 900;

	auto y = InventoryY;
	int x = InventoryX;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : inventoryChoice) {
		unsigned int color = 0x44ff44;

		//選択された時の処理
		if (currentInputIndex == idx) {
			isInputtypeSelected = true;
			color = 0xff0000;
		}

		//各キーの表示
		DrawBox(x, y, x + 64, y + 64, color, true);
		DrawBox(x + 8, y + 8, x + 56, y + 56, 0x000000, false);

		x += 64;
		++idx;
	}
}
