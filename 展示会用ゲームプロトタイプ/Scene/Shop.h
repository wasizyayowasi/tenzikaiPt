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
	

	int currentInputIndex = 0;//現在選択中のインデックス
	//入力一時テーブル

	bool isEditing = false;//現在特定の入力は編集中です。
	bool selecting = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	Player* player;

	std::map<ProductList, std::string> shopTable;
	std::map<Price, std::string> priceTable;
};

