#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;

enum class PurchaseChoice
{
	kau,			/// ���֍s���{�^��
	yameru,			/// �O�ɖ߂�{�^��
};

class Trade : public SceneBase
{
public:
	Trade(SceneManager& manager, const InputState& input);
	~Trade();

	void update(const InputState& input);
	void draw();
private:
	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
	//���͈ꎞ�e�[�u��

	//bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	std::map<PurchaseChoice, std::string> purchaseChoiceTable;
};

