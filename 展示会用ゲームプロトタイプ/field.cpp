#include "field.h"
#include "Pad.h"
#include "DxLib.h"

namespace {
	constexpr float speed = 8.0f;
}

Field::Field()
{
}

Field::~Field()
{
}

void Field::update()
{
}

void Field::draw(Vec2 offset)
{
	//DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight,0xffffff, true);

	for (int x = 0; x < FieldData::bgNumX; x++) {
		for (int y = 0; y < FieldData::bgNumY; y++) {

			const int chipNo = FieldData::field[y][x];

			int posX = x * FieldData::chipSize + static_cast<int>(offset.x);
			int posY = y * FieldData::chipSize;

			if (chipNo == 1) {
				DrawBox(posX, posY, posX + FieldData::chipSize, posY + FieldData::chipSize, 0xffff00, true);
			}
			else if (chipNo == 2) {
				DrawBox(posX, posY, posX + FieldData::chipSize, posY + FieldData::chipSize, 0x444444, true);
			}
			else if (chipNo == 3) {
				DrawBox(posX, posY, posX + FieldData::chipSize, posY + FieldData::chipSize, 0x44ff44, true);
			}
			else if (chipNo == 5) {
				DrawBox(posX, posY, posX + FieldData::chipSize, posY + FieldData::chipSize, 0x999999, true);
			}
			else if (chipNo == 6) {
				DrawBox(posX, posY, posX + FieldData::chipSize, posY + FieldData::chipSize, 0x6f4b2c, true);
			}
			//DrawFormatString(posX, posY, 0x000000, "%d", posX);
		}
	}

	DrawCircle(1732 + offset.x, 748 + offset.y, 4, 0xffffff, true);
	DrawCircle(1754 + offset.x, 748 + offset.y, 4, 0xffffff, true);

	DrawCircle(1731 + offset.x, 747 + offset.y, 2, 0x000000, true);
	DrawCircle(1755 + offset.x, 747 + offset.y, 2, 0x000000, true);

	DrawBox(1736 + offset.x, 752 + offset.y, 1736 + offset.x + 10, 752 + offset.y + 5,0xff0000, true);
}

int Field::getWidth() const
{
	return FieldData::bgNumX * FieldData::chipSize;
}

int Field::getHeight() const
{
	return FieldData::bgNumY * FieldData::chipSize;
}
