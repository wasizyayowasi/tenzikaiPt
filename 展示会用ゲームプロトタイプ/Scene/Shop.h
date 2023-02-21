#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;
class Player;

enum class ProductList
{
	kaki,			/// 次へ行くボタン
	susi,			/// 前に戻るボタン
	niku,			/// ポーズボタン
};

enum class Price
{
	kaki,			/// 次へ行くボタン
	susi,			/// 前に戻るボタン
	niku,			/// ポーズボタン
};

class Shop : public SceneBase
{
public:
	Shop(SceneManager& manager, const InputState& input,Player* dPlayer);
	~Shop();

	void update(const InputState& input);
	void draw();
private:
	int timer = 0;
	int moneyPossessed;
	int amount = 0;

	int currentInputIndex;//現在選択中のインデックス
	//入力一時テーブル

	bool isEditing = false;//現在特定の入力は編集中です。
	bool Selecting = false;//現在特定の入力は編集中です。
	const InputState& inputState;

	Player* player;

	std::map<ProductList, std::string> shopTable;
	std::map<Price, std::string> priceTable;
};

