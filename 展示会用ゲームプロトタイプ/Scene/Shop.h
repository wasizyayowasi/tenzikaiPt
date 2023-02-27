#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;
class Player;

enum class ProductList
{
	heart,	
	block,	
	potion,	
};

enum class Price
{
	heart,
	block,
	potion,
};

class Shop : public SceneBase
{
public:
	Shop(SceneManager& manager, const InputState& input,Player* dPlayer,int pHandle,int gHandle,int hHandle,int rHandle);
	~Shop();

	void update(const InputState& input);
	void draw();
private:
	int timer = 0;
	int moneyPossessed = 0;
	int amount = 0;

	int hpHandle = -1;
	int portionHandle = -1;
	int guiHandle = -1;
	int repairHandle = -1;
	

	int currentInputIndex = 0;//���ݑI�𒆂̃C���f�b�N�X
	//���͈ꎞ�e�[�u��

	bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	bool selecting = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	Player* player;

	std::map<ProductList, std::string> shopTable;
	std::map<Price, std::string> priceTable;
};

