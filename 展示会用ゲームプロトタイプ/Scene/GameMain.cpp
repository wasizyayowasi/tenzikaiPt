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
	//�v���C���[�̍X�V
	if (player->isEnable()) {
		player->update();
		//���͑��u�̏���ǂݎ��
		Pad::update();
	}
	else {
		player->updateDeath();
	}

	/*if (Pad::isPress(PAD_INPUT_LEFT)) {
		player->playerMotionUpdate(1,true);
		scrollX += 10.0f;
	}
	if (Pad::isPress(PAD_INPUT_RIGHT)) {
		player->playerMotionUpdate(1,false);
		scrollX -= 10.0f;
	}*/

	if (player->isEnable()) {
		//��Ԃ̍X�V
		for (auto& space : space) {
			if (space->isEnable()) {
				space->update();
			}
		}
	}
}

void GameMain::draw()
{
	int offsetX = scrollX;
	if (offsetX > 1) { 
		offsetX = 0; 
		player->playerMove(true);
	}
	if (offsetX < -Game::kScreenWidth * 2) { 
		offsetX = -Game::kScreenWidth * 2;
		player->playerMove(true);
	}

	drawMap(offsetX);

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {
			DrawBox(x * Field::chipSize, y * Field::chipSize, x * Field::chipSize + Field::chipSize, y * Field::chipSize + Field::chipSize, 0x224422, false);
		}
	}

	//�n��
	DrawString(fieldX, fieldY, "�n��", 0x000000);
	DrawString(100, ladderBlockY, "�n��", 0x000000);

	//�B��ꏊ
	DrawString(hiddenBlockX, hiddenBlockY, "�B��ꏊ", 0x000000);

	//��q
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

void GameMain::drawMap(int offsetX)
{
	dataX = offsetX / 32;

	for (int x = 0; x < Field::bgNumX; x++) {
		for (int y = 0; y < Field::bgNumY; y++) {

			const int chipNo = Field::field[y][x];

			if (chipNo == 1) {
				DrawBox(x * Field::chipSize, y * Field::chipSize, x * Field::chipSize + Field::chipSize, y * Field::chipSize + Field::chipSize, 0xffff00, true);
			}
			else if (chipNo == 2) {
				DrawBox(x * Field::chipSize, y * Field::chipSize, x * Field::chipSize + Field::chipSize, y * Field::chipSize + Field::chipSize, 0x444444, true);
			}
			else if (chipNo == 3) {
				DrawBox(x * Field::chipSize, y * Field::chipSize, x * Field::chipSize + Field::chipSize, y * Field::chipSize + Field::chipSize, 0x44ff44, true);
			}
		}
	}
}
