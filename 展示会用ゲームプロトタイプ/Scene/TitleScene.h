#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>
#include <map>

enum class choice
{
	main,			/// 次へ行くボタン
	tutorial,		/// 前に戻るボタン
	option,
	end,			/// ポーズボタン
};

class TitleScene : public SceneBase
{
public:

	TitleScene(SceneManager& manager);
	~TitleScene() {}

	void update(const InputState& input);
	void draw();

	void choiceScene(const InputState& input);
	void choiceSceneDraw();

private:
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;//黒矩形のブレンド具合

	unsigned int FadeColor = 0x000000;//フェード色黒

	int currentInputIndex = 0;//現在選択中のインデックス

	//フェードインの時のupdate関数
	void fadeInUpdate(const InputState& input);
	//通常状態のupdate関数
	void normalUpdate(const InputState& input);
	//フェードアウト時のupdate関数
	void fadeOutUpdate(const InputState& input);

	std::map<choice, std::string> sceneTable;

	//update用メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};

