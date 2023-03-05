#include "TextScene.h"
#include "DxLib.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "SceneManager.h"

TextScene::TextScene(SceneManager& manager, const InputState& input, int num, Vec2 tempOffset, int handle) : inputState(input),SceneBase(manager),textNum(num),offset(tempOffset), bottanHandle(handle)
{
	
	switch (textNum) {
	case 0:
		textMaxCount = 2;
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
			DrawString(750 + offset.x, 665, "���O���񂻂��ŉ����Ă�", 0xffffff);
			DrawString(750 + offset.x, 690, "���̉�������������̂�", 0xffffff);
			break;
		case 1:
			DrawString(730 + offset.x, 680, "�������̃|�X�g�̗��ɉB��Ȃ���", 0xffffff);
			break;
		case 2:
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
		DrawBox(4000 + offset.x, 570, 4400 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�
		DrawBox(4000 + offset.x, 570, 4400 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(4030 + offset.x, 640, "���O���񂠂̃{���{��������Ԃ������邩�H", 0xffffff);
			break;
		case 1:
			DrawString(4040 + offset.x, 640, "���͂Ȃ����߂Ă��邩�炩�킩��Ȃ���", 0xffffff);
			break;
		case 2:
			DrawString(4030 + offset.x, 640, "���̋�Ԃ��炳�����̉��������o�Ă��Ă���", 0xffffff);
			break;
		case 3:
			DrawString(4040 + offset.x, 640, "���O����Ȃ�O�̊X���݂ɖ߂���͂���", 0xffffff);
			break;
		case 4:
			DrawString(4165 + offset.x, 640, "���񂾂�", 0xffffff);
			break;
		}
		break;
	case 3:
		
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
		DrawBox(5100 + offset.x, 570, 5500 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�
		DrawBox(5100 + offset.x, 570, 5500 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(5130 + offset.x, 640, "���O����Ȃ��Ȃ������r�����Ă邶��Ȃ���", 0xffffff);
			break;
		case 1:
			DrawString(5150 + offset.x, 640, "���O��������z���Ă��肢������", 0xffffff);
			break;
		case 2:
			DrawString(5130 + offset.x, 640, "���ݐi�s�`�Œ��������������ɏP���Ă���", 0xffffff);
			break;
		case 3:
			DrawString(5120 + offset.x, 640, "���̋�Ԃ��󂵂ĉ������̊X�����߂��Ă���", 0xffffff);
			break;
		case 4:
			DrawString(5265 + offset.x, 640, "���񂾂�", 0xffffff);
			break;
		}
		break;
	}
}
