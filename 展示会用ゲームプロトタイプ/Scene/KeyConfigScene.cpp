#include "KeyConfigScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "../game.h"
#include "../DrawFunctions.h"

namespace {
	int graphSize = 16;;
}

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input):SceneBase(manager),inputState(input),currentInputIndex(0),updateFunc(&KeyConfigScene::firstUpdate)
{
	handle = my::myLoadGraph("data/GUIGraph/huti.png");
	bottanHandle = my::myLoadGraph("data/GUIGraph/bottan.png");
	bottanHandle2 = my::myLoadGraph("data/GUIGraph/bottan1.png");
	tempHandle = my::myLoadGraph("data/objectGraph/CopperCoin.png");
	uiSound = LoadSoundMem("data/soundEffect/ui3.mp3");

	LPCSTR UIfontPath = "data/other/Silver.ttf";

	AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL);

	subjectFontHandle = CreateFontToHandle("Silver", 64, 9, -1);
	subjectFontHandle2 = CreateFontToHandle("Silver", 80, 9, -1);
	fontHandle = CreateFontToHandle("Silver", 32, 9, -1);
	fontHandle2 = CreateFontToHandle("Silver", 48, 9, -1);

	inputDeviceTable[KeySelect::padNext] = "padNext";
	inputDeviceTable[KeySelect::padNextItem] = "padNextItem";
	inputDeviceTable[KeySelect::padPrevItem] = "padPrevItem";
	inputDeviceTable[KeySelect::padPause] = "padPause";
	inputDeviceTable[KeySelect::padAttack] = "padAttack";
	inputDeviceTable[KeySelect::padAbility] = "padAbility";
	inputDeviceTable[KeySelect::padPrev] = "padPrev";
}

KeyConfigScene::~KeyConfigScene()
{
	inputState.savekeyInfo();
	DeleteFontToHandle(subjectFontHandle);
	DeleteFontToHandle(fontHandle);
	DeleteFontToHandle(subjectFontHandle2);
	DeleteFontToHandle(fontHandle2);
	DeleteGraph(handle);
	DeleteGraph(tempHandle);
	DeleteGraph(bottanHandle);
	DeleteGraph(bottanHandle2);
	DeleteSoundMem(uiSound);
}

void KeyConfigScene::update(const InputState& input)
{

	if (--time == 0) {
		moveImgX++;
		time = 8;
	}

	if (moveImgX > 5) {
		moveImgX = 2;
	}

	(this->*updateFunc)(input);
}

void KeyConfigScene::firstUpdate(const InputState& input)
{
	colorChange = false;
	isEditing = false;

	auto& configInput = const_cast<InputState&>(input);

	if (!isEditing) {
	
		const int nameCount = input.inputNameTable.size() + 2;

		if (input.isTriggered(InputType::up)) {
			ChangeVolumeSoundMem(160, uiSound);
			PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
			currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
		}
		else if (input.isTriggered(InputType::down)) {
			ChangeVolumeSoundMem(160, uiSound);
			PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
			currentInputIndex = (currentInputIndex + 1) % nameCount;
		}
		keySelectNum = currentInputIndex;
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
		colorChange = true;
		updateFunc = &KeyConfigScene::secondUpdate;
		return;
	}

	if (input.isTriggered(InputType::prev)) {
		manager_.popScene();
		return;
	}
}

void KeyConfigScene::secondUpdate(const InputState& input)
{
	auto& configInput = const_cast<InputState&>(input);

	if (input.isTriggered(InputType::next)) {
		isEditing = !isEditing;
		return;
	}
	

	if (isEditing) {
		char keystate[256];
		GetHitKeyStateAll(keystate);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);

		int idx = 0;
		InputType currentType = InputType::max;
		for (const auto& name : inputState.inputNameTable) {
			if (keySelectNum == idx) {
				currentType = name.first;
				break;
			}
			++idx;
		}

		if (padState != 0) {
			bottanNum(padState);
			configInput.rewriteInputInfo(currentType, InputCategory::pad, padState);
		}
	}

	if (!isEditing) {
		if (input.isTriggered(InputType::prev)) {
			updateFunc = &KeyConfigScene::firstUpdate;
			return;
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

	int size = GetDrawStringWidthToHandle("【変更したキーは確定後、変更されます】", strlen("【変更したキーは確定後、変更されます】"), subjectFontHandle);
	DrawStringToHandle(Game::kScreenWidth / 2 - size / 2, 160, "【変更したキーは確定後、変更されます】", 0xff0000, subjectFontHandle);

	auto y = pw_width;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : inputState.inputNameTable) {
		unsigned int color = 0xffffff;

		int font = strlen(name.second.c_str());
		fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, subjectFontHandle);

		int x = (Game::kScreenWidth / 2 - fontSize) - 100;

		if (currentInputIndex == idx) {
			isInputtypeSelected = true;
			color = 0xffff00;
			fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, subjectFontHandle2);

			int x = (Game::kScreenWidth / 2 - fontSize) - 100;
			DrawStringToHandle(x, y, name.second.c_str(), color, subjectFontHandle2);
		}
		else {
			DrawStringToHandle(x, y, name.second.c_str(), color, subjectFontHandle);
		}
	
		y += 85;
		idx++;
	}

	

	//GUI
	int graphX = Game::kScreenWidth / 2 + 100;
	int graphY = pw_width - 20;
	int graphIdX = 0;
	for (auto& inputDevice : inputDeviceTable) {
		unsigned int color = 0x00ff00;

		if (keySelectNum == graphIdX) {
			if (currentInputIndex < 7 && colorChange) {
				if (isEditing) {
					color = 0xff0000;
				}
				DrawBox(graphX - 10, graphY - 10, graphX + 210, graphY + 90, color, false);
			}
		}

		DrawGraph(graphX, graphY, handle, true);

		graphY += 85;
		graphIdX++;
		if (graphIdX == 7) {
			graphX += 300;
			graphY = pw_width - 20;
		}
	}

	int bottanSetHeight = pw_width + 20;
	for (const auto& name : inputState.inputNameTable) {
		int bottanSetWidth = (Game::kScreenWidth / 2  + 200);
		auto type = name.first;
		auto it = inputState.tempMapTable.find(type);

		
		for (const auto elem : it->second) {
			if (elem.cat == InputCategory::pad) {
				bottanNum(elem.id);
				my::myDrawRectRotaGraph(bottanSetWidth, bottanSetHeight, imgX * 16, (imgY) * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			}
		}

		bottanSetHeight += 85;
		++idx;
	}

	int width = GetDrawStringWidthToHandle("確定します", strlen("確定します"), fontHandle);
	int editingWidth = Game::kScreenWidth / 2 - width / 2;

	int color = 0xffffff;
	int keyColor = 0xffffff;
	if (currentInputIndex == 7) {
		color = 0xffff00;
		width = GetDrawStringWidthToHandle("確定します", strlen("確定します"), fontHandle2);
		editingWidth = Game::kScreenWidth / 2 - width / 2;
		DrawStringToHandle(editingWidth, y, "確定します", color, fontHandle2);
	}
	else {
		DrawStringToHandle(editingWidth, y, "確定します", color, fontHandle);
	}
	
	y += 32;

	width = GetDrawStringWidthToHandle("キーリセット", strlen("キーリセット"), fontHandle);
	editingWidth = Game::kScreenWidth / 2 - width / 2;

	if(currentInputIndex == 8) {
		keyColor = 0xffff00;
		width = GetDrawStringWidthToHandle("キーリセット", strlen("キーリセット"), fontHandle2);
		editingWidth = Game::kScreenWidth / 2 - width / 2;
		DrawStringToHandle(editingWidth, y, "キーリセット", keyColor, fontHandle2);
	}
	else {
	DrawStringToHandle(editingWidth, y, "キーリセット", keyColor, fontHandle);
	}

	DrawStringToHandle(50, Game::kScreenHeight - 55, "決定", 0xffffff, fontHandle);
	DrawStringToHandle(180, Game::kScreenHeight - 55, "戻る", 0xffffff, fontHandle);
	moveImgY = 1;
	DrawRectRotaGraph(20, Game::kScreenHeight - 50, moveImgX * graphSize, moveImgY* graphSize, graphSize, graphSize, 3.0f, 0.0f, bottanHandle2, true, false);
	moveImgY = 2;
	DrawRectRotaGraph(150, Game::kScreenHeight - 50, moveImgX * graphSize, moveImgY* graphSize, graphSize, graphSize, 3.0f, 0.0f, bottanHandle2, true, false);

}

void KeyConfigScene::bottanNum(int num)
{
	switch (num) {
	//X
	case 64:
		imgX = 1;
		imgY = 2;
		break;
	//A
	case 16:
		imgX = 1;
		imgY = 3;
		break;
	//Y
	case 128:
		imgX = 1;
		imgY = 4;
		break;
	//B
	case 32:
		imgX = 1;
		imgY = 5;
		break;
	//上
	case 8:
		imgX = 8;
		imgY = 5;
		break;
	//下
	case 1:
		imgX = 9;
		imgY = 5;
		break;
	//左
	case 2:
		imgX = 8;
		imgY = 6;
		break;
	//右	
	case 4:
		imgX = 9;
		imgY = 6;
		break;
	//LT
	case 256:
		imgX = 21;
		imgY = 5;
		break;
	//RT
	case 512:
		imgX = 21;
		imgY = 6;
		break;
	//BACK
	case 1024:
		imgX = 10;
		imgY = 13;
		break;
	//start
	case 2048:
		imgX = 10;
		imgY = 14;
		break;

	}
}
