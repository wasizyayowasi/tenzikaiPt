#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>
#include <map>

enum class choice
{
	main,			/// ���֍s���{�^��
	tutorial,		/// �O�ɖ߂�{�^��
	option,
	end,			/// �|�[�Y�{�^��
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
	int fadeValue_ = 255;//����`�̃u�����h�

	unsigned int FadeColor = 0x000000;//�t�F�[�h�F��

	int currentInputIndex = 0;//���ݑI�𒆂̃C���f�b�N�X

	//�t�F�[�h�C���̎���update�֐�
	void fadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�update�֐�
	void normalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g����update�֐�
	void fadeOutUpdate(const InputState& input);

	std::map<choice, std::string> sceneTable;

	//update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};

