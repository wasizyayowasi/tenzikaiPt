#pragma once
#include "DxLib.h"

//�Q�[���S�ʂ̐ݒ�
namespace Game {
#ifdef _DEBUG
	//window���[�h
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = false;
#endif
	//window��
	const char* const kTitleText = " project Viking ";
	//window�T�C�Y
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight =1080;
	//�J���[���[�h
	constexpr int kScreenDepth = 32;     //32 or 16
}