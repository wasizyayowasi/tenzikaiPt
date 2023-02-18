#pragma once
#include "SceneBase.h"

class InputState;
class SceneManager;

class Pause : public SceneBase
{
public:
	Pause(SceneManager& manager);
	virtual ~Pause();

	virtual void update(const InputState& input)override;
	void draw();

private:
};

