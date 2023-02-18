#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:

	TitleScene(SceneManager& manager);
	~TitleScene() {}

	void update(const InputState& input);
	void draw();

private:
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;//黒矩形のブレンド具合

	unsigned int FadeColor = 0x000000;//フェード色黒



	//フェードインの時のupdate関数
	void fadeInUpdate(const InputState& input);
	//通常状態のupdate関数
	void normalUpdate(const InputState& input);
	//フェードアウト時のupdate関数
	void fadeOutUpdate(const InputState& input);

	//update用メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};

