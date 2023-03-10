#include "Trade.h"
#include "SceneManager.h"
#include "../InputState.h"
#include "DxLib.h"
#include "../game.h"
#include "Shop.h"
#include "../Object/Player.h"

constexpr int defaultTime = 120;

Trade::Trade(SceneManager& manager, const InputState& input, Player* dPlayer, int amountOfMoney, int num) : SceneBase(manager),inputState(input), player(dPlayer),amount(amountOfMoney),itemNum(num)
{
	purchaseChoiceTable[PurchaseChoice::kau] = "����";
	purchaseChoiceTable[PurchaseChoice::yameru] = "��߂Ƃ�";
}

Trade::~Trade()
{
}

void Trade::update(const InputState& input)
{
	const int nameCount = purchaseChoiceTable.size();

	if (input.isTriggered(InputType::left)) {
		currentInputIndex = ((currentInputIndex - 1) + nameCount) % nameCount;
	}
	else if (input.isTriggered(InputType::right)) {
		currentInputIndex = (currentInputIndex + 1) % nameCount;
	}
	

	if (input.isTriggered(InputType::next)) {
		if (!(itemNum == 0 && player->returnHp() > 9)) {
			if (currentInputIndex == 0) {
				if (player->setMoneyPossessed() - amount >= 0) {
					player->setItemControl(itemNum);
					player->setMoney(amount);
					manager_.popScene();
					return;
				}
			}
			else {
				manager_.popScene();
				return;
			}
		}
	}

	if (input.isTriggered(InputType::prev)) {
		manager_.popScene();
		return;
	}
}

void Trade::draw()
{

	constexpr int pw_width = Game::kScreenWidth / 2 - 200;						//�|�[�Y�g�̕�
	constexpr int pw_height = Game::kScreenHeight / 2 - 200;						//�|�[�Y�g�̍���
	constexpr int pw_start_x = Game::kScreenWidth / 2 + 200;	//�|�[�Y�g�ɍ�
	constexpr int pw_start_y = Game::kScreenHeight / 2 + 200;	//�|�[�Y�g��

	auto y = Game::kScreenHeight / 2 + 200;
	int x = pw_width + 410;
	int idx = 0;
	bool isInputtypeSelected = false;
	for (const auto& name : purchaseChoiceTable) {
		int offset = 0;
		unsigned int color = 0xffffff;

		//�I�����ꂽ���̏���
		if (currentInputIndex == idx) {
			offset = 10;
			isInputtypeSelected = true;
			color = 0xffff00;
			DrawString(x - 20, y, "��", 0xff0000);
		}

		//�e�L�[�̕\��
		if (currentInputIndex == idx) {
			x += offset;
		}
		DrawString(x, y, name.second.c_str(), color);

		if (currentInputIndex == idx) {
			x -= offset;
		}

		x += 150;
		++idx;
	}

	y += 20;
	if (!isInputtypeSelected) {
		int yOffset = 0;
		if (currentInputIndex == purchaseChoiceTable.size() + 1) {
			yOffset = 20;
		}
		DrawString(pw_width + 90, y + yOffset, "��", 0xff0000);
	}
}
