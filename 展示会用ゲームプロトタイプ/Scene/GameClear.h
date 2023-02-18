#pragma once
#include "SceneBase.h"

class GameClear : public SceneBase
{
public:
	GameClear(SceneManager& manager);
	virtual ~GameClear(){}

	void update(const InputState& input);
	void draw();
		
private:
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	unsigned int FadeColor = 0x000000;//フェード色黒

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (GameClear::*)(const InputState&);
	UpdateFunc_t updateFunc;
};

