#pragma once
#include "DxLib.h"

//ゲーム全般の設定
namespace Game {
#ifdef _DEBUG
	//windowモード
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = false;
#endif
	//window名
	const char* const kTitleText = " project Viking ";
	//windowサイズ
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight =1080;
	//カラーモード
	constexpr int kScreenDepth = 32;     //32 or 16
}