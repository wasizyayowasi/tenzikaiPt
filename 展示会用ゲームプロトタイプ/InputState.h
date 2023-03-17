#pragma once

#include <vector>
#include <map>
#include <string>

enum class InputType
{
	next,			/// ���֍s���{�^��
	attack,		/// �L�[�R���t�B�O
	shot,			///����
	prev,
	nextItem,		/// �O�ɖ߂�{�^��
	prevItem,		///�ύX
	pause,			/// �|�[�Y�{�^��
	up,				///��
	down,			///��
	left,			///��
	right,			///�E
	max,			///�ő���̓C���f�b�N�X
};

enum class InputCategory {
	keybd,		//�L�[�{�[�h
	pad,		//�Q�[���p�b�h
	mouse,		//�}�E�X
};

struct InputInfo {
	InputCategory cat;
	int id;				
};

class KeyConfigScene;
class Pause;
class Shop;
class GameMain;
class BossBattleScene;
class TutorialScene;

class InputState
{
	friend KeyConfigScene;
	friend Pause;
	friend Shop;
public:
	InputState();

	bool isTriggered(InputType type) const;

	
	bool isPressed(InputType type) const;

	
	void update();

	
	void rewriteInputInfo(InputType type, InputCategory cat, int id);

	
	void commitChangedInputInfo();

	
	void rollbackChangedInputInfo();

	
	void resetInputInfo();

	
	void savekeyInfo()const;

	void loadKeyInfo();
private:
	
	using InputMap_t = std::map<InputType, std::vector<InputInfo> >;
	InputMap_t inputMapTable;

	InputMap_t tempMapTable;

	InputMap_t defaultMapTable;

	std::map<InputType, std::string> inputNameTable;

	std::vector<bool> currentInput;	
	std::vector<bool> lastInput;	
};

