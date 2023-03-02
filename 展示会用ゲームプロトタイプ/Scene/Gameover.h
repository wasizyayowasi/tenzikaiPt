#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;

enum class GameOverChoice
{
	continueMain,			/// ���֍s���{�^��
	end,			/// �O�ɖ߂�{�^��
};

class Gameover : public SceneBase
{
public:
	Gameover(SceneManager& manager, const InputState& input,int num);
	virtual ~Gameover(){}
	
	void update(const InputState& input);
	void draw();
private:
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	int sceneNum = 0;

	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X

	unsigned int FadeColor = 0x000000;//�t�F�[�h�F��

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);
	void continueFadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (Gameover::*)(const InputState&);
	UpdateFunc_t updateFunc_;

	const InputState& inputState;

	std::map<GameOverChoice, std::string> gameOverChoiceTable;
};

