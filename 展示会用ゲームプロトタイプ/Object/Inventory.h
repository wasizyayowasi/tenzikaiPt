#pragma once
#include <map>
#include <string>

class InputState;

enum class InventoryChoice
{
	Inventory1,			/// ���֍s���{�^��
	Inventory2,			/// �O�ɖ߂�{�^��
	Inventory3,
};

class Inventory
{
public:
	Inventory();
	~Inventory();

	void update(const InputState& input);
	void draw();

	int setCurrentInputIndex() { return currentInputIndex; }
private:
	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X

	std::map<InventoryChoice, std::string> inventoryChoice;
};

