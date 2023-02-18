#include "Pause.h"
#include "SceneBase.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "DxLib.h"
#include "../game.h"
#include "KeyConfigScene.h"

Pause::Pause(SceneManager& manager) : SceneBase(manager)
{
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
	if (input.isTriggered(InputType::keyconf)) {
		manager_.pushScene(new KeyConfigScene(manager_, input));
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
}
