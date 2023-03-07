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

	constexpr int InventoryX = Game::kScreenWidth / 2 - 96;
	constexpr int InventoryY = 950;

	int mPosY = 0;
	int bPosY = 0;
	int pPosY = 0;

	auto y = InventoryY;
	int x = InventoryX;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : inventoryChoice) {
		unsigned int color = 0x44ff44;
		unsigned int offsetY = 0;

		//選択された時の処理
		if (currentInputIndex == idx) {
			isInputtypeSelected = true;
			color = 0xff0000;
			offsetY = -10;
		}

		//各キーの表示
		DrawBox(x, y + offsetY, x + 64, y + 64 + offsetY, color, true);
		DrawRotaGraph(x + 32, y + 32 + offsetY, 2.0f, 0.0f, guiHandle, true, false);

		x += 64;
		++idx;
	}

	switch (currentInputIndex) {
	case 0:
		mPosY = -10;
		break;
	case 1:
		bPosY = -10;
		break;
	case 2:
		pPosY = -10;
		break;
	}

	if(!hachete){
		DrawRotaGraph(InventoryX + 32, InventoryY + 32 + mPosY, 1.3f, 5.6f, hacheteHandle, true, false);
	}
	if (repairBlock > 0) {
		DrawRotaGraph(InventoryX + 96, InventoryY + 32 + bPosY, 1.0f, 0.0f, repairHandle, true, false);
		DrawFormatString(Game::kScreenWidth / 2 + 8, 993 + bPosY, 0x000000, "x%d", repairBlock);
	}
	if (recoveryItem > 0) {
		DrawRotaGraph(InventoryX + 160, InventoryY + 32 + pPosY, 2.0f, 0.0f, portionHandle, true, false);
		DrawFormatString(Game::kScreenWidth / 2 + 74, 993 + pPosY, 0x000000, "x%d", recoveryItem);
	}
	
}
