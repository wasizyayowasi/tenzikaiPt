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
	int fadeValue_ = 255;//����`�̃u�����h�

	unsigned int FadeColor = 0x000000;//�t�F�[�h�F��



	//�t�F�[�h�C���̎���update�֐�
	void fadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�update�֐�
	void normalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g����update�֐�
	void fadeOutUpdate(const InputState& input);

	//update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};

