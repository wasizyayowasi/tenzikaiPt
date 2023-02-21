#include "Shop.h"
#include "DxLib.h"
#include "../game.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "Trade.h"



Shop::Shop(SceneManager& manager, const InputState& input, Player* dPlayer) : SceneBase(manager), inputState(input), player(dPlayer)
{
	shopTable[ProductList::kaki] = "柿";
	shopTable[ProductList::susi] = "寿司";
	shopTable[ProductList::niku] = "肉";

	priceTable[Price::kaki] = "300円";
	priceTable[Price::susi] = "400円";
	priceTable[Price::niku] = "500円";
}

Shop::~Shop()
{
}

void Shop::update(const InputState& input)
{
	const int nameCount = shopTable.size() + 1;

	if (input.isTriggered(InputType::up)) {
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}

	//この時もう、確定しますを選択している
	if (currentInputIndex == shopTable.size()) {
		if (input.isTriggered(InputType::next)) {
			manager_.popScene();
			return;
		}
	}

	if (input.isTriggered(InputType::next)) {
		isEditing = !isEditing;
		Selecting = true;
		return;
	}

	if (isEditing) {
		switch (currentInputIndex) {
		case 0:
			amount = 300;
			break;
		case 1:
			amount = 400;
			break;
		case 2:
			amount = 500;
			break;
		}
		manager_.pushScene(new Trade(manager_, input,player,amount,currentInputIndex));
		isEditing = false;
	}

	if (input.isTriggered(InputType::shot)) {
		manager_.popScene();
		return;
	}
}

void Shop::draw()
{

	constexpr int shopkeeperWidth = 480;
	constexpr int shopkeeperHeight = 170;
	constexpr int shopkeeperX = (Game::kScreenWidth - shopkeeperWidth);
	constexpr int shopkeeperY = shopkeeperHeight * 2;

	constexpr int goodsWidth = 480;
	constexpr int goodsHeight = shopkeeperHeight * 2 + 100;
	constexpr int goodsX = Game::kScreenWidth / 2 - 300;
	constexpr int goodsY = Game::kScreenHeight / 2 + 250;

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	//店主セリフ部分
	DrawBox(shopkeeperWidth, shopkeeperHeight, shopkeeperX, shopkeeperY, 0x000000, true);
	DrawBox(goodsWidth, goodsHeight, goodsX, goodsY, 0x000000, true);
	DrawBox(goodsWidth + 300, goodsHeight, shopkeeperX, goodsY, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す


	auto y = 520;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : shopTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		//選択された時の処理
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			if (Selecting) {
				color = 0xffff00;
			}
			DrawString(shopkeeperWidth + 10, y, "⇒", 0xff0000);
			DrawString(shopkeeperWidth + 20 + offset + 320, shopkeeperWidth, name.second.c_str(), 0xffffff);
		}

		//各キーの表示
		int x = shopkeeperWidth + 20 + offset + 10;
		DrawString(x - 15, y, "・", color);
		DrawString(x, y, name.second.c_str(), color);

		y += 80;
		++idx;
	}


	y = 520;
	idx = 0;
	for (const auto& price : priceTable) {
		unsigned int color = 0xffffff;

		//選択された時の処理
		if (currentInputIndex == idx) {
			isInputtypeSelected = true;
			DrawString(shopkeeperWidth + 10, y, "⇒", 0xff0000);
		}

		//各キーの表示
		int x = shopkeeperWidth + 20;
		DrawString(x + 100, y, price.second.c_str(), color);

		y += 80;
		++idx;
	}

	

	y += 20;
	if (!isInputtypeSelected) {
		int yOffset = 0;
		if (currentInputIndex == shopTable.size() + 1) {
			yOffset = 20;
		}
		DrawString(shopkeeperWidth + 100, y - 20, "⇒", 0xff0000);
	}

	DrawString(shopkeeperWidth + 130, y - 20, "退店 ", 0xffffff);

	DrawString(shopkeeperWidth + 10, shopkeeperHeight + 10, "店長", 0xffffff);
	DrawString(shopkeeperWidth + 200, shopkeeperHeight + 80, "おめぇ何かにくれてやるものなんか何一つね～んだよ。あっち行け。シッシッ", 0xffffff);

	if (--timer < 0) {
		timer = 180;
	}

	if (timer > 0) {
		if ((timer / 30) % 2 == 1) {
			DrawString(shopkeeperX - 140, shopkeeperY - 30, "  shoping...", 0xffff88);
		}
	}

	//ポーズウィンドウ枠線
	DrawBox(shopkeeperWidth, shopkeeperHeight, shopkeeperX, shopkeeperY, 0xffffff, false);
	DrawBox(goodsWidth, goodsHeight, goodsX, goodsY, 0xffffff, false);
	DrawBox(goodsWidth + 300, goodsHeight, shopkeeperX, goodsY, 0xffffff, false);
}
