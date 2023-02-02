#include "GameMain.h"
#include "DxLib.h"
#include "../Pad.h"
#include "../game.h"
#include "../Object/Player.h"
#include "../Object/BugSpace.h"

GameMain::GameMain()
{
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
		space->enemySetPlayer();
	}
}


void GameMain::init()
{
	//��Ԃ̏ꏊ�������_���Őݒ�
	for (auto& space : space) {
		space->setPos({static_cast<float>(GetRand(Game::kScreenWidth - 50)), 600.0f});
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
	//�n��
	DrawBox(0, 700, Game::kScreenWidth, Game::kScreenHeight, GetColor(255, 255, 0), true);
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
	player->draw();

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
	float playerBottom = player->getPos().y + 64;

	for (ColData data : ladderColData) {
		if (playerRight < data.left)continue;
		DrawString(300, 0, "aiu1", 0xffffff);
		if (playerLeft > data.right)continue;
		DrawString(300, 15, "aiu1", 0xffffff);
		if (playerBottom < data.top)continue;
		DrawString(300, 30, "aiu1", 0xffffff);
		if (playerTop > data.bottom)continue;
		DrawString(300, 45, "aiu1", 0xffffff);
		return true;
	}
	return false;
}
