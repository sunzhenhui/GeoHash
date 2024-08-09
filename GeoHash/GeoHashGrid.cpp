#include "GeoHashGrid.h"
#include "GeoHashPoint.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

void GeoHashGrid::GeneratePoints(int nPointNum)
{
    // 清空之前的点
    mvPoints.clear();
    mnCircleCount = nPointNum;

    // 设置随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < mnCircleCount; i++)
    {
        // 生成随机点的坐标
        double dRandomX = mdX0 + (static_cast<double>(std::rand()) / RAND_MAX) * mdWidth;
        double dRandomY = mdY0 + (static_cast<double>(std::rand()) / RAND_MAX) * mdHeight;
        mvPoints.push_back(GeoHashPoint(dRandomX, dRandomY, 5.0, 1000.0));
    }
}