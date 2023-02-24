#include "InputState.h"
#include "DxLib.h"

InputState::InputState()
{
	//����
	defaultMapTable[InputType::next] = { {InputCategory::keybd,KEY_INPUT_RETURN},
										{InputCategory::pad,PAD_INPUT_R},
										{InputCategory::mouse,MOUSE_INPUT_LEFT} };//�X�^�[�g�{�^��
	//�O��
	defaultMapTable[InputType::nextItem] = { {InputCategory::keybd,KEY_INPUT_C },
										{InputCategory::pad,PAD_INPUT_6} };
	//�ύX
	defaultMapTable[InputType::prevItem] = { {InputCategory::keybd,KEY_INPUT_M},
										  {InputCategory::pad,PAD_INPUT_5} };//�E�V�����_�[
	//�|�[�Y
	defaultMapTable[InputType::pause] = { {InputCategory::keybd,KEY_INPUT_P},
										 {InputCategory::pad,PAD_INPUT_7} };//�Z���N�g�{�^��
	//�L�[�R���t�B�O
	defaultMapTable[InputType::attack] = { {InputCategory::keybd,KEY_INPUT_K},
										   {InputCategory::pad,PAD_INPUT_Y} };//���V�����_�[
	//��
	defaultMapTable[InputType::up] = { {InputCategory::keybd,KEY_INPUT_UP},
									  {InputCategory::pad,PAD_INPUT_UP} };//��
	//��
	defaultMapTable[InputType::down] = { {InputCategory::keybd,KEY_INPUT_DOWN},
										{InputCategory::pad,PAD_INPUT_DOWN} };//��
	//��
	defaultMapTable[InputType::left] = { {InputCategory::keybd,KEY_INPUT_LEFT},
										{InputCategory::pad,PAD_INPUT_LEFT} };//��
	//�E
	defaultMapTable[InputType::right] = { {InputCategory::keybd,KEY_INPUT_RIGHT},
									  {InputCategory::pad,PAD_INPUT_RIGHT} };//�E

	//�e
	defaultMapTable[InputType::shot] = { {InputCategory::keybd,KEY_INPUT_C},
										  {InputCategory::pad,PAD_INPUT_C} };//�e


	inputMapTable = defaultMapTable;

	loadKeyInfo();

	//�ꎞ�}�b�v�e�[�u���ɃR�s�[
	tempMapTable = inputMapTable;

	//���̓^�C�v�̖��O�̃e�[�u�������
	inputNameTable[InputType::next] = "����";
	inputNameTable[InputType::nextItem] = "���̃A�C�e��";
	inputNameTable[InputType::prevItem] = "�O�̃A�C�e��";
	inputNameTable[InputType::pause] = "�|�[�Y";
	inputNameTable[InputType::attack] = "�U��";
	inputNameTable[InputType::shot] = "�\��";


	currentInput.resize(static_cast<int>(InputType::max));
	lastInput.resize(static_cast<int>(InputType::max));
}

bool InputState::isTriggered(InputType type) const
{
	return isPressed(type) && !lastInput[static_cast<int>(type)];
}

bool InputState::isPressed(InputType type) const
{
	return currentInput[static_cast<int>(type)];
}

void InputState::update()
{
	lastInput = currentInput;//���O�̓��͏����L�����Ă���

	char keystate[256];
	GetHitKeyStateAll(keystate);//�S�L�[���擾

	int padState = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�R���̏����擾����
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable) {			//�}�b�v�̑S�������[�v����
		for (const auto& input : keymap.second) {		//���͏��z������[�v����
			//���̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă�
			//keymap.second�ɂ́A���̓��͏��Z�b�gInputInfo�������Ă���B
			//keymap.first�ɂ́A�Ή�����Q�[�����͖���"InputType"�Ȃǂ������Ă���
			if (input.cat == InputCategory::keybd) {
				currentInput[static_cast<int>(keymap.first)] = keystate[input.id];
			}
			else if (input.cat == InputCategory::pad) {
				currentInput[static_cast<int>(keymap.first)] = padState & input.id;
			}
			else if (input.cat == InputCategory::mouse) {
				currentInput[static_cast<int>(keymap.first)] = mouseState & input.id;
			}
			//3�̓��͂̂����ǂꂩ��true������������u���͂���Ă�v
			//�Ƃ݂Ȃ��āAbreak����B
			if (currentInput[static_cast<int>(keymap.first)]) {
				break;
			}
		}
	}
}

void InputState::rewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//���͎��(���蓖�Đ�)���Ȃ���΁A�����Ȃ̂Ŗ������܂��B
	if (tempMapTable.count(type) == 0) {
		return;
	}

	bool isRewrite = false;

	for (auto& InputInfo : tempMapTable[type]) {
		if (InputInfo.cat == cat)//�J�e�S�����q�b�g������
		{
			InputInfo.id = id;//ID���㏑������
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite) {//�����J�e�S�������݂��Ȃ�������A�����Œǉ����Ă���
		tempMapTable[type].push_back({ cat,id });
	}
}

void InputState::commitChangedInputInfo()
{
	inputMapTable = tempMapTable;
}

void InputState::rollbackChangedInputInfo()
{
	tempMapTable = inputMapTable;
}

void InputState::resetInputInfo()
{
	inputMapTable = defaultMapTable;
	tempMapTable = defaultMapTable;
}

void InputState::savekeyInfo() const
{
	FILE* fp = nullptr;

	auto err = fopen_s(&fp, "key.info", "wb");
	if (fp == nullptr) {
		return;
	}

	//���z�L�[�^�C�v�̐�����������
	int keyTypeNum = inputMapTable.size();
	fwrite(&keyTypeNum, sizeof(keyTypeNum), 1, fp);
	//���z�L�[�^�C�v(next�Aprev�Ȃ�)�̃��[�v
	for (const auto& key : inputMapTable) {
		int keytype = static_cast<int>(key.first);
		//���z�L�[�ԍ�
		fwrite(&keytype, sizeof(keytype), 1, fp);
		int dataSize = key.second.size();
		//�������̓f�[�^������̂�
		fwrite(&dataSize, sizeof(dataSize), 1, fp);

		//Vector�^��data()�͐擪�̃A�h���X��Ԃ�
		//��C�ɂ��̓��̓f�[�^����������
		fwrite(key.second.data(), dataSize * sizeof(InputInfo), 1, fp);
	}

	fclose(fp);
}

void InputState::loadKeyInfo()
{
	int handle = FileRead_open("key.info");
	if (handle == 0) {
		return;
	}
	int keyTypeNum = 0;
	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);
	inputMapTable.clear();
	for (int i = 0; i < keyTypeNum; i++) {
		int  inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo>inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo) * dataSize, handle);
		inputMapTable[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable = inputMapTable;
	FileRead_close(handle);
}
