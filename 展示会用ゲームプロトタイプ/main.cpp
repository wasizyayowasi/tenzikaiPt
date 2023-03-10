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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//window���[�h�̐ݒ�
	ChangeWindowMode(Game::kWindowMode);

	//window���ݒ�
	SetMainWindowText(Game::kTitleText);

	//if (Game::kWindowMode) {
		//��ʃT�C�Y�̐ݒ�
		SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kScreenDepth);
	//}
	//else {
		//��ʃT�C�Y�̐ݒ�
	//	SetGraphMode(640, 400, Game::kScreenDepth);
	//}

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}


	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);


	InputState input;
	SceneManager sceneManager;
	sceneManager.changeScene(new TitleScene(sceneManager));
//	sceneManager.changeScene(new GameMain(sceneManager));
//	sceneManager.changeScene(new TutorialScene(sceneManager));
//	sceneManager.changeScene(new BossBattleScene(sceneManager));
//	sceneManager.changeScene(new Gameover(sceneManager,input,1));
//	sceneManager.changeScene(new GameClear(sceneManager));

	while (ProcessMessage() == 0) {

		LONGLONG time = GetNowHiPerformanceCount();
		//��ʂ̃N���A
		ClearDrawScreen();


		input.update();

		sceneManager.update(input);
		sceneManager.draw();

		auto fps = GetFPS();//Frame Per Second;
		auto drawcall = GetDrawCallCount();//�`�施�ߐ�

		DrawFormatString(10, 10, 0xffffff, "FPS = %2.2f", fps);
		DrawFormatString(10, 30, 0xffffff, "DC = %d", drawcall);
		
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