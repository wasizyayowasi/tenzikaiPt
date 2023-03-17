#include "Pause.h"
#include "SceneBase.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "../game.h"
#include "KeyConfigScene.h"
#include "TitleScene.h"
#include "../DrawFunctions.h"

namespace {
	int graphSize = 16;;
}

Pause::Pause(SceneManager& manager,const InputState& input) : SceneBase(manager),inputState(input), currentInputIndex(0)
{
	choiceTable[Choice::keyConfig] = "キーコンフィグ";
	choiceTable[Choice::title] = "タイトルへ";
	choiceTable[Choice::prev] = "戻る";

	uiSound = LoadSoundMem("data/soundEffect/ui3.mp3");
	uiSound2 = LoadSoundMem("data/soundEffect/ui4.mp3");
	bottanHandle = my::myLoadGraph("data/GUIGraph/bottan.png");

	LPCSTR fontPath = "data/other/CompassPro.ttf";
	LPCSTR UIfontPath = "data/other/Silver.ttf";

	AddFontResourceEx(fontPath, FR_PRIVATE, NULL);
	AddFontResourceEx(UIfontPath, FR_PRIVATE, NULL);
	
	fontHandle = CreateFontToHandle("Silver", 48, 3, -1);
	fontHandle2 = CreateFontToHandle("Silver", 64, 3, -1);
	titleFont = CreateFontToHandle("CompassPro", 64, -1, -1);
	titleFont2 = CreateFontToHandle("CompassPro", 200, -1, -1);

	titleWidth = GetDrawStringWidthToHandle("P  R  O  J  E  C  T", strlen("P  R  O  J  E  C  T"), titleFont);
	titleWidth2 = GetDrawStringWidthToHandle("VIKING", strlen("VIKING"), titleFont2);
}

Pause::~Pause()
{
	DeleteFontToHandle(fontHandle);
	DeleteFontToHandle(titleFont);
	DeleteFontToHandle(fontHandle2);
	DeleteFontToHandle(titleFont2);
	DeleteSoundMem(uiSound);
	DeleteSoundMem(uiSound2);
	DeleteGraph(bottanHandle);
}

void Pause::update(const InputState& input)
{
	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
		return;
	}

	const int nameCount = static_cast<int>(choiceTable.size());

	
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
	
	if (input.isTriggered(InputType::next)) {
		ChangeVolumeSoundMem(180, uiSound2);
		PlaySoundMem(uiSound2, DX_PLAYTYPE_BACK);
		switch (currentInputIndex) {
		case 0:
			manager_.pushScene(new KeyConfigScene(manager_, input));
			break;
		case 1:
			StopMusic();
			manager_.changeScene(new TitleScene (manager_));
			return;
		case 2:
			manager_.popScene();
			return;
		}
	}

	if (input.isTriggered(InputType::prev)) {
		manager_.popScene();
		return;
	}

	if (--time == 0) {
		moveImgX++;
		time = 8;
	}

	if (moveImgX > 6) {
		moveImgX = 3;
	}
}

void Pause::draw()
{
	constexpr int pw_width = 256;						//ポーズ枠の幅
	constexpr int pw_height = 144;						//ポーズ枠の高さ
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width);	//ポーズ枠に左
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height);	//ポーズ枠上

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//乗算合成
	//ポーズウィンドウセロファン(黒い)
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth / 2 + 5, Game::kScreenHeight / 5 + 5, "P  R  O  J  E  C  T", 0x888833, titleFont);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth2 / 2 + 5, Game::kScreenHeight / 5 + 37, "VIKING", 0x888833, titleFont2);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth / 2, Game::kScreenHeight / 5, "P  R  O  J  E  C  T", 0xffffff, titleFont);
	DrawStringToHandle(Game::kScreenWidth / 2 - titleWidth2 / 2, Game::kScreenHeight / 5 + 32, "VIKING", 0xffffff, titleFont2);

	auto y = Game::kScreenHeight / 2 + 180;
	
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : choiceTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		int font = strlen(name.second.c_str());
		fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, fontHandle);
		int x = Game::kScreenWidth / 2 - fontSize / 2;

		//選択された時の処理
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			fontSize = GetDrawStringWidthToHandle(name.second.c_str(), font, fontHandle2);
			x = Game::kScreenWidth / 2 - fontSize / 2;
			DrawStringToHandle(x, y, name.second.c_str(), color, fontHandle2);
		}
		else {
			//各キーの表示
			DrawStringToHandle(x, y, name.second.c_str(), color, fontHandle);
		}

		y += 80;
		++idx;
	}

	DrawStringToHandle(Game::kScreenWidth - 200, Game::kScreenHeight - 55, "決定", 0xffffff, fontHandle);
	DrawStringToHandle(Game::kScreenWidth - 70, Game::kScreenHeight - 55, "戻る", 0xffffff, fontHandle);


	for (const auto& name : inputState.inputNameTable) {
		int bottanSetWidth = (Game::kScreenWidth / 2 + 200);
		auto type = name.first;
		auto it = inputState.tempMapTable.find(type);

		for (const auto elem : it->second) {
			if (type == InputType::next && elem.cat == InputCategory::pad) {
				nextId = elem.id;
			}
			if (type == InputType::prev && elem.cat == InputCategory::pad) {
				prevId = elem.id;
			}
		}
	}

	bottanNum(nextId);
	moveImgY = imgY;
	DrawRectRotaGraph(Game::kScreenWidth - 230, Game::kScreenHeight - 50, moveImgX * graphSize, moveImgY * graphSize, graphSize, graphSize, 3.0f, 0.0f, bottanHandle, true, false);

	bottanNum(prevId);
	moveImgY = imgY;
	DrawRectRotaGraph(Game::kScreenWidth - 100, Game::kScreenHeight - 50, moveImgX * graphSize, moveImgY * graphSize, graphSize, graphSize, 3.0f, 0.0f, bottanHandle, true, false);

}

void Pause::bottanNum(int num)
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