#include "Player.h"
#include "PlayerThrowinAttack.h"

#include "PlayerMotion.h"
#include "ObjectHp.h"
#include "Inventory.h"
#include "../field.h"

#include "../DrawFunctions.h"
#include "../InputState.h"
#include "DxLib.h"
#include <algorithm>



namespace {
	constexpr float correctionSizeX = 14.0f;
	constexpr float correctionSizeY = 74.0f;
	constexpr int ultimate_frames = 120;
}

Player::Player(int num) : sceneNum(num)
{
	playerHandle = my::myLoadGraph("data/objectGraph/player.png");
	smokeHandle = my::myLoadGraph("data/effect/spritesheet.png");
	attackSound = LoadSoundMem("data/soundEffect/attack_knight.wav");
	coinSound = LoadSoundMem("data/soundEffect/Single-Coin-Drop-On-Table-www.fesliyanstudios.com.mp3");
	walkSound = LoadSoundMem("data/soundEffect/Concrete 1.wav");

	hp = new ObjectHp;
	motion = new PlayerMotion;
	inventory = new Inventory();
	flyingObject = std::make_shared<PlayerThrowinAttack>(sceneNum);
	hp->setObjectMaxHp(playerHp);
	updateFunc = &Player::updateDescent;
}

Player::~Player()
{
	delete hp;
	delete motion;
	delete inventory;

	DeleteGraph(playerHandle);
	DeleteGraph(smokeHandle);

	DeleteSoundMem(attackSound);
	DeleteSoundMem(coinSound);
	DeleteSoundMem(walkSound);
}

void Player::init()
{
	inventory->setHandle(portionHandle, macheteHandle,guiHandle,repairHandle);
}

void Player::update(Vec2 offset, const InputState& input)
{
	
	inventory->update(input);
	hp->setObjectHp(playerHp);

	if (flyingObject->isEnable()) {
		if (sceneNum != 2) {
			flyingObject->update(offset);
		}
		else {
			flyingObject->bossUpdate(offset);
		}
		
	}

	ultimateTimer_ = (std::max)(ultimateTimer_ - 1, 0);

	(this->*updateFunc)(offset,input);

	if (playerPos.x < 0) {
		playerPos.x = 0;
	}
	if (sceneNum == 0) {
		if (playerPos.x + 50 > Game::kScreenWidth * 2) {
			playerPos.x = Game::kScreenWidth * 2 - 50;
		}
	}
	else if(sceneNum == 1) {
		if (playerPos.x + 50 > Game::kScreenWidth * 3) {
			playerPos.x = Game::kScreenWidth * 3 - 50;
		}
	}
	else {
		if (playerPos.x + 50 > Game::kScreenWidth * 8) {
			playerPos.x = Game::kScreenWidth * 8 - 50;
		}
	}
	

	if (playerHp < 1) {
		motionNum = 4;
		isEnabled = false;
		updateFunc = &Player::updateDeath;
	}
}

void Player::tutorialUpdate(Vec2 offset, const InputState& input)
{
	//モーション関係
	motionNum = 0;
	motion->update(motionNum);

	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//配列の中身を見る
	int chipNo = buildingData::tutorialBuilding[underfootChipNoY][underfootChipNoX];

	{
		//梯子を降りる
		if (chipNo == 119 || chipNo == 121) {
			if (input.isPressed(InputType::down)) {
				playerPos.y += 3.0f;
				if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
					updateFunc = &Player::updateLadder;
				}
			}
		}

		//梯子を上る
		for (int x = -1; x < 1; x++) {
			int chipNo = buildingData::tutorialBuilding[underfootChipNoY - 1][underfootChipNoX - x];
			if (chipNo == 119 || chipNo == 121) {
				if (input.isPressed(InputType::up)) {
					if (ladderCollision(underfootChipNoX - x, underfootChipNoY - 1)) {
						updateFunc = &Player::updateLadder;
						return;
					}
				}
			}
		}
	}

	//降下
	int DescentChipNo3 = groundData::tutorialGround[underfootChipNoY][underfootChipNoX];
	int DescentChipNo4 = buildingData::tutorialBuilding[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0 && DescentChipNo4 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//移動
	if (!push) {

		if (input.isPressed(InputType::left)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			if (--soundCount == 0) {
				ChangeVolumeSoundMem(130, walkSound);
				PlaySoundMem(walkSound, DX_PLAYTYPE_BACK, true);
				soundCount = 20;
			}
			motion->update(motionNum);
			playerPos.x -= 5;
			playerDirections = true;
		}
		else if (input.isPressed(InputType::right)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			if (--soundCount == 0) {
				ChangeVolumeSoundMem(130, walkSound);
				PlaySoundMem(walkSound, DX_PLAYTYPE_BACK, true);
				soundCount = 20;
			}
			motion->update(motionNum);
			playerPos.x += 5;
			playerDirections = false;
		}
	}


	//近接攻撃
	if (!push) {
		if (!flyingObject->isEnable()) {
			if (input.isPressed(InputType::attack)) {
				ChangeVolumeSoundMem(soundVolume, attackSound);
				PlaySoundMem(attackSound, DX_PLAYTYPE_BACK, true);
				motionNum = 3;
				motion->update(motionNum);
				proximityAttack = true;
			}
			else {
				proximityAttack = false;
			}
		}
	}

	//アイテムの切り替え
	switch (inventory->setCurrentInputIndex()) {
	case 0:
		//投擲
		if (!push) {
			if (input.isTriggered(InputType::shot)) {
				if (!flyingObject->isEnable()) {
					flyingObject->attack(playerPos, playerDirections);
				}
			}
		}
		break;
	case 1:
		//修復タイマー
		if (repairBlock > 0) {
			if (input.isPressed(InputType::shot)) {
				spaceHpDisplay = true;
			}
			else {
				spaceHpDisplay = false;
				motionStart = false;
			}
		}
		else {
			spaceHpDisplay = false;
			motionStart = false;
		}
		break;
	case 2:
		if (recoveryItem > 0) {
			if (input.isTriggered(InputType::shot)) {
				if (playerHp < 10) {
					playerHp = (std::min)({ playerHp + 5, hp->returnMaxHp() });
					recoveryItem--;
				}
			}
		}
		break;
	}

	if (flyingObject->landing()) {
		if (flyingObject->playerCollision(playerPos, offset)) {
			flyingObject->deadFlyingObject();
		}
	}

	{
		//隠れる処理
		int objectChipNo = objectData::tutorialObject[underfootChipNoY - 1][underfootChipNoX];
		if (objectChipNo == 36 || objectChipNo == 37) {
			if (objectCollision(underfootChipNoX, underfootChipNoY - 1)) {
				if (input.isPressed(InputType::next)) {
					DrawString(300, 100, "aiueo", 0xffffff);
					push = true;
				}
				else {
					push = false;
				}
			}
		}
		else {
			hidden = false;
			push = false;
		}
	}
	
}

void Player::BossUpdate(Vec2 offset, const InputState& input)
{
	rightClosure = false;
	leftClosure = false;

	//モーション関係
	motionNum = 0;
	motion->update(motionNum);

	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//配列の中身を見る
	int chipNo = buildingData::bossBuilding[underfootChipNoY][underfootChipNoX];

	//梯子を降りる
	if (chipNo == 119 || chipNo == 121) {
		if (input.isPressed(InputType::down)) {
			playerPos.y += 3.0f;
			if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
			}
		}
	}

	//梯子を上る
	for (int x = -1; x < 1; x++) {
		int chipNo = buildingData::bossBuilding[underfootChipNoY - 1][underfootChipNoX - x];
		if (chipNo == 119 || chipNo == 121) {
			if (input.isPressed(InputType::up)) {
				if (ladderCollision(underfootChipNoX - x, underfootChipNoY - 1)) {
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	//降下
	int DescentChipNo3 = groundData::bossGround2[underfootChipNoY][underfootChipNoX];
	int DescentChipNo4 = buildingData::bossBuilding[underfootChipNoY][underfootChipNoX];
	int DescentChipNo5 = groundData::bossGround[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0 && DescentChipNo4 == 0 && DescentChipNo5 == 0) {
		updateFunc = &Player::updateDescent;
	}



	//つま先の配列番号を見る
	int tiptoeChipNoY = (playerPos.y + chipSize * 2) / chipSize;

	if (!playerDirections) {
		int rightTiptoeChipNoX = (playerPos.x + correctionSizeX + chipSize) / chipSize;
		int chipNo2 = groundData::bossGround[tiptoeChipNoY][rightTiptoeChipNoX];
		if (chipNo2 != 0) {
			rightClosure = true;
		}
	}
	else {
		int leftTiptoeChipNoX = (playerPos.x + correctionSizeX - chipSize) / chipSize;
		int chipNo2 = groundData::bossGround[tiptoeChipNoY][leftTiptoeChipNoX];
		if (chipNo2 != 0) {
			leftClosure = true;
		}
	}
	
	

	//移動
	if (!push) {
		if (input.isPressed(InputType::left)) {
			if (!leftClosure) {
				if (!(motionNum == 3)) {
					motionNum = 1;
				}
				if (--soundCount == 0) {
					ChangeVolumeSoundMem(130, walkSound);
					PlaySoundMem(walkSound, DX_PLAYTYPE_BACK, true);
					soundCount = 20;
				}
				motion->update(motionNum);
				playerPos.x -= 5;
				playerDirections = true;
			}
		}
		else if (input.isPressed(InputType::right)) {
			if (!rightClosure) {
				if (!(motionNum == 3)) {
					motionNum = 1;
				}
				if (--soundCount == 0) {
					ChangeVolumeSoundMem(130, walkSound);
					PlaySoundMem(walkSound, DX_PLAYTYPE_BACK, true);
					soundCount = 20;
				}
				motion->update(motionNum);
				playerPos.x += 5;
				playerDirections = false;
			}
		}
	}


	//近接攻撃
	if (!push) {
		if (!flyingObject->isEnable()) {
			if (input.isPressed(InputType::attack)) {
				ChangeVolumeSoundMem(soundVolume, attackSound);

				PlaySoundMem(attackSound, DX_PLAYTYPE_NORMAL, true);
				motionNum = 3;
				motion->update(motionNum);
				proximityAttack = true;
			}
			else {
				proximityAttack = false;
			}
		}
	}

	//アイテムの切り替え
	switch (inventory->setCurrentInputIndex()) {
	case 0:
		//投擲
		if (!push) {
			if (input.isTriggered(InputType::shot)) {
				if (!flyingObject->isEnable()) {
					flyingObject->bossAttack(playerPos, playerDirections);
				}
			}
		}
		break;
	case 1:
		//修復タイマー

		if (repairBlock > 0) {
			if (input.isPressed(InputType::shot)) {
				spaceHpDisplay = true;
			}
			else {
				spaceHpDisplay = false;
				motionStart = false;
			}
		}
		else {
			spaceHpDisplay = false;
			motionStart = false;
		}

		break;
	case 2:
		if (recoveryItem > 0) {
			if (input.isTriggered(InputType::shot)) {
				if (playerHp < 10) {
					playerHp = (std::min)({ playerHp + 5, hp->returnMaxHp() });
					recoveryItem--;
				}
			}
		}
		break;
	}

	//得物を回収
	if (flyingObject->landing()) {
		if (flyingObject->playerCollision(playerPos, offset)) {
			flyingObject->deadFlyingObject();
		}
	}
}

void Player::updateField(Vec2 offset, const InputState& input)
{
	//モーション関係
	motionNum = 0;
	motion->update(motionNum);

	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) /chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//配列の中身を見る
	int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX];

	//梯子を降りる
	if (chipNo == 119 || chipNo == 121) {
		if (input.isPressed(InputType::down)) {
			playerPos.y += 3.0f;
			if (ladderCollision(underfootChipNoX, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
			}
		}
	}

	//梯子を上る
	for (int x = -1; x < 1; x++) {
		int chipNo = buildingData::building[underfootChipNoY - 1][underfootChipNoX - x];
		if (chipNo == 119 || chipNo == 121) {
			if (input.isPressed(InputType::up)) {
				if (ladderCollision(underfootChipNoX - x, underfootChipNoY - 1)) {
					updateFunc = &Player::updateLadder;
					return;
				}
			}
		}
	}

	//降下
	int DescentChipNo3 = groundData::ground[underfootChipNoY][underfootChipNoX];
	int DescentChipNo4 = buildingData::building[underfootChipNoY][underfootChipNoX];
	if (DescentChipNo3 == 0 && DescentChipNo4 == 0) {
		updateFunc = &Player::updateDescent;
	}

	//移動
	if (!push) {
		if (input.isPressed(InputType::left)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			if (--soundCount == 0) {
				ChangeVolumeSoundMem(130, walkSound);
				PlaySoundMem(walkSound, DX_PLAYTYPE_BACK, true);
				soundCount = 20;
			}
			motion->update(motionNum);
			playerPos.x -= 5;
			playerDirections = true;
		}
		else if (input.isPressed(InputType::right)) {
			if (!(motionNum == 3)) {
				motionNum = 1;
			}
			if (--soundCount == 0) {
				ChangeVolumeSoundMem(130, walkSound);
				PlaySoundMem(walkSound, DX_PLAYTYPE_BACK, true);
				soundCount = 20;
			}
			motion->update(motionNum);
			playerPos.x += 5;
			playerDirections = false;
		}
	}
	

	//近接攻撃
	if (!push) {
		if (!flyingObject->isEnable()) {
			if (input.isTriggered(InputType::attack)) {
				ChangeVolumeSoundMem(soundVolume, attackSound);
				PlaySoundMem(attackSound, DX_PLAYTYPE_BACK, true);
				motionNum = 3;
				motion->update(motionNum);
				proximityAttack = true;
			}
			else {
				proximityAttack = false;
			}
		}
	}

	//アイテムの切り替え
	switch (inventory->setCurrentInputIndex()) {
	case 0:
		//投擲
		if (!push) {
			if (input.isTriggered(InputType::shot)) {
				if (!flyingObject->isEnable()) {
					flyingObject->attack(playerPos, playerDirections);
				}
			}
		}
		break;
	case 1:
		//修復タイマー
		
		if (repairBlock > 0) {
			if (input.isPressed(InputType::shot)) {
				spaceHpDisplay = true;
			}
			else {
				spaceHpDisplay = false;
				motionStart = false;
			}
		}
		else {
			spaceHpDisplay = false;
			motionStart = false;
		}
		
		break;
	case 2:
		if (recoveryItem > 0) {
			if (input.isTriggered(InputType::shot)) {
				if (playerHp < 10) {
					playerHp = (std::min)({ playerHp + 5, hp->returnMaxHp()});
					recoveryItem--;
				}
			}
		}
		break;
	}
	
	//得物を回収
	if (flyingObject->landing()) {
		if (flyingObject->playerCollision(playerPos, offset)) {
			flyingObject->deadFlyingObject();
		}
	}

	//隠れる処理
	int objectChipNo = objectData::object[underfootChipNoY - 1][underfootChipNoX];
	if (objectChipNo == 36 || objectChipNo == 37) {
		if (objectCollision(underfootChipNoX, underfootChipNoY - 1)) {
			if (input.isPressed(InputType::next)) {
				DrawString(300, 100, "aiueo", 0xffffff);
				push = true;
			}
			else {
				push = false;
			}
		}
	}
	else {
		hidden = false;
		push = false;
	}
}

void Player::updateDescent(Vec2 offset, const InputState& input)
{
	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + chipSize) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 3) / chipSize;

	//降下
	playerPos += vec;

	switch (sceneNum) {
	case 0:
		//地面との判定
		for (int x = 0; x < bgNumX; x++) {
			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = groundData::ground[y][x];

				//地面
				if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
					if (playerFiledCollision(x, y)) {
						if (playerPos.y + correctionSizeY > y * chipSize) {
							playerPos.y = y * chipSize - 74;
						}

						updateFunc = &Player::updateField;
						break;
						return;
					}
				}
			}
		}
		break;
	case 1:
		//地面との判定
		for (int x = 0; x < tutorialNumX; x++) {
			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = groundData::tutorialGround[y][x];

				//地面
				if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
					if (playerFiledCollision(x, y)) {
						if (playerPos.y + correctionSizeY > y * chipSize) {
							playerPos.y = y * chipSize - 74;
						}

						updateFunc = &Player::tutorialUpdate;
						return;
					}
				}
			}
		}
		break;
	case 2:
		//地面との判定
		for (int x = 0; x < bgNumX; x++) {
			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = groundData::bossGround2[y][x];

				//地面
				if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
					if (playerFiledCollision(x, y)) {
						if (playerPos.y + correctionSizeY > y * chipSize) {
							playerPos.y = y * chipSize - 74;
						}

						updateFunc = &Player::BossUpdate;
						return;
					}
				}
			}
		}
		break;
	}

	

	//梯子との判定
	for (int x = -1; x < 1; x++) {

		const int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX + x];
		//地面
		if (chipNo == 119 || chipNo == 121) {
			if (objectCollision(underfootChipNoX + x, underfootChipNoY)) {
				updateFunc = &Player::updateLadder;
				return;
			}
		}
	}

}

void Player::updateLadder(Vec2 offset, const InputState& input)
{
	//足元の配列番号を見る
	int underfootChipNoX = (playerPos.x + correctionSizeX) / chipSize;
	int underfootChipNoY = (playerPos.y + chipSize * 2) / chipSize;

	switch (sceneNum) {
	case 0:
		//登りきった後
		if (input.isPressed(InputType::up)) {

			motionNum = 2;
			playerPos.y -= 5;
			motion->update(motionNum);

			int chipNo = buildingData::building[underfootChipNoY][underfootChipNoX];
			//地面
			if (chipNo == 0 || chipNo == 106 || chipNo == 108) {
				if (playerFiledCollision(underfootChipNoX, underfootChipNoY)) {
					updateFunc = &Player::updateField;
					return;
				}
			}
		}

		//下ったとき
		if (input.isPressed(InputType::down)) {
			motionNum = 2;
			playerPos.y += 5;
			motion->update(motionNum);

			int chipNo = groundData::ground[underfootChipNoY + 1][underfootChipNoX];
			//地面
			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				if (playerFiledCollision(underfootChipNoX, underfootChipNoY + 1)) {
					motionNum = 0;
					updateFunc = &Player::updateField;
					return;
				}
			}
		}
		break;
	case 1:
		//登りきった後
		if (input.isPressed(InputType::up)) {

			motionNum = 2;
			playerPos.y -= 5;
			motion->update(motionNum);

			int chipNo = buildingData::tutorialBuilding[underfootChipNoY][underfootChipNoX];
			//地面
			if (chipNo == 0 || chipNo == 106 || chipNo == 108) {
				if (playerFiledCollision(underfootChipNoX, underfootChipNoY)) {
					updateFunc = &Player::tutorialUpdate;
					return;
				}
			}
		}

		//下ったとき
		if (input.isPressed(InputType::down)) {
			motionNum = 2;
			playerPos.y += 5;
			motion->update(motionNum);

			int chipNo = groundData::tutorialGround[underfootChipNoY + 1][underfootChipNoX];
			//地面
			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo == 45 || chipNo == 46) {
				if (playerFiledCollision(underfootChipNoX, underfootChipNoY + 1)) {
					motionNum = 0;
					updateFunc = &Player::tutorialUpdate;
					return;
				}
			}
		}
		break;
	case 2:
		//登りきった後
		if (input.isPressed(InputType::up)) {

			motionNum = 2;
			playerPos.y -= 5;
			motion->update(motionNum);

			int chipNo = buildingData::bossBuilding[underfootChipNoY][underfootChipNoX];
			//地面
			if (chipNo == 0 || chipNo == 106 || chipNo == 108) {
				if (playerFiledCollision(underfootChipNoX, underfootChipNoY)) {
					updateFunc = &Player::BossUpdate;
					return;
				}
			}
		}

		//下ったとき
		if (input.isPressed(InputType::down)) {
			motionNum = 2;
			playerPos.y += 5;
			motion->update(motionNum);

			int chipNo = groundData::bossGround[underfootChipNoY + 1][underfootChipNoX];
			int chipNo2 = groundData::bossGround2[underfootChipNoY + 1][underfootChipNoX];
			//地面
			if (chipNo == 53 || chipNo == 60 || chipNo == 61 || chipNo == 31 || chipNo == 32 || chipNo2 == 45 || chipNo2 == 46 || chipNo2 == 53 || chipNo2 == 60 || chipNo2 == 61 || chipNo2 == 31 || chipNo2 == 32) {
				if (playerFiledCollision(underfootChipNoX, underfootChipNoY + 1)) {
					motionNum = 0;
					updateFunc = &Player::BossUpdate;
					return;
				}
			}
		}
		break;
	}

	

	

	
	//落下
	//chipNo3 = buildingData::building[underfootChipNoY + 1][underfootChipNoX];
	//if (chipNo3 == 0) {
	//	motionNum = 0;
	//	motion->update(motionNum);
	//	//updateFunc = &Player::updateDescent;
	//}
}

void Player::updateDeath(Vec2 offset, const InputState& input)
{
	motion->update(motionNum);
	if (motion->setDead()) {
		gameoverScene = true;
	}
}

void Player::setItemControl(int num)
{
	switch (num) {
	case 0:
		hp->setObjectMaxHp(hp->returnMaxHp() + 1);
		break;
	case 1:
		repairBlock++;
		break;
	case 2:
		recoveryItem++;
		break;
	}
}

void Player::setMoney(int amount)
{
	money -= amount;
	ChangeVolumeSoundMem(soundVolume, coinSound);
	PlaySoundMem(coinSound, DX_PLAYTYPE_BACK, true);
}

void Player::consumption()
{
	repairBlock--;
}

void Player::setMotion(bool start)
{
	if (!motionStart) {
		motionStart = start;
	}
}

bool Player::returnFlyingisEnabled()
{
	return flyingObject->isEnable();
}

//描画
void Player::draw(Vec2 offset)
{
	hp->playerHpDraw(hpHandle);

	inventory->setNum(repairBlock, recoveryItem, flyingObject->isEnable());
	inventory->draw();

	DrawRotaGraph(1810, 50, 2.0f, 0.0f, coinHandle, true);
	DrawFormatString(1845, 50, 0xffffff, "%d", money);

	//飛び道具
	if (flyingObject->isEnable()) {
		if (sceneNum != 2) {
			flyingObject->draw(macheteHandle, offset);
		}
		else {
			flyingObject->bossDraw(macheteHandle, offset);
		}
		
	}

	if (ultimateTimer_ > 0) {
		if ((ultimateTimer_ / 10) % 2 == 0) {
			return;
		}
	}

	if (motionStart) {
		imgX++;
		if (imgX > 9) {
			imgY++;
			imgX = 0;
		}
		if (imgY == 8 && imgX > 3) {
			imgY = 0;
			imgX = 0;
		}
	}
	else {
		imgX = 0;
		imgY = 0;
	}
	
	if (!push) {
		motion->draw(playerPos, playerHandle, playerDirections, offset);
	}

	if (motionStart) {
		DrawRectRotaGraph(playerPos.x + offset.x - 10, playerPos.y, imgX * 100, imgY * 100, 100, 100, 1.0f, 0.0f, smokeHandle, true, false);
		DrawRectRotaGraph(playerPos.x + offset.x + 30, playerPos.y, imgX * 100, imgY * 100, 100, 100, 1.0f, 0.0f, smokeHandle, true, false);
		DrawRectRotaGraph(playerPos.x + offset.x + 30, playerPos.y + 30, imgX * 100, imgY * 100, 100, 100, 1.0f, 0.0f, smokeHandle, true, false);
		DrawRectRotaGraph(playerPos.x + offset.x - 10, playerPos.y + 30, imgX * 100, imgY * 100, 100, 100, 1.0f, 0.0f, smokeHandle, true, false);
	}
}


//隠れる
bool Player::beHidden()
{
	return push;
}



void Player::damege(bool inversion)
{
	if (ultimateTimer_ <= 0) {
		if (!inversion) {
			playerPos.x += 30.0f;
		}
		else {
			playerPos.x -= 30.0f;
		}

		if (playerHp > 0) {
			playerHp--;
		}
		ultimateTimer_ = ultimate_frames;
	}

}

void Player::bossDamege()
{
	playerPos.x += 30.0f;
	
	playerHp -= playerHp;
}

int Player::enemyAttack(Vec2 enemyPos, Vec2 offset)
{
	if (flyingObject->enemyCollision(enemyPos,offset)) {
		enemyHit = true;
	}
	else {
		enemyHit = false;
	}

	return enemyHit;
}

int Player::bossEnemyAttack(Vec2 enemyPos, Vec2 offset)
{
	if (flyingObject->bossEnemyCollision(enemyPos, offset)) {
		enemyHit = true;
	}

	return enemyHit;
}



//++++++++++++++++++++++++++++++++++
//　　　　　　当たり判定
//++++++++++++++++++++++++++++++++++

//近接攻撃の当たり判定
bool Player::proximityAttackCollision(const Vec2& pos)
{
	float enemyLeft = pos.x;
	float enemyTop = pos.y;
	float enemyRight = pos.x + 40;
	float enemyBottom = pos.y + 40;

	if (proximityAttack) {
		if (playerDirections) {
			if (enemyLeft > playerPos.x + 20)			return false;
			if (enemyRight < playerPos.x - 50)			return false;
			if (enemyTop > playerPos.y + 50)			return false;
			if (enemyBottom < playerPos.y + 10)			return false;
		}
		else if (!playerDirections) {
			if (enemyLeft > playerPos.x  + 80)			return false;
			if (enemyRight < playerPos.x + 20)			return false;
			if (enemyTop > playerPos.y + 50)			return false;
			if (enemyBottom < playerPos.y + 10)			return false;
		}
		return true;
	}
	return false;
}

//空間を修復する
bool Player::repairSpace(const Vec2& pos,Vec2 offset)
{
	float spaceLeft = pos.x + offset.x;
	float spaceTop = pos.y + offset.y;
	float spaceRight = pos.x + chipSize * 3 + offset.x;
	float spaceBottom = pos.y + chipSize * 3 + offset.y;

	float playerLeft = playerPos.x + 14 + offset.x;
	float playerTop = playerPos.y + offset.y;
	float playerRight = playerPos.x + 15 + offset.x;
	float playerBottom = playerPos.y + 74 + offset.y;

	if (playerRight < spaceLeft)				return false;
	if (playerLeft > spaceRight)				return false;
	if (playerBottom < spaceTop)				return false;
	if (playerTop > spaceBottom)				return false;
		
	return true;

}

bool Player::playerFiledCollision(int x,int y)
{
	float fieldLeft = x * chipSize;
	float fieldRight = x * chipSize + chipSize;
	float fieldTop = y * chipSize;
	float fieldBottom = y * chipSize + chipSize;

	if (playerPos.x + 15.0f < fieldLeft)				return false;
	if (playerPos.x + correctionSizeX > fieldRight)		return false;
	if (playerPos.y + correctionSizeY < fieldTop)		return false;
	if (playerPos.y > fieldBottom)						return false;

	return true;
}

bool Player::objectCollision(int x,int y)
{

	float objectLeft = x * chipSize;
	float objectRight = x * chipSize  + chipSize ;
	float objectTop = y * chipSize;
	float objectBottom = y * chipSize + chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;
		
	return true;
}

bool Player::ladderCollision(int x, int y)
{
	float objectLeft = x * chipSize ;
	float objectRight = x * chipSize + chipSize;
	float objectTop = y * chipSize;
	float objectBottom = y * chipSize + chipSize;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	if (playerPos.y > objectBottom)						return false;

	return true;
}

bool Player::coinCollision(Vec2 pos, Vec2 offset)
{
	float objectLeft =pos.x + offset.x;
	float objectRight = pos.x + 30 + offset.x;
	float objectTop = pos.y + offset.y;
	float objectBottom = pos.y + 30 + offset.y;

	if (playerPos.x + 15.0f + offset.x < objectLeft)				return false;
	if (playerPos.x + correctionSizeX + offset.x > objectRight)		return false;
	if (playerPos.y + correctionSizeY + offset.y < objectTop)		return false;
	if (playerPos.y + offset.y > objectBottom)						return false;

	money += 1000;
	ChangeVolumeSoundMem(soundVolume, coinSound);
	PlaySoundMem(coinSound, DX_PLAYTYPE_BACK, true);

	return true;
}

bool Player::shopCollision(int x, int y, Vec2 offset)
{
	float objectLeft = x * chipSize;
	float objectRight = x * chipSize + chipSize;
	float objectTop = y * chipSize;
	float objectBottom = y * chipSize + chipSize * 3;

	if (playerPos.x + 15.0f < objectLeft)				return false;
	DrawString(300, 400, "aiu", 0xffffff);
	if (playerPos.x + correctionSizeX > objectRight)	return false;
	DrawString(300, 400, "aiu", 0xffffff);
	if (playerPos.y + correctionSizeY < objectTop)		return false;
	DrawString(300, 400, "aiu", 0xffffff);
	if (playerPos.y > objectBottom)						return false;

	return true;
}






