#include "Shop.h"
#include "DxLib.h"
#include "../game.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "Trade.h"
#include "../Object/Player.h"

namespace {
	int graphSize = 16;;
}

Shop::Shop(SceneManager& manager, const InputState& input, Player* dPlayer, int pHandle, int gHandle,int hHandle, int rHandle,int cHandle) : SceneBase(manager), inputState(input), player(dPlayer),portionHandle(pHandle),guiHandle(gHandle), hpHandle(hHandle),repairHandle(rHandle), coinHandle(cHandle)
{

	LPCSTR fontPath = "data/other/Silver.ttf";
	AddFontResourceEx(fontPath, FR_PRIVATE, NULL);

	fontHandle = CreateFontToHandle("Silver", 64, 9, -1);
	fontHandle2 = CreateFontToHandle("Silver", 48, 9, -1);
	uiSound = LoadSoundMem("data/soundEffect/ui3.mp3");
	bottanHandle = LoadGraph("data/GUIGraph/bottan.png");

	shopTable[ProductList::heart] = "�n�[�g";
	shopTable[ProductList::block] = "�C���A�C�e��";
	shopTable[ProductList::potion] = "�񕜃A�C�e��";

	priceTable[Price::heart] = "500�~";
	priceTable[Price::block] = "500�~";
	priceTable[Price::potion] = "1500�~";
}

Shop::~Shop()
{
	DeleteSoundMem(uiSound);
	DeleteFontToHandle(fontHandle);
	DeleteFontToHandle(fontHandle2);
	DeleteGraph(bottanHandle);
}

void Shop::update(const InputState& input)
{
	const int nameCount = static_cast <int>(shopTable.size() + 1);

	if (input.isTriggered(InputType::up)) {
		ChangeVolumeSoundMem(160, uiSound);
		PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::down)) {
		ChangeVolumeSoundMem(160, uiSound);
		PlaySoundMem(uiSound, DX_PLAYTYPE_BACK);
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}

	//���̎������A�m�肵�܂���I�����Ă���
	if (currentInputIndex == shopTable.size()) {
		if (input.isTriggered(InputType::next)) {
			manager_.popScene();
			return;
		}
	}

	if (input.isTriggered(InputType::next)) {
		isEditing = !isEditing;
		selecting = true;
		return;
	}

	if (isEditing) {
		switch (currentInputIndex) {
		case 0:
			amount = 500;
			break;
		case 1:
			amount = 500;
			break;
		case 2:
			amount = 1500;
			break;
		}
		if (!(currentInputIndex == 0 && player->returnHp() > 9)) {
			manager_.pushScene(new Trade(manager_, input, player, amount, currentInputIndex));
		}
		else {
			int aiu = 0;
		}
		isEditing = false;
	}

	if (input.isTriggered(InputType::prev)) {
		manager_.popScene();
		return;
	}

	if (--time == 0) {
		moveImgX++;
		time = 8;
	}

	if (moveImgX > 6) {
		moveImgX = 3;
	}
}

void Shop::draw()
{

	constexpr int shopkeeperWidth = 480;
	constexpr int shopkeeperHeight = 170;
	constexpr int shopkeeperX = (Game::kScreenWidth - shopkeeperWidth);
	constexpr int shopkeeperY = shopkeeperHeight * 2;

	constexpr int goodsWidth = 480;
	constexpr int goodsHeight = shopkeeperHeight * 2 + 100;
	constexpr int goodsX = Game::kScreenWidth / 2 - 300;
	constexpr int goodsY = Game::kScreenHeight / 2 + 250;

	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);//��Z����
	//�|�[�Y�E�B���h�E�Z���t�@��(����)
	//�X��Z���t����
	DrawBox(shopkeeperWidth, shopkeeperHeight, shopkeeperX, shopkeeperY, 0x000000, true);
	//���i
	DrawBox(goodsWidth, goodsHeight, goodsX + 50, goodsY, 0x000000, true);
	//���i�Љ�E�B���h�E
	DrawBox(goodsWidth + 300, goodsHeight, shopkeeperX, goodsY, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//�ʏ�`��ɖ߂�

	int coinWidth = Game::kScreenWidth - 64 * 2;
	DrawRotaGraph(coinWidth, 96, 3.0f, 0.0f, coinHandle, true);
	std::string tempMoney;
	tempMoney = std::to_string(player->setMoneyPossessed());
	int tempWidth = GetDrawStringWidthToHandle(tempMoney.c_str(), strlen(tempMoney.c_str()), fontHandle);
	coinWidth = (((coinWidth + 96) - coinWidth) / 2 - tempWidth / 2) + coinWidth - 48;
	DrawFormatStringToHandle(coinWidth, 150, 0xffffff, fontHandle, "%d", player->setMoneyPossessed());

	

	//���i�̕`��
	auto y = 520;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : shopTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		//�I�����ꂽ���̏���
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			if (selecting) {
				color = 0xffff00;
			}
			DrawString(shopkeeperWidth + 10, y, "��", 0xff0000);
			DrawString(shopkeeperWidth + 20 + offset + 320, shopkeeperWidth, name.second.c_str(), 0xffffff);
			DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 5.0f, 0.0f, guiHandle, true, false);
			switch (currentInputIndex) {
			case 0:
				DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 5.0f, 0.0f, hpHandle, true, false);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 80, "�ő�̗͂�1���������邱�Ƃ��ł���B", 0xffffff);
				break;
			case 1:
				DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 4.0f, 0.0f, repairHandle, true, false);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 80, "�ǂ�ǂ�Ƃ��������B", 0xffffff);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 100, "�Q�[�g����邱�Ƃ��ł�������", 0xffffff);
				break;
			case 2:
				DrawRotaGraph(shopkeeperWidth + 20 + offset + 420, shopkeeperWidth + 150, 5.0f, 0.0f, portionHandle, true, false);
				DrawString(shopkeeperWidth + 20 + offset + 570, shopkeeperWidth + 80, "���g�̗̑͂�5�񕜂��邱�Ƃ��ł���B", 0xffffff);
				break;
			}
			
		}

		//�e�L�[�̕\��
		int x = shopkeeperWidth + 20 + offset + 10;
		DrawString(x - 15, y, "�E", color);
		DrawString(x, y, name.second.c_str(), color);

		y += 80;
		++idx;
	}

	//�l�i�̕`��
	y = 520;
	idx = 0;
	for (const auto& price : priceTable) {
		unsigned int color = 0xffffff;

		//�I�����ꂽ���̏���
		if (currentInputIndex == idx) {
			isInputtypeSelected = true;
			DrawString(shopkeeperWidth + 10, y, "��", 0xff0000);
		}

		//�e�L�[�̕\��
		int x = shopkeeperWidth + 20;
		DrawString(x + 150, y, price.second.c_str(), color);

		y += 80;
		++idx;
	}

	if (player->returnHp() > 9) {
		DrawBox(shopkeeperWidth + 10, 525, shopkeeperWidth + 220, 530, 0xff0000, true);
	}

	y += 20;
	if (!isInputtypeSelected) {
		int yOffset = 0;
		if (currentInputIndex == shopTable.size() + 1) {
			yOffset = 20;
		}
		DrawString(shopkeeperWidth + 150, y - 20, "��", 0xff0000);
	}

	DrawString(shopkeeperWidth + 180, y - 20, "�ޓX ", 0xffffff);

	DrawString(shopkeeperWidth + 10, shopkeeperHeight + 10, "�X��", 0xffffff);
	DrawString(shopkeeperWidth + 200, shopkeeperHeight + 80, "���߂������ɂ���Ă����̂Ȃ񂩉���ˁ`�񂾂�B�������s���B�V�b�V�b", 0xffffff);

	if (--timer < 0) {
		timer = 180;
	}

	if (timer > 0) {
		if ((timer / 30) % 2 == 1) {
			DrawString(shopkeeperX - 140, shopkeeperY - 30, "  shoping...", 0xffff88);
		}
	}

	//�|�[�Y�E�B���h�E�g��
	DrawBox(shopkeeperWidth, shopkeeperHeight, shopkeeperX, shopkeeperY, 0xffffff, false);
	DrawBox(goodsWidth, goodsHeight, goodsX + 50, goodsY, 0xffffff, false);
	DrawBox(goodsWidth + 300, goodsHeight, shopkeeperX, goodsY, 0xffffff, false);

	DrawStringToHandle(Game::kScreenWidth - 200, Game::kScreenHeight - 55, "����", 0xffffff, fontHandle2);
	DrawStringToHandle(Game::kScreenWidth - 70, Game::kScreenHeight - 55, "�߂�", 0xffffff, fontHandle2);


	for (const auto& name : inputState.inputNameTable) {
		int bottanSetWidth = (Game::kScreenWidth / 2 + 200);
		auto type = name.first;
		auto it = inputState.tempMapTable.find(type);

		for (const auto elem : it->second) {
			if (type == InputType::next && elem.cat == InputCategory::pad) {
				nextId = elem.id;
			}
			if (type == InputType::prev && elem.cat == InputCategory::pad) {
				prevId = elem.id;
			}
		}
	}

	bottanNum(nextId);
	moveImgY = imgY;
	DrawRectRotaGraph(Game::kScreenWidth - 230, Game::kScreenHeight - 50, moveImgX * graphSize, moveImgY * graphSize, graphSize, graphSize, 3.0f, 0.0f, bottanHandle, true, false);

	bottanNum(prevId);
	moveImgY = imgY;
	DrawRectRotaGraph(Game::kScreenWidth - 100, Game::kScreenHeight - 50, moveImgX * graphSize, moveImgY * graphSize, graphSize, graphSize, 3.0f, 0.0f, bottanHandle, true, false);
}

void Shop::bottanNum(int num)
{
	switch (num) {
		//X
	case 64:
		imgX = 1;
		imgY = 2;
		break;
		//A
	case 16:
		imgX = 1;
		imgY = 3;
		break;
		//Y
	case 128:
		imgX = 1;
		imgY = 4;
		break;
		//B
	case 32:
		imgX = 1;
		imgY = 5;
		break;
		//��
	case 8:
		imgX = 8;
		imgY = 5;
		break;
		//��
	case 1:
		imgX = 9;
		imgY = 5;
		break;
		//��
	case 2:
		imgX = 8;
		imgY = 6;
		break;
		//�E	
	case 4:
		imgX = 9;
		imgY = 6;
		break;
		//LT
	case 256:
		imgX = 21;
		imgY = 5;
		break;
		//RT
	case 512:
		imgX = 21;
		imgY = 6;
		break;
		//BACK
	case 1024:
		imgX = 10;
		imgY = 13;
		break;
		//start
	case 2048:
		imgX = 10;
		imgY = 14;
		break;

	}
}