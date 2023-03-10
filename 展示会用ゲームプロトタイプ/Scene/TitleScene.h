#pragma once
#include "SceneBase.h"
#include "../Vec2.h"
#include <memory>
#include <string>
#include <map>
#include <array>

class Enemy;
class Field;

enum class choice
{
	tutorial,		
	main,			
	boss,
	end,			
};

class TitleScene : public SceneBase
{
public:

	TitleScene(SceneManager& manager);
	~TitleScene();

	void update(const InputState& input);
	void draw();

	void choiceScene(const InputState& input);
	void choiceSceneDraw();

private:
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;//����`�̃u�����h�

	unsigned int FadeColor = 0x000000;//�t�F�[�h�F��

	int currentInputIndex = 0;//���ݑI�𒆂̃C���f�b�N�X

	//�t�F�[�h�C���̎���update�֐�
	void fadeInUpdate(const InputState& input);
	//�ʏ��Ԃ�update�֐�
	void normalUpdate(const InputState& input);
	//�t�F�[�h�A�E�g����update�֐�
	void fadeOutUpdateGameMain(const InputState& input);
	void fadeOutUpdateTutorial(const InputState& input);
	void fadeOutUpdateBoss(const InputState& input);

	int soundHandle = -1;
	int soundVolume = 150;
	int fontHandle = -1;
	int fontHandle2 = -1;
	int UIfontHandle = -1;
	int UIfontHandle2 = -1;
	int bottanHandle = -1;

	int imgX = 2;
	int imgY = 1;
	int time = 8;

	int titleWidth = 0;
	int titleWidth2 = 0;

	int enemyHandle = -1;
	int collTime = 300;
	int luck = 0;
	int fontSize = 0;
	bool enemyGo = false;
	bool enemiesGo = false;
	Vec2 shift = {120.0f,800.0f};

	std::map<choice, std::string> sceneTable;

	std::shared_ptr<Enemy> enemy;
	std::shared_ptr<Field> field;

	std::array<std::shared_ptr<Enemy>, 16> enemies;

	//update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};

