#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;
class Player;

enum class PurchaseChoice
{
	kau,			/// ���֍s���{�^��
	yameru,			/// �O�ɖ߂�{�^��
};

class Trade : public SceneBase
{
public:
	Trade(SceneManager& manager, const InputState& input, Player* dPlayer,int amountOfMoney,int num);
	~Trade();

	void update(const InputState& input);
	void draw();

	int setBuy() { return buy; }
private:
	int amount = 0;
	int textNum = 0;
	int textDisplayTime = 0;
	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X

	int itemNum;
	//���͈ꎞ�e�[�u��

	bool buy = false;

	//bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	Player* player;

	std::map<PurchaseChoice, std::string> purchaseChoiceTable;
};

