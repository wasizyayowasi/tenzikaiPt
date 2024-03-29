#include "field.h"
#include "Pad.h"
#include "DxLib.h"
#include "DrawFunctions.h"
#include <cassert>

namespace {
	constexpr float speed = 8.0f;
}

Field::Field()
{
	groundHandle = my::myLoadGraph("data/FieldGraph/Tiles-2.png");
	buildingHandle = my::myLoadGraph("data/FieldGraph/Buildings1.png");
	objectHandle = my::myLoadGraph("data/FieldGraph/Props-01.png");
	buildingHandle2 = my::myLoadGraph("data/FieldGraph/Buildings2.png");
	backgroundHandle = my::myLoadGraph("data/FieldGraph/BaseColor.png");
	backgroundHandle2 = my::myLoadGraph("data/FieldGraph/BaseColor2.png");
	backgroundHandle3 = my::myLoadGraph("data/FieldGraph/BaseColor3.png");
	shopperHandle = my::myLoadGraph("data/objectGraph/shop.png");
	backBuildingHandle = my::myLoadGraph("data/FieldGraph/BackgroundProps1.png");
	
	GetGraphSize(groundHandle, &groundGraphWidth, &groundGraphHeight);
	GetGraphSize(buildingHandle, &buildingGraphWidth, &buildingGraphHeight);
	GetGraphSize(objectHandle, &objectGraphWidth, &objectGraphHeight);
	GetGraphSize(buildingHandle2, &buildingGraphWidth2, &buildingGraphHeight2);
	GetGraphSize(backBuildingHandle, &backBuildingWidth, &backBuildingHeight);
}

Field::~Field()
{
	DeleteGraph(groundHandle);
	DeleteGraph(buildingHandle);
	DeleteGraph(objectHandle);
	DeleteGraph(buildingHandle2);
	DeleteGraph(backgroundHandle);
	DeleteGraph(backgroundHandle2);
	DeleteGraph(backgroundHandle3);
	DeleteGraph(shopperHandle);
	DeleteGraph(backBuildingHandle);
}

void Field::update()
{
}

void Field::draw(Vec2 offset, int num)
{
	switch (num) {
	//`[gA
	case 0:
	{
		DrawRotaGraph(1920, 540, 2.0f, 0.0f, backgroundHandle2, true, false);

		//ΕΜwi
		for (int x = 0; x < 49; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.2f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::backGround2[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));


				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}
		//©ηρΒΪΜwi
		for (int x = 0; x < 53; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.4f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::backGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));

				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}

		DrawRotaGraph(static_cast < int>(2510 + offset.x), 780, 2.0f, 0.0f, shopperHandle, true);

		for (int x = 0; x < tutorialNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY + 1; y++) {

				const int chipNo = groundData::tutorialGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(groundGraphWidth, groundGraphHeight));


				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(groundGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(groundGraphWidth)) * graphChipSize);

				
				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, groundHandle, true, false);
			
			}
		}

		for (int x = 0; x < tutorialNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = objectData::tutorialObject[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(objectGraphWidth, objectGraphHeight));

				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(objectGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(objectGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, objectHandle, true, false);

			}
		}

		for (int x = 0; x < tutorialNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = buildingData::tutorialBuilding[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(buildingGraphWidth, buildingGraphHeight));

				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(buildingGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(buildingGraphWidth)) * graphChipSize);

				
				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, buildingHandle, true, false);
			}
		}
	}
		break;
	case 1:
	//GAMEMAINΜ`ζ
	{
		DrawRotaGraph(1920, 540, 2.0f, 0.0f, backgroundHandle, true, false);

		//ΕΜwi
		for (int x = 0; x < 49; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.2f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::backGround2[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));

				
				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}
		//©ηρΒΪΜwi
		for (int x = 0; x < 53; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.4f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::backGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));

				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}

		//Xυ
		DrawRotaGraph(static_cast < int>(2130 + offset.x), 780, 2.0f, 0.0f, shopperHandle, true);

		//nΚ
		for (int x = 0; x < bgNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY + 1; y++) {

				const int chipNo = groundData::ground[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(groundGraphWidth, groundGraphHeight));

				
				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(groundGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(groundGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, groundHandle, true, false);

			}
		}
		//¨
		for (int x = 0; x < bgNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = buildingData::building[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(buildingGraphWidth, buildingGraphHeight));

				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(buildingGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(buildingGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, buildingHandle, true, false);

			}
		}
		//IuWFNgή
		for (int x = 0; x < bgNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = objectData::object[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(objectGraphWidth, objectGraphHeight));

				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(objectGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(objectGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, objectHandle, true, false);
				//DrawBox(x * 32 + offset.x, y * 32, x * 32 + chipSize + offset.x, y * 32 + chipSize, 0xff4444, false);
			}
		}
		//όP
		for (int x = 0; x < bgNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = ornamentData::ornament[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(buildingGraphWidth2, buildingGraphHeight2));

				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(buildingGraphWidth2)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(buildingGraphWidth2)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, buildingHandle2, true, false);

			}
		}
		DrawString(static_cast < int>(2110 + offset.x), 660, "shop", 0xffffff);
		//ό2
		for (int x = 0; x < bgNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = ornamentData::ornament2[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(buildingGraphWidth, buildingGraphHeight));

				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(buildingGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(buildingGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, buildingHandle, true, false);
			}
		}
	}
		break;
	case 2:
	//BOSSνΜ`ζ
	{
		//ΕΜwi
		for (int x = 0; x < 199; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.2f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::bossBackGround2[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));

				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}
		//©ηρΒΪΜwi
		for (int x = 0; x < 215; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.4f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::bossBackGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));

				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, static_cast<int>(y * backGroundGraphChipSize + 24), graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}
		//nΚ
		for (int x = 0; x < bossNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < chipSize * 14) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = groundData::bossGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(groundGraphWidth, groundGraphHeight));

				
				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(groundGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(groundGraphWidth)) * graphChipSize);
				if (!(offset.x < -13430)) {
					if (chipNo != 1) {
						DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, groundHandle, true, false);
					}
				}
			}
		}
		//nΚ
		for (int x = 0; x < bossNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY + 1; y++) {

				const int chipNo = groundData::bossGround2[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(groundGraphWidth, groundGraphHeight));


				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(groundGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(groundGraphWidth)) * graphChipSize);
				if (chipNo != 1) {
					DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, groundHandle, true, false);
				}
			}
		}
		//¨
		for (int x = 0; x < bossNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < chipSize * 14) {
				continue;
			}

			for (int y = 0; y < bgNumY; y++) {

				const int chipNo = buildingData::bossBuilding[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(buildingGraphWidth, buildingGraphHeight));

				
				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(buildingGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(buildingGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, buildingHandle, true, false);
			}
		}
	}
		break;
	//^Cg
	case 3:
	{
		DrawRotaGraph(1920, 540, 2.0f, 0.0f, backgroundHandle3, true, false);

		for (int x = 0; x < 49; x++) {

			int posX = static_cast<int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (static_cast<int>(offset.x) * 0.2f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::titleBackGround2[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));


				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}
		//©ηρΒΪΜwi
		for (int x = 0; x < 53; x++) {

			int posX = static_cast <int>((x * backGroundGraphChipSize + backGroundGraphChipSize / 2) + (offset.x * 0.4f));

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < 20 + 1; y++) {

				const int chipNo = backGroundData::titleBackGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(backBuildingWidth, backBuildingHeight));

				int posY = static_cast<int>(y * backGroundGraphChipSize + backGroundGraphChipSize / 2);

				int graphX = static_cast<int>((chipNo % chipNumX(backBuildingWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(backBuildingWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 3.0f, 0.0f, backBuildingHandle, true, false);

			}
		}

		for (int x = 0; x < tutorialNumX; x++) {

			int posX = static_cast<int>(x * chipSize + static_cast<int>(offset.x) + graphChipSize);

			if (posX > Game::kScreenWidth + chipSize) {
				break;
			}
			if (posX < -chipSize) {
				continue;
			}

			for (int y = 0; y < bgNumY + 1; y++) {

				const int chipNo = groundData::titleGround[y][x];

				assert(chipNo >= 0);
				assert(chipNo < chipNum(groundGraphWidth, groundGraphHeight));


				int posY = static_cast<int>(y * chipSize + graphChipSize);

				int graphX = static_cast<int>((chipNo % chipNumX(groundGraphWidth)) * graphChipSize);
				int graphY = static_cast<int>((chipNo / chipNumX(groundGraphWidth)) * graphChipSize);

				DrawRectRotaGraph(posX, posY, graphX, graphY, static_cast<int>(graphChipSize), static_cast<int>(graphChipSize), 2.0f, 0.0f, groundHandle, true, false);

			}
		}
	}
		break;
	}
}


int Field::chipNumX(int width)
{
	return (width / static_cast<int>(graphChipSize));
}

int Field::chipNumY(int height)
{
	return (height / static_cast<int>(graphChipSize));
}

int Field::chipNum(int width, int height)
{
	return (chipNumX(width) * chipNumY(height));
}



int Field::getWidth() const
{
	return bgNumX * chipSize;
}

int Field::getBossWidth() const
{
	return Game::kScreenWidth * 8;
}

int Field::getTutorialWidth() const
{
	return Game::kScreenWidth * 2;
}

int Field::getHeight() const
{
	return bgNumY * chipSize;
}
