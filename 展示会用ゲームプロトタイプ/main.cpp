#include "DxLib.h"
#include "game.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "InputState.h"

//����
#include "Scene/GameMain.h"
#include "Scene/BossBattleScene.h"
#include "Scene/TutorialScene.h"
#include "Scene/Gameover.h"
#include "Scene/GameClear.h"


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//window���[�h�̐ݒ�
	ChangeWindowMode(Game::kWindowMode);

	SetWindowSizeChangeEnableFlag(true);

	//window���ݒ�
	SetMainWindowText(Game::kTitleText);

	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kScreenDepth);
	

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}


	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);


	InputState input;
	SceneManager sceneManager;
	sceneManager.changeScene(new TitleScene(sceneManager));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();
		//��ʂ̃N���A
		ClearDrawScreen();


		input.update();

		sceneManager.update(input);
		sceneManager.draw();
		
		//����ʂ�\��ʂɐ؂�ւ���
		ScreenFlip();

		//esc����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}