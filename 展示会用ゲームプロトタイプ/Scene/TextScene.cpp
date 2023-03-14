#include "TextScene.h"
#include "DxLib.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "SceneManager.h"
#include <string>

namespace {
	const char* const textNum1textCount1 = "お前さんそこで何してる";
	const char* const textNum1textCount2 = "後ろの化け物が見えんのか";
	const char* const textNum1textCount3 = "早くこのポストの裏に隠れなさい";

	const char* const textNum2textCount1 = "お前さんいい武具をもってるじゃないか";
	const char* const textNum2textCount2 = "ちょいとそこの化け物を退治してくれないか";

	const char* const textNum3textCount1 = "お前さんあのボワボワした空間が見えるか？";
	const char* const textNum3textCount2 = "今はなりを潜めているからかわからないが";
	const char* const textNum3textCount3 = "あの空間からさっきの化け物が出てきている";
	const char* const textNum3textCount4 = "お前さんなら前の街並みに戻せるはずだ";
	const char* const textNum3textCount5 = "頼んだぞ";

	const char* const textNum4textCount1 = "お前さんなかなかいい腕をしてるじゃないか";
	const char* const textNum4textCount2 = "お前さんを見越して願いがある";
	const char* const textNum4textCount3 = "現在進行形で町が化け物たちに襲われている";
	const char* const textNum4textCount4 = "あの空間を壊して俺たちの街を取り戻してくれ";
	const char* const textNum4textCount5 = "頼んだぞ";
}

TextScene::TextScene(SceneManager& manager, const InputState& input, int num, Vec2 tempOffset, int handle) : inputState(input),SceneBase(manager),textNum(num),offset(tempOffset), bottanHandle(handle)
{
	LPCSTR fontPath = "data/other/Silver.ttf";
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}

	textHandle = CreateFontToHandle("Silver", 32, 9, -1);

	switch (textNum) {
	case 0:
		textMaxCount = 3;
		break;
	case 1:
		textMaxCount = 2;
		break;
	case 2:
		textMaxCount = 4;
		break;
	case 3:
		textMaxCount = 4;
		break;
	}
}

TextScene::~TextScene()
{
	DeleteFontToHandle(textHandle);
}

void TextScene::update(const InputState& input)
{
	if (--time < 0) {
		imgX++;
		time = 8;
	}

	if (input.isTriggered(InputType::next)) {
		textCount++;
	}

	if (textCount > textMaxCount) {
		manager_.popScene();
		return;
	}

}

void TextScene::draw()
{
	switch (textNum) {
	case 0:

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
		DrawBox(660 + offset.x, 650, 1040 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
		DrawBox(660 + offset.x, 650, 1040 + offset.x, 720, 0xffffff, false);

		
		switch (textCount) {
		case 0:
			DrawString(750 + offset.x, 680, "お前さんそこで何してる", 0xffffff);
			break;
		case 1:
			DrawString(750 + offset.x, 680, "後ろの化け物が見えんのか", 0xffffff);
			break;
		case 2:
			DrawString(730 + offset.x, 680, "早くこのポストの裏に隠れなさい", 0xffffff);
			break;
		case 3:
			imgY = 1;
			if (imgX > 5) {
				imgX = 2;
			}
			DrawRectRotaGraph(690 + offset.x, 685, imgX * 16, imgY * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
			DrawString(705 + offset.x, 680, "を押している間敵に見つからなくなります", 0xffffff);
			break;
		}


		break;
	case 1:

		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
		DrawBox(1600 + offset.x, 570, 2000 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
		DrawBox(1600 + offset.x, 570, 2000 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(1650 + offset.x, 640, "お前さんいい武具をもってるじゃないか", 0xffffff);
			break;
		case 1:
			DrawString(1630 + offset.x, 640, "ちょいとそこの化け物を退治してくれないか", 0xffffff);
			break;
		case 2:
			DrawRectRotaGraph(1730 + offset.x, 640, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			DrawString(1690 + offset.x, 680, "斧を投げる", 0xffffff);
			DrawRectRotaGraph(1860 + offset.x, 640, imgX * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			DrawString(1820 + offset.x, 680, "斬りつける", 0xffffff);
			DrawString(1750 + offset.x, 580, "敵を倒そう", 0xffffff);
			DrawString(1785 + offset.x, 650, "or", 0xffffff);
			break;
		}
		break;
	case 2:
		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
		DrawBox(2300 + offset.x, 570, 2700 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
		DrawBox(2300 + offset.x, 570, 2700 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(2330 + offset.x, 640, "お前さんあのボワボワした空間が見えるか？", 0xffffff);
			break;
		case 1:
			DrawString(2340 + offset.x, 640, "今はなりを潜めているのかわからないが", 0xffffff);
			break;
		case 2:
			DrawString(2330 + offset.x, 640, "あの空間からさっきの化け物が出てきている", 0xffffff);
			break;
		case 3:
			DrawString(2340 + offset.x, 640, "お前さんなら前の街並みに戻せるはずだ", 0xffffff);
			break;
		case 4:
			DrawString(2465 + offset.x, 640, "頼んだぞ", 0xffffff);
			break;
		}
		break;
	case 3:
		
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
		DrawBox(3200 + offset.x, 570, 3600 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す
		DrawBox(3200 + offset.x, 570, 3600 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(3230 + offset.x, 640, "お前さんなかなかいい腕をしてるじゃないか", 0xffffff);
			break;
		case 1:
			DrawString(3250 + offset.x, 640, "お前さんを見越して願いがある", 0xffffff);
			break;
		case 2:
			DrawString(3230 + offset.x, 640, "現在進行形で町が化け物たちに襲われている", 0xffffff);
			break;
		case 3:
			DrawString(3220 + offset.x, 640, "あの空間を壊して俺たちの街を取り戻してくれ", 0xffffff);
			break;
		case 4:
			DrawString(3365 + offset.x, 640, "頼んだぞ", 0xffffff);
			break;
		}
		break;
	}
}
