#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"
#include"../field.h"



GameMain::GameMain()
{
	enemyHandle = LoadGraph("data/enemy.png");
	playerHandle = LoadGraph("data/player.png");

	player = new Player;
	//��Ԃ̃f�[�^�����
	for (int i = 0; i < 3;i++) {
		space[i] = std::make_shared<BugSpace>(i);
	}
	//�v���C���[�̃f�[�^�𑗂�
	for (auto& space : space) {
		space->setPlayer(player);
	}
	for (auto& space : space) {
		space->enemySetPlayer(enemyHandle);
	}
}

GameMain::~GameMain()
{
	delete player;
	DeleteGraph(enemyHandle);
}


void GameMain::init()
{
	//��Ԃ̏ꏊ�������_���Őݒ�
	for (auto& space : space) {
		//space->setPos({static_cast<float>(GetRand(Game::kScreenWidth - 50)), 600.0f});
		space->init();
	}
}

void GameMain::update()
{
	//���͑��u�̏���ǂݎ��
	Pad::update();

	//�v���C���[�̍X�V
	player->update();

	//��Ԃ̍X�V
	for (auto& space : space) {
		if (space->isEnable()) {
			space->update();
		}
	}

	//�g���B�����Ԃɂ��邩�̐��ۂ�Ԃ�
	if (hiddenPlayer()) {
		player->setHidden(true);
	}
	else {
		player->setHidden(false);
	}

	//��q�ɓo����ԂȂ̂����ۂ�Ԃ�
	if (ladderCollision()) {
		player->setLadder(true);
	}
	else {
		player->setLadder(false);
	}
}

void GameMain::draw()
{
	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 1) {
				DrawBox(x * Field::chipSize, y * Field::chipSize, x * Field::chipSize + Field::chipSize, y * Field::chipSize + Field::chipSize, 0xffff00, true);
			}
		}
	}

	//�n��
	//DrawBox(0, 700, Game::kScreenWidth, Game::kScreenHeight, GetColor(255, 255, 0), true);
	DrawString(0, 700, "�n��", 0x000000);

	DrawBox(100, ladderBlockY, ladderBlockX, ladderBlockY + 50, GetColor(255, 255, 0), true);
	DrawString(100, ladderBlockY, "�n��", 0x000000);

	//�B��ꏊ
	DrawBox(hiddenBlockX, hiddenBlockY, hiddenBlockX + 100, hiddenBlockY + 100, GetColor(100, 255, 0), true);
	DrawString(hiddenBlockX, hiddenBlockY, "�B��ꏊ", 0x000000);

	//��q
	DrawBox(ladderBlockX, ladderBlockY, ladderBlockX + 70, ladderBlockY + 500, GetColor(100, 100, 100), true);
	DrawString(ladderBlockX, ladderBlockY, "��q", 0xffffff);

	//��Ԃ̕`��
	for (auto& space : space) {
		if (space->isEnable()) {
			space->draw();
		}
	}

	//�v���C���[�̕`��
	player->draw(playerHandle);

}

struct ColData {
	int left;
	int top;
	int right;
	int bottom;
};

//�B��ꏊ�̃f�[�^
ColData colData[] = {
	{600,600,700,700}
};

//��q�̃f�[�^
ColData ladderColData[] = {
	{300,200,370,700}
};

//�B��ꏊ�ƃv���C���[�̓����蔻��
bool GameMain::hiddenPlayer() {

	float playerLeft = player->getPos().x;
	float playerRight = player->getPos().x + 50;
	float playerTop = player->getPos().y;
	float playerBottom = player->getPos().y + 64;

	for (ColData data : colData) {
		if (playerRight < data.left)continue;
		if (playerLeft > data.right)continue;
		if (playerBottom < data.top)continue;
		if (playerTop > data.bottom)continue;
		return true;
	}
	return false;
}

//��q�ƃv���C���[�̓����蔻��
bool GameMain::ladderCollision()
{
	float playerLeft = player->getPos().x + 25;
	float playerRight = player->getPos().x + 26;
	float playerTop = player->getPos().y;
	float playerBottom = player->getPos().y + 74;

	for (ColData data : ladderColData) {
		if (playerRight < data.left)continue;
		if (playerLeft > data.right)continue;
		if (playerBottom < data.top)continue;
		if (playerTop > data.bottom)continue;

		return true;
	}
	return false;
}
