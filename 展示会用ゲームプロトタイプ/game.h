#pragma once
#include "DxLib.h"

//ゲーム全般の設定
namespace Game {
	//windowモード
	constexpr bool kWindowMode = true;
	//window名
	const char* const kTitleText = " ";
	//windowサイズ
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight =1080;
	//カラーモード
	constexpr int kScreenDepth = 32;     //32 or 16
}