#include "KeyConfigScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "../game.h"
#include "../DrawFunctions.h"

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),inputState(input),currentInputIndex(0)
{
	handle = my::myLoadGraph("data/GUIGraph/huti.png");
	bottanHandle = my::myLoadGraph("data/GUIGraph/bottan.png");

	LPCSTR UIfontPath = "data/other/Silver.ttf";

	if (AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL) > 0)
	{
	}

	subjectFontHandle = CreateFontToHandle("Silver", 64, 9, -1);
	fontHandle = CreateFontToHandle("Silver", 32, 9, -1);

	inputDeviceTable[KeySelect::keyNext] = "keyNext";
	inputDeviceTable[KeySelect::keyNextItem] = "keyNextItem";
	inputDeviceTable[KeySelect::keyPrevItem] = "keyPrevItem";
	inputDeviceTable[KeySelect::keyPause] = "keyPause";
	inputDeviceTable[KeySelect::keyAttack] = "keyAttack";
	inputDeviceTable[KeySelect::keyAbility] = "keyAbility";

	inputDeviceTable[KeySelect::padNext] = "padNext";
	inputDeviceTable[KeySelect::padNextItem] = "padNextItem";
	inputDeviceTable[KeySelect::padPrevItem] = "padPrevItem";
	inputDeviceTable[KeySelect::padPause] = "padPause";
	inputDeviceTable[KeySelect::padAttack] = "padAttack";
	inputDeviceTable[KeySelect::padAbility] = "padAbility";
}

KeyConfigScene::~KeyConfigScene()
{
	inputState.savekeyInfo();
	DeleteFontToHandle(subjectFontHandle);
	DeleteFontToHandle(fontHandle);
	DeleteGraph(handle);
	DeleteGraph(bottanHandle);
}

void KeyConfigScene::update(const InputState& input)
{
	auto& configInput = const_cast<InputState&>(input);

	if (!isEditing) {
		/*if (input.isTriggered(InputType::attack)) {
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

		}*/

		const int nameCount = input.inputNameTable.size() + 2;

		if (input.isTriggered(InputType::up)) {
			currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
		}
		else if (input.isTriggered(InputType::down)) {
			currentInputIndex = (currentInputIndex + 1) % nameCount;
		}
	}

	/*if (!isEditing) {
		const int nameCount = inputDeviceTable.size();

		if (input.isTriggered(InputType::up)) {
			Y = ((Y - 1) + nameCount) % nameCount;
		}
		else if (input.isTriggered(InputType::down)) {
			Y = (Y + 1) % nameCount;
		}
		if (input.isTriggered(InputType::left)) {
			Y = ((Y - 6) + nameCount) % nameCount;
		}
		else if (input.isTriggered(InputType::right)) {
			Y = (Y + 6) % nameCount;
		}
	}*/

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


	/*if (isEditing) {
		char keystate[256];
		GetHitKeyStateAll(keystate);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);
		auto mouseState = GetMouseInput();

		int idx = 0;
		InputType currentType = InputType::max;
		for (const auto& name : inputState.inputNameTable) {
			if (Y == idx) {
				currentType = name.first;
				break;
			}
			if (Y == 6) {
				idx = 0;
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
	}*/
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

		y += 85;
		idx++;
	}

	

	//GUI
	int graphX = ((Game::kScreenWidth / 3 - fontSize) + 20) + 200;
	int graphY = pw_width - 20;
	int graphIdX = 0;
	for (auto& inputDevice : inputDeviceTable) {

		if (Y == graphIdX) {
			DrawString(graphX - 20, graphY, "->", 0xff0000);
		}

		DrawGraph(graphX, graphY, handle, true);

		graphY += 85;
		graphIdX++;
		if (graphIdX == 6) {
			graphX += 300;
			graphY = pw_width - 20;
		}
	}


	
	int bottanSetHeight = pw_width + 20;
	for (const auto& name : inputState.inputNameTable) {
		int bottanSetWidth = ((Game::kScreenWidth / 3 - fontSize) + 20) + 200 + 100;
		auto type = name.first;
		auto it = inputState.tempMapTable.find(type);

		for (const auto elem : it->second) {

			if (elem.cat == InputCategory::keybd) {
				bottanSetWidth += 300;
				//DrawGraph(bottanSetWidth, bottanSetHeight, bottanHandle, true);
				my::myDrawRectRotaGraph(bottanSetWidth, bottanSetHeight, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			}
			else if (elem.cat == InputCategory::pad) {
				bottanSetWidth -= 300;
				my::myDrawRectRotaGraph(bottanSetWidth, bottanSetHeight, imgX * 16, (imgY + 1) * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
				//DrawFormatStringToHandle(bottanSetWidth, bottanSetHeight, color, fontHandle, "pad = %d", elem.id);
			}
			
		}
		bottanSetHeight += 85;
		++idx;
	}


	//キー設定外のこと
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

void KeyConfigScene::bottanNum(int num)
{
	switch (num) {
	//X
	case 0:
		imgX = 1;
		imgY = 2;
		break;
	//A
	case 1:
		imgX = 1;
		imgY = 3;
		break;
	//Y
	case 2:
		imgX = 1;
		imgY = 4;
		break;
	//B
	case 3:
		imgX = 1;
		imgY = 5;
		break;
	//上
	case 4:
		imgX = 8;
		imgY = 5;
		break;
	//下
	case 5:
		imgX = 9;
		imgY = 5;
		break;
	//左
	case 6:
		imgX = 8;
		imgY = 6;
		break;
	//右	
	case 7:
		imgX = 9;
		imgY = 6;
		break;
	//LT
	case 8:
		imgX = 21;
		imgY = 3;
		break;
	//RT
	case 9:
		imgX = 21;
		imgY = 4;
		break;
	//BACK
	case 10:
		imgX = 10;
		imgY = 13;
		break;
	//start
	case 11:
		imgX = 10;
		imgY = 14;
		break;

	}
}
