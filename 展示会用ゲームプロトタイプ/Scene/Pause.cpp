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
	constexpr int pw_width = 256;						//�|�[�Y�g�̕�
	constexpr int pw_height = 144;						//�|�[�Y�g�̍���
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width);	//�|�[�Y�g�ɍ�
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height);	//�|�[�Y�g��

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
	//�|�[�Y�E�B���h�E�Z���t�@��(����)
	DrawBox(pw_width, pw_height, pw_start_x, pw_start_y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�

	DrawString(pw_width + 10, pw_height + 10, "Pausing...", 0xffff88);

	//�|�[�Y�E�B���h�E�g��
	DrawBox(pw_width, pw_height, pw_start_x, pw_start_y, 0xffffff, false);
}
