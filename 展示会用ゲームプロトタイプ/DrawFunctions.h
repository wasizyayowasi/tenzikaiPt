#pragma once
#include<tchar.h>

namespace my
{

	int mtLoadGraph(const TCHAR* path);

	int myDrawRectRotaGraph(int x, int y, int left,int top, int width, int height, float scale, float angle, int handle, bool transFlg, bool TurnFlag);

};

