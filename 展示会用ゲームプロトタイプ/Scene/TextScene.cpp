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

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(660 + offset.x, 650, 1040 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(660 + offset.x, 650, 1040 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(750 + offset.x, 665, "‚¨‘O‚³‚ñ‚»‚±‚Å‰½‚µ‚Ä‚é", 0xffffff);
			DrawString(750 + offset.x, 690, "Œã‚ë‚Ì‰»‚¯•¨‚ªŒ©‚¦‚ñ‚Ì‚©", 0xffffff);
			break;
		case 1:
			DrawString(730 + offset.x, 680, "‘‚­‚±‚Ìƒ|ƒXƒg‚Ì— ‚É‰B‚ê‚È‚³‚¢", 0xffffff);
			break;
		case 2:
			imgY = 1;
			if (imgX > 5) {
				imgX = 2;
			}
			DrawRectRotaGraph(690 + offset.x, 685, imgX * 16, imgY * 16, 16, 16, 2.0f, 0.0f, bottanHandle, true, false);
			DrawString(705 + offset.x, 680, "‚ğ‰Ÿ‚µ‚Ä‚¢‚éŠÔ“G‚ÉŒ©‚Â‚©‚ç‚È‚­‚È‚è‚Ü‚·", 0xffffff);
			break;
		}


		break;
	case 1:

		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(1600 + offset.x, 570, 2000 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(1600 + offset.x, 570, 2000 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(1650 + offset.x, 640, "‚¨‘O‚³‚ñ‚¢‚¢•‹ï‚ğ‚à‚Á‚Ä‚é‚¶‚á‚È‚¢‚©", 0xffffff);
			break;
		case 1:
			DrawString(1630 + offset.x, 640, "‚¿‚å‚¢‚Æ‚»‚±‚Ì‰»‚¯•¨‚ğ‘Ş¡‚µ‚Ä‚­‚ê‚È‚¢‚©", 0xffffff);
			break;
		case 2:
			DrawRectRotaGraph(1730 + offset.x, 640, imgX * 16, imgY * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			DrawString(1690 + offset.x, 680, "•€‚ğ“Š‚°‚é", 0xffffff);
			DrawRectRotaGraph(1860 + offset.x, 640, imgX * 16, imgY2 * 16, 16, 16, 3.0f, 0.0f, bottanHandle, true, false);
			DrawString(1820 + offset.x, 680, "a‚è‚Â‚¯‚é", 0xffffff);
			DrawString(1750 + offset.x, 580, "“G‚ğ“|‚»‚¤", 0xffffff);
			DrawString(1785 + offset.x, 650, "or", 0xffffff);
			break;
		}
		break;
	case 2:
		imgY = 0;
		if (imgX > 5) {
			imgX = 2;
		}

		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(4000 + offset.x, 570, 4400 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(4000 + offset.x, 570, 4400 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(4030 + offset.x, 640, "‚¨‘O‚³‚ñ‚ ‚Ìƒ{ƒƒ{ƒ‚µ‚½‹óŠÔ‚ªŒ©‚¦‚é‚©H", 0xffffff);
			break;
		case 1:
			DrawString(4040 + offset.x, 640, "¡‚Í‚È‚è‚ğö‚ß‚Ä‚¢‚é‚©‚ç‚©‚í‚©‚ç‚È‚¢‚ª", 0xffffff);
			break;
		case 2:
			DrawString(4030 + offset.x, 640, "‚ ‚Ì‹óŠÔ‚©‚ç‚³‚Á‚«‚Ì‰»‚¯•¨‚ªo‚Ä‚«‚Ä‚¢‚é", 0xffffff);
			break;
		case 3:
			DrawString(4040 + offset.x, 640, "‚¨‘O‚³‚ñ‚È‚ç‘O‚ÌŠX•À‚İ‚É–ß‚¹‚é‚Í‚¸‚¾", 0xffffff);
			break;
		case 4:
			DrawString(4165 + offset.x, 640, "—Š‚ñ‚¾‚¼", 0xffffff);
			break;
		}
		break;
	case 3:
		
		SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//æZ‡¬
		DrawBox(5100 + offset.x, 570, 5500 + offset.x, 720, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//’Êí•`‰æ‚É–ß‚·
		DrawBox(5100 + offset.x, 570, 5500 + offset.x, 720, 0xffffff, false);

		switch (textCount) {
		case 0:
			DrawString(5130 + offset.x, 640, "‚¨‘O‚³‚ñ‚È‚©‚È‚©‚¢‚¢˜r‚ğ‚µ‚Ä‚é‚¶‚á‚È‚¢‚©", 0xffffff);
			break;
		case 1:
			DrawString(5150 + offset.x, 640, "‚¨‘O‚³‚ñ‚ğŒ©‰z‚µ‚Ä‚¨Šè‚¢‚ª‚ ‚é", 0xffffff);
			break;
		case 2:
			DrawString(5130 + offset.x, 640, "Œ»İisŒ`‚Å’¬‚ª‰»‚¯•¨‚½‚¿‚ÉP‚í‚ê‚Ä‚¢‚é", 0xffffff);
			break;
		case 3:
			DrawString(5120 + offset.x, 640, "‚ ‚Ì‹óŠÔ‚ğ‰ó‚µ‚Ä‰´‚½‚¿‚ÌŠX‚ğæ‚è–ß‚µ‚Ä‚­‚ê", 0xffffff);
			break;
		case 4:
			DrawString(5265 + offset.x, 640, "—Š‚ñ‚¾‚¼", 0xffffff);
			break;
		}
		break;
	}
}
