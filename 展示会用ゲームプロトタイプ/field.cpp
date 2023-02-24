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
	groundHandle = my::myLoadGraph("data/Tiles-1.png");
	buildingHandle = my::myLoadGraph("data/Buildings1.png");
	objectHandle = my::myLoadGraph("data/Props-01.png");
	GetGraphSize(groundHandle, &groundGraphWidth, &groundGraphHeight);
	GetGraphSize(buildingHandle, &buildingGraphWidth, &buildingGraphHeight);
	GetGraphSize(objectHandle, &objectGraphWidth, &objectGraphHeight);
}

Field::~Field()
{
	DeleteGraph(groundHandle);
	DeleteGraph(buildingHandle);
	DeleteGraph(objectHandle);
}

void Field::update()
{
}

void Field::draw(Vec2 offset)
{
	DrawCircle(1732 + offset.x, 748 + offset.y, 4, 0xffffff, true);
	DrawCircle(1754 + offset.x, 748 + offset.y, 4, 0xffffff, true);

	DrawCircle(1731 + offset.x, 747 + offset.y, 2, 0x000000, true);
	DrawCircle(1755 + offset.x, 747 + offset.y, 2, 0x000000, true);

	DrawBox(1736 + offset.x, 752 + offset.y, 1736 + offset.x + 10, 752 + offset.y + 5, 0xff0000, true);

	
	for (int x = 0; x < bgNumX; x++) {
		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = groundData::ground[y][x];

			assert(chipNo >= 0);
			assert(chipNo < chipNum(groundGraphWidth, groundGraphHeight));

			int posX = x * chipSize + static_cast<int>(offset.x);
			int posY = y * chipSize;

			int graphX = (chipNo % chipNumX(groundGraphWidth)) * graphChipSize;
			int graphY = (chipNo / chipNumX(groundGraphWidth)) * graphChipSize;

			DrawRectRotaGraph(posX + graphChipSize, posY + graphChipSize, graphX, graphY, graphChipSize, graphChipSize, 2.0f, 0.0f, groundHandle, true, false);

		}
	}

	for (int x = 0; x < bgNumX; x++) {
		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = buildingData::building[y][x];

			assert(chipNo >= 0);
			assert(chipNo < chipNum(buildingGraphWidth, buildingGraphHeight));

			int posX = x * chipSize + static_cast<int>(offset.x) + graphChipSize;
			int posY = y * chipSize + graphChipSize;

			int graphX = (chipNo % chipNumX(buildingGraphWidth)) * graphChipSize;
			int graphY = (chipNo / chipNumX(buildingGraphWidth)) * graphChipSize;

			DrawRectRotaGraph(posX, posY, graphX, graphY, graphChipSize, graphChipSize, 2.0f, 0.0f, buildingHandle, true, false);

		}
	}

	for (int x = 0; x < bgNumX; x++) {
		for (int y = 0; y < bgNumY; y++) {

			const int chipNo = objectData::object[y][x];

			assert(chipNo >= 0);
			assert(chipNo < chipNum(objectGraphWidth, objectGraphHeight));

			int posX = x * chipSize + static_cast<int>(offset.x) + graphChipSize;
			int posY = y * chipSize + graphChipSize;

			int graphX = (chipNo % chipNumX(objectGraphWidth)) * graphChipSize;
			int graphY = (chipNo / chipNumX(objectGraphWidth)) * graphChipSize;

			DrawRectRotaGraph(posX, posY, graphX, graphY, graphChipSize, graphChipSize, 2.0f, 0.0f, objectHandle, true, false);

		}
	}
}


int Field::chipNumX(int width)
{
	return (width / graphChipSize);
}

int Field::chipNumY(int height)
{
	return (height / graphChipSize);
}

int Field::chipNum(int width, int height)
{
	return (chipNumX(width) * chipNumY(height));
}



int Field::getWidth() const
{
	return bgNumX * chipSize;
}

int Field::getHeight() const
{
	return bgNumY * chipSize;
}
