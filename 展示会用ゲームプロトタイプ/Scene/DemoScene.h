#pragma once
#include "SceneBase.h"


class DemoScene : public SceneBase
{
public:
	DemoScene(SceneManager& manager);
	~DemoScene();

	void update(const InputState& input);
	void draw();

	void normalUpdate(const InputState& input);
private:
	int demoMovieHandle = -1;
	int UIFontHandle = -1;

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	int fadeValue = 255;

	int ultimateTimer = 120;

	unsigned int FadeColor = 0x000000;//フェード色黒

	void fadeInUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	void(DemoScene::* updateFunc)(const InputState& input);
};

