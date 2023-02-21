#include "Pause.h"
#include "SceneBase.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "../game.h"
#include "KeyConfigScene.h"
#include "TitleScene.h"

Pause::Pause(SceneManager& manager,const InputState& input) : SceneBase(manager),inputState(input)
{
	choiceTable[Choice::keyConfig] = "キーコンフィグ";
	choiceTable[Choice::title] = "タイトルへ";
	choiceTable[Choice::prev] = "戻る";
}

Pause::~Pause()
{
}

void Pause::update(const InputState& input)
{
	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
		return;
	}

	const int nameCount = choiceTable.size();

	
	if (input.isTriggered(InputType::up)) {
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}
	
	if (input.isTriggered(InputType::next)) {
		switch (currentInputIndex) {
		case 0:
			manager_.pushScene(new KeyConfigScene(manager_, input));
			break;
		case 1:
			manager_.changeScene(new TitleScene (manager_));
			return;
		case 2:
			manager_.popScene();
			return;
		}
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
	DrawBox(pw_width, pw_height, pw_start_x, pw_start_y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//通常描画に戻す

	DrawString(pw_width + 10, pw_height + 10, "Pausing...", 0xffff88);

	//ポーズウィンドウ枠線
	DrawBox(pw_width, pw_height, pw_start_x, pw_start_y, 0xffffff, false);

	auto y = 520;
	int x = Game::kScreenWidth / 2 - pw_width;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : choiceTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		//選択された時の処理
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			DrawString(x - 50, y, "⇒", 0xff0000);
		}

		//各キーの表示
		
		DrawString(x - 15, y, "・", color);
		DrawString(x, y, name.second.c_str(), color);

		y += 80;
		++idx;
	}

	
}
