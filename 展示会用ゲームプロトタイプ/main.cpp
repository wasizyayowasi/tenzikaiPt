#include "DxLib.h"
#include "game.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "InputState.h"

//消す
#include "Scene/GameMain.h"
#include "Scene/BossBattleScene.h"
#include "Scene/TutorialScene.h"
#include "Scene/Gameover.h"
#include "Scene/GameClear.h"


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//windowモードの設定
	ChangeWindowMode(Game::kWindowMode);

	//window名設定
	SetMainWindowText(Game::kTitleText);

	//if (Game::kWindowMode) {
		//画面サイズの設定
		SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kScreenDepth);
	//}
	//else {
		//画面サイズの設定
	//	SetGraphMode(640, 400, Game::kScreenDepth);
	//}

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}


	//ダブルバッファモード
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
		//画面のクリア
		ClearDrawScreen();


		input.update();

		sceneManager.update(input);
		sceneManager.draw();

		auto fps = GetFPS();//Frame Per Second;
		auto drawcall = GetDrawCallCount();//描画命令数

		DrawFormatString(10, 10, 0xffffff, "FPS = %2.2f", fps);
		DrawFormatString(10, 30, 0xffffff, "DC = %d", drawcall);
		
		//裏画面を表画面に切り替える
		ScreenFlip();

		//escを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;

		//fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667) {

		}

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}