#include "DrawFunctions.h"
#include "DxLib.h"
#include <cassert>

namespace my {


    int my::myLoadGraph(const TCHAR* path)
    {
        int handle = LoadGraph(path);
        assert(handle >= 0);
        return handle;
    }

    int my::myDrawRectRotaGraph(int x, int y,int left, int top, int width, int height, float scale, float angle, int handle, bool transFlg, bool TurnFlag)
    {
        return DrawRectRotaGraph(x, y,left,top,width,height,scale,angle,handle,transFlg,TurnFlag);
    }
}

