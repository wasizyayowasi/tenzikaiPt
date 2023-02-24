#pragma once
#include "SceneBase.h"
#include <memory>
#include "../Vec2.h"

class Player;
class Field;

class BossBattleScene : public SceneBase
{
public:
	BossBattleScene(SceneManager& manager);
	virtual ~BossBattleScene();

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

	Vec2 offset;

	std::shared_ptr<Field> field;

	Player* player;

	using updateFunc_t = void (BossBattleScene::*)(const InputState& input);
	updateFunc_t updateFunc = nullptr;
};

