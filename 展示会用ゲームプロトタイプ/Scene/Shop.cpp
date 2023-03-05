#include "Shop.h"
#include "DxLib.h"
#include "../game.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "Trade.h"
#include "../Object/Player.h"



Shop::Shop(SceneManager& manager, const InputState& input, Player* dPlayer, int pHandle, int gHandle,int hHandle, int rHandle) : SceneBase(manager), inputState(input), player(dPlayer),portionHandle(pHandle),guiHandle(gHandle), hpHandle(hHandle),repairHandle(rHandle)
{
	shopTable[ProductList::heart] = "ハート";
	shopTable[ProductList::block] = "修復アイテム";
	shopTable[ProductList::potion] = "回復アイテム";

	priceTable[Price::heart] = "500円";
	priceTable[Price::block] = "500円";
	priceTable[Price::potion] = "1500円";
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
		selecting = true;
		return;
	}

	if (isEditing) {
		switch (currentInputIndex) {
		case 0:
			amount = 500;
			break;
		case 1:
			amount = 500;
			break;
		case 2:
			amount = 1500;
			break;
		}
		if (!(currentInputIndex == 0 && player->returnHp() > 9)) {
			manager_.pushScene(new Trade(manager_, input, player, amount, currentInputIndex));
		}
		else {
			int aiu = 0;
		}
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
	//商品
	DrawBox(goodsWidth, goodsHeight, goodsX + 50, goodsY, 0x000000, true);
	//商品紹介ウィンドウ
	DrawBox(goodsWidth + 300, goodsHeight, shopkeeperX, goodsY, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

	

	//商品の描画
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
			if (selecting) {
				color = 0xffff00;
			}
			DrawString(shopkeeperWidth + 10, y, "⇒", 0xff0000);
			DrawString(shopkeeperWidth + 20 + offset + 320, shopkeeperWidth, name.second.c_str(), 0xffffff);
			DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 5.0f, 0.0f, guiHandle, true, false);
			switch (currentInputIndex) {
			case 0:
				DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 5.0f, 0.0f, hpHandle, true, false);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 80, "最大体力を1増加させることができる。", 0xffffff);
				break;
			case 1:
				DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 4.0f, 0.0f, repairHandle, true, false);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 80, "どろどろとした何か。", 0xffffff);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 100, "ゲートを閉じることができそうだ", 0xffffff);
				break;
			case 2:
				DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 5.0f, 0.0f, portionHandle, true, false);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 80, "自身の体力を5回復することができる。", 0xffffff);
				break;
			}
			
		}

		//各キーの表示
		int x = shopkeeperWidth + 20 + offset + 10;
		DrawString(x - 15, y, "・", color);
		DrawString(x, y, name.second.c_str(), color);

		y += 80;
		++idx;
	}

	//値段の描画
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
		DrawString(x + 150, y, price.second.c_str(), color);

		y += 80;
		++idx;
	}

	if (player->returnHp() > 9) {
		DrawBox(shopkeeperWidth + 10, 525, shopkeeperWidth + 220, 530, 0xff0000, true);
	}

	y += 20;
	if (!isInputtypeSelected) {
		int yOffset = 0;
		if (currentInputIndex == shopTable.size() + 1) {
			yOffset = 20;
		}
		DrawString(shopkeeperWidth + 150, y - 20, "⇒", 0xff0000);
	}

	DrawString(shopkeeperWidth + 180, y - 20, "退店 ", 0xffffff);

	DrawString(shopkeeperWidth + 10, shopkeeperHeight + 10, "店長", 0xffffff);
	DrawString(shopkeeperWidth + 200, shopkeeperHeight + 80, "おめぇ何かにくれてやるものなんか何一つね〜んだよ。あっち行け。シッシッ", 0xffffff);

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
	DrawBox(goodsWidth, goodsHeight, goodsX + 50, goodsY, 0xffffff, false);
	DrawBox(goodsWidth + 300, goodsHeight, shopkeeperX, goodsY, 0xffffff, false);
}
