#pragma once
#include "SceneBase.h"
#include <map>
#include <string>

class InputState;
class SceneManager;

enum class ProductList
{
	kaki,			/// ���֍s���{�^��
	susi,			/// �O�ɖ߂�{�^��
	niku,			/// �|�[�Y�{�^��
};

enum class Price
{
	kaki,			/// ���֍s���{�^��
	susi,			/// �O�ɖ߂�{�^��
	niku,			/// �|�[�Y�{�^��
};

class Shop : public SceneBase
{
public:
	Shop(SceneManager& manager, const InputState& input);
	~Shop();

	void update(const InputState& input);
	void draw();
private:
	int timer = 0;

	int currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
	//���͈ꎞ�e�[�u��

	bool isEditing = false;//���ݓ���̓��͕͂ҏW���ł��B
	bool Selecting = false;//���ݓ���̓��͕͂ҏW���ł��B
	const InputState& inputState;

	std::map<ProductList, std::string> shopTable;
	std::map<Price, std::string> priceTable;
};

