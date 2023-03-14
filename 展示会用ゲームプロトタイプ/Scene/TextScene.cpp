#include "TextScene.h"
#include "DxLib.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "SceneManager.h"
#include <string>

namespace {
	const char* const textNum1textCount1 = "���O���񂻂��ŉ����Ă�";
	const char* const textNum1textCount2 = "���̉�������������̂�";
	const char* const textNum1textCount3 = "�������̃|�X�g�̗��ɉB��Ȃ���";

	const char* const textNum2textCount1 = "���O���񂢂�����������Ă邶��Ȃ���";
	const char* const textNum2textCount2 = "���傢�Ƃ����̉�������ގ����Ă���Ȃ���";

	const char* const textNum3textCount1 = "���O���񂠂̃{���{��������Ԃ������邩�H";
	const char* const textNum3textCount2 = "���͂Ȃ����߂Ă��邩�炩�킩��Ȃ���";
	const char* const textNum3textCount3 = "���̋�Ԃ��炳�����̉��������o�Ă��Ă���";
	const char* const textNum3textCount4 = "���O����Ȃ�O�̊X���݂ɖ߂���͂���";
	const char* const textNum3textCount5 = "���񂾂�";

	const char* const textNum4textCount1 = "���O����Ȃ��Ȃ������r�����Ă邶��Ȃ���";
	const char* const textNum4textCount2 = "���O��������z���Ċ肢������";
	const char* const textNum4textCount3 = "���ݐi�s�`�Œ��������������ɏP���Ă���";
	const char* const textNum4textCount4 = "���̋�Ԃ��󂵂ĉ������̊X�����߂��Ă���";
	const char* const textNum4textCount5 = "���񂾂�";
}

TextScene::TextScene(SceneManager& manager, const InputState& input, int num, Vec2 tempOffset, int handle) : inputState(input),SceneBase(manager),textNum(num),offset(tempOffset), bottanHandle(handle)
{
	LPCSTR fontPath = "data/other/Silver.ttf";
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}

	textHandle = CreateFontToHandle("Silver", 32, 9, -1);

	switch (textNum) {
	case 0:
		textMaxCount = 3;
		break;
	case 1:
		textMaxCount = 2;
		break;
	case 2:
		textMaxCount = 4;
		break;
	case 3:
		textMaxCount = 4;
		break;
	}
}

TextScene::~TextScene()
{
	DeleteFontToHandle(textHandle);
}

void TextScene::update(const InputState& input)
{
	if (--time < 0) {
		imgX++;
		time = 8;
	}

	if (input.isTriggered(InputType::next)) {
		textCount++;
	}

	if (textCount > textMaxCount) {
		manager_.popScene();
		return;
	}

}

void TextScene::draw()
{
	switch (textNum) {
	case 0:

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
		DrawBox(660 + offset.x, 650, 1040 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�
		DrawBox(660 + offset.x, 650, 1040 + offset.x, 720, 0xffffff, false);

		
		switch (textCount) {
		case 0:
			DrawString(750 + offset.x, 680, "���O���񂻂��ŉ����Ă�", 0xffffff);
			break;
		case 1:
			DrawString(750 + offset.x, 680, "���̉�������������̂�", 0xffffff);
			break;
		case 2:
			DrawString(730 + offset.x, 680, "�������̃|�X�g�̗��ɉB��Ȃ���", 0xffffff);
			break;
		case 3:
			imgY = 1;
			if (imgX > 5) {
				imgX = 2;
			}
			DrawRectRotaGraph(690 + offset.x, 685, imgX * 16, imgY * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
			DrawString(705 + offset.x, 680, "�������Ă���ԓG�Ɍ�����Ȃ��Ȃ�܂�", 0xffffff);
			break;
		}


		break;
	case 1:

		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
		DrawBox(1600 + offset.x, 570, 2000 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�
		DrawBox(1600 + offset.x, 570, 2000 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(1650 + offset.x, 640, "���O���񂢂�����������Ă邶��Ȃ���", 0xffffff);
			break;
		case 1:
			DrawString(1630 + offset.x, 640, "���傢�Ƃ����̉�������ގ����Ă���Ȃ���", 0xffffff);
			break;
		case 2:
			DrawRectRotaGraph(1730 + offset.x, 640, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			DrawString(1690 + offset.x, 680, "���𓊂���", 0xffffff);
			DrawRectRotaGraph(1860 + offset.x, 640, imgX * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			DrawString(1820 + offset.x, 680, "�a�����", 0xffffff);
			DrawString(1750 + offset.x, 580, "�G��|����", 0xffffff);
			DrawString(1785 + offset.x, 650, "or", 0xffffff);
			break;
		}
		break;
	case 2:
		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
		DrawBox(2300 + offset.x, 570, 2700 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�
		DrawBox(2300 + offset.x, 570, 2700 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(2330 + offset.x, 640, "���O���񂠂̃{���{��������Ԃ������邩�H", 0xffffff);
			break;
		case 1:
			DrawString(2340 + offset.x, 640, "���͂Ȃ����߂Ă���̂��킩��Ȃ���", 0xffffff);
			break;
		case 2:
			DrawString(2330 + offset.x, 640, "���̋�Ԃ��炳�����̉��������o�Ă��Ă���", 0xffffff);
			break;
		case 3:
			DrawString(2340 + offset.x, 640, "���O����Ȃ�O�̊X���݂ɖ߂���͂���", 0xffffff);
			break;
		case 4:
			DrawString(2465 + offset.x, 640, "���񂾂�", 0xffffff);
			break;
		}
		break;
	case 3:
		
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
		DrawBox(3200 + offset.x, 570, 3600 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�
		DrawBox(3200 + offset.x, 570, 3600 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(3230 + offset.x, 640, "���O����Ȃ��Ȃ������r�����Ă邶��Ȃ���", 0xffffff);
			break;
		case 1:
			DrawString(3250 + offset.x, 640, "���O��������z���Ċ肢������", 0xffffff);
			break;
		case 2:
			DrawString(3230 + offset.x, 640, "���ݐi�s�`�Œ��������������ɏP���Ă���", 0xffffff);
			break;
		case 3:
			DrawString(3220 + offset.x, 640, "���̋�Ԃ��󂵂ĉ������̊X�����߂��Ă���", 0xffffff);
			break;
		case 4:
			DrawString(3365 + offset.x, 640, "���񂾂�", 0xffffff);
			break;
		}
		break;
	}
}
