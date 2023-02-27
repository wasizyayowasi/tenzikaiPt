#pragma once
#include "SceneBase.h"
#include "../Vec2.h"
#include <memory>

class Player;
class Field;

class TutorialScene : public SceneBase
{
public:
	TutorialScene(SceneManager& manager);
	virtual ~TutorialScene();

	void init();

	void update(const InputState& input);
	void draw();

	//�t�F�[�h�C���̎���update�֐�
	void fadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�update�֐�
	void normalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g����update�֐�
	void gameoverFadeOutUpdate(const InputState& input);
	void clearFadeOutUpdate(const InputState& input);
private:

	static constexpr int fadeInterval = 60;
	int fadeTimer = fadeInterval;
	float fadeValue = 255;//����`�̃u�����h�

	int enemyHandle = -1;
	int repairHandle = -1;
	int hpHandle = -1;
	int portionHandle = -1;
	int hacheteHandle = -1;
	int guiHandle = -1;
	int bugHandle = -1;

	Vec2 offset;

	std::shared_ptr<Field> field;

	Player* player;

	using updateFunc_t = void (TutorialScene::*)(const InputState& input);
	updateFunc_t updateFunc = nullptr;
};

