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
	virtual ~Inventory();

	void update(const InputState& input);
	void draw();

	void setHandle(int pHandle, int mHandle,int gHandle) { portionHandle = pHandle, hacheteHandle = mHandle,guiHandle = gHandle; }

	void setNum(int repairNum, int recoveryNum,bool isEnabled) { repairBlock = repairNum, recoveryItem = recoveryNum,hachete = isEnabled; }

	int setCurrentInputIndex() { return currentInputIndex; }
private:
	int guiHandle = -1;
	int hacheteHandle = -1;
	int portionHandle = -1;

	bool hachete = false;
	int repairBlock = 1;
	int recoveryItem = 1;

	int currentInputIndex = 0;//���ݑI�𒆂̃C���f�b�N�X

	std::map<InventoryChoice, std::string> inventoryChoice;
};

