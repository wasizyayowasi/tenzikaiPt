#pragma once

#include <vector>
#include <map>
#include <string>

enum class InputType
{
	next,			/// ���֍s���{�^��
	prev,			/// �O�ɖ߂�{�^��
	pause,			/// �|�[�Y�{�^��
	keyconf,		/// �L�[�R���t�B�O
	change,			///�ύX
	up,				///��
	down,			///��
	left,			///��
	right,			///�E
	shot,			///����
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

class InputState
{
	friend KeyConfigScene;
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

