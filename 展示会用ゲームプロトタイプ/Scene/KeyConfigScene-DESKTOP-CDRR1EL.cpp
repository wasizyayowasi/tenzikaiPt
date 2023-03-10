#include "KeyConfigScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "../game.h"

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),inputState(input),currentInputIndex(0)
{
	LPCSTR UIfontPath = "data/other/Silver.ttf";

	if (AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL) > 0)
	{
	}

	subjectFontHandle = CreateFontToHandle("Silver", 64, 3, -1);
	fontHandle = CreateFontToHandle("Silver", 32, 3, -1);
}

KeyConfigScene::~KeyConfigScene()
{
	inputState.savekeyInfo();
	DeleteFontToHandle(subjectFontHandle);
	DeleteFontToHandle(fontHandle);
}

void KeyConfigScene::update(const InputState& input)
{
	auto& configInput = const_cast<InputState&>(input);

	if (!isEditing) {
		if (input.isTriggered(InputType::attack)) {
			manager_.popScene();
			return;
		}
		if (input.isTriggered(InputType::prevItem)) {

			configInput.rewriteInputInfo(InputType::nextItem, InputCategory::keybd, KEY_INPUT_ESCAPE);
			configInput.rewriteInputInfo(InputType::nextItem, InputCategory::pad, PAD_INPUT_A);
			configInput.rewriteInputInfo(InputType::nextItem, InputCategory::mouse, MOUSE_INPUT_RIGHT);

			static int count = 0;
			++count;
			char logstr[64] = {};
			sprintf_s(logstr, sizeof(logstr), "%d回キーが書き換えられました。\n", count);
			OutputDebugStringA(logstr);

		}

		const int nameCount = input.inputNameTable.size() + 2;

		if (input.isTriggered(InputType::up)) {
			currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
		}
		else if (input.isTriggered(InputType::down)) {
			currentInputIndex = (currentInputIndex + 1) % nameCount;
		}
	}


	//この時もう、確定しますを選択している
	if (currentInputIndex == input.inputNameTable.size()) {
		if (input.isTriggered(InputType::next)) {
			configInput.commitChangedInputInfo();
			manager_.popScene();
			return;
		}
	}
	if (currentInputIndex == input.inputNameTable.size() + 1) {
		if (input.isTriggered(InputType::next)) {
			configInput.resetInputInfo();
			return;
		}
	}

	//nextボタンでエディット中かそうじゃないかを
	//決定する
	if (input.isTriggered(InputType::next)) {
		isEditing = !isEditing;
		return;
	}

	if (isEditing) {
		char keystate[256];
		GetHitKeyStateAll(keystate);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);
		auto mouseState = GetMouseInput();

		int idx = 0;
		InputType currentType = InputType::max;
		for (const auto& name : inputState.inputNameTable) {
			if (currentInputIndex == idx) {
				currentType = name.first;
				break;
			}
			++idx;
		}

		for (int i = 0; i < 256; i++) {
			if (keystate[i]) {
				configInput.rewriteInputInfo(currentType, InputCategory::keybd, i);
				break;
			}
		}
		if (padState != 0) {
			configInput.rewriteInputInfo(currentType, InputCategory::pad, padState);
		}
		if (mouseState != 0) {
			configInput.rewriteInputInfo(currentType, InputCategory::mouse, mouseState);
		}
	}

}

void KeyConfigScene::draw()
{
	constexpr int pw_width = 288;						//ポーズ枠の幅
	constexpr int pw_height = 162;						//ポーズ枠の高さ

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

	//キーコンフィグメッセージ
	DrawString(pw_width + 10, pw_height + 10, "KeyConfig...", 0xffffaa);

	auto y = pw_width;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : inputState.inputNameTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		int font = strlen(name.second.c_str());
		fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, subjectFontHandle);

		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			if (isEditing) {
				color = 0xff0000;
			}
			DrawString(pw_width + 10, y,"⇒", 0xff0000);
		}
		//各キーの表示


		int x =  (Game::kScreenWidth / 3 - fontSize) + 20 + offset;
		DrawStringToHandle(x, y, name.second.c_str(), color,subjectFontHandle);


		auto type = name.first;
		auto it = inputState.tempMapTable.find(type);

		x += 256;
		x += 35;
		DrawStringToHandle(x, y, " : ", color,fontHandle);
		x += 40;
		for (const auto elem : it->second) {

			if (elem.cat == InputCategory::keybd) {
				DrawFormatStringToHandle(x, y, color,fontHandle, "key = %d", elem.id);
			}
			else if (elem.cat == InputCategory::pad) {
				DrawFormatStringToHandle(x, y, color, fontHandle, "pad = %d", elem.id);
			}
			else if (elem.cat == InputCategory::mouse) {
				DrawFormatStringToHandle(x, y, color, fontHandle, "mse = %d", elem.id);
			}
			x += 100;
		}
		y += 64;
		++idx;
	}

	y += 20;
	if (!isInputtypeSelected) {
		int yOffset = 0;
		if (currentInputIndex == inputState.inputNameTable.size() + 1) {
			yOffset = 20;
		}
		DrawString(pw_width + 90, y + yOffset, "⇒", 0xff0000);
	}
	//各キーの表示
	DrawString(pw_width + 100, y, " 確定します ", 0xffffff);

	y += 20;
	DrawString(pw_width + 100, y, " キーリセット ", 0xffffff);

}
