#include "Trade.h"
#include "SceneManager.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "Shop.h"
#include "../Object/Player.h"

constexpr int defaultTime = 120;

Trade::Trade(SceneManager& manager, const InputState& input, Player* dPlayer, int amountOfMoney, int num) : SceneBase(manager),inputState(input), player(dPlayer),amount(amountOfMoney),itemNum(num)
{
	purchaseChoiceTable[PurchaseChoice::kau] = "買う";
	purchaseChoiceTable[PurchaseChoice::yameru] = "やめとく";
}

Trade::~Trade()
{
}

void Trade::update(const InputState& input)
{
	const int nameCount = purchaseChoiceTable.size();


	if (buy == false) {
		if (input.isTriggered(InputType::left)) {
			currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
		}
		else if (input.isTriggered(InputType::right)) {
			currentInputIndex = (currentInputIndex + 1) % nameCount;
		}
	}

	if (input.isTriggered(InputType::next)) {
		if (!(itemNum == 0 && player->returnHp() > 9)) {
			if (currentInputIndex == 0) {
				if (player->setMoneyPossessed() - amount >= 0) {
					player->setItemControl(itemNum);
					player->setMoney(amount);
					textNum = 0;
					textDisplayTime = defaultTime;
					buy = true;
				}
				else {
					textNum = 1;
					textDisplayTime = defaultTime;
				}
			}
			else {
				manager_.popScene();
				return;
			}
		}
		else {
			textNum = 2;
			manager_.popScene();
			return;
		}
	}
	

}

void Trade::draw()
{

	constexpr int pw_width = Game::kScreenWidth / 2 - 200;						//ポーズ枠の幅
	constexpr int pw_height = Game::kScreenHeight / 2 - 200;						//ポーズ枠の高さ
	constexpr int pw_start_x = Game::kScreenWidth / 2 + 200;	//ポーズ枠に左
	constexpr int pw_start_y = Game::kScreenHeight / 2 + 200;	//ポーズ枠上

	auto y = Game::kScreenHeight / 2 + 200;
	int x = pw_width + 410;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : purchaseChoiceTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		//選択された時の処理
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			DrawString(x - 20, y, "⇒", 0xff0000);
		}

		//各キーの表示
		if (currentInputIndex == idx) {
			x += offset;
		}
		DrawString(x, y, name.second.c_str(), color);

		if (currentInputIndex == idx) {
			x -= offset;
		}

		x += 150;
		++idx;
	}

	y += 20;
	if (!isInputtypeSelected) {
		int yOffset = 0;
		if (currentInputIndex == purchaseChoiceTable.size() + 1) {
			yOffset = 20;
		}
		DrawString(pw_width + 90, y + yOffset, "⇒", 0xff0000);
	}

	if (textDisplayTime != 0) {
		if (--textDisplayTime > 0) {
			textDraw(textNum);
		}
		else {
			manager_.popScene();
			return;
		}
	}
}

void Trade::textDraw(int num)
{
	switch (num) {
	case 0:
		DrawString(Game::kScreenWidth / 2 - 50, 270, "毎度あり", 0xffffff);
		break;
	case 1:
		DrawString(Game::kScreenWidth / 2 - 50, 270, "金ねーじゃん", 0xffffff);
		break;
	case 2:
		DrawString(Game::kScreenWidth / 2 - 50, 270, "品切れだ", 0xffffff);
		break;
	}
}
