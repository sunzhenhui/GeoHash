#include "GeoHashGrid.h"
#include "GeoHashPoint.h"
#include "GeoHashLine.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <corecrt_math_defines.h>

GeoHashGrid::~GeoHashGrid()
{
    if (mpLine)
    {
        delete mpLine;
        mpLine = nullptr;
    }
}

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

// 生成原点位于正方形区域外，但是穿过该区域的射线
void GeoHashGrid::GenerateLine()
{
    // 随机选择射线的起始点，确保在矩形区域外部
    double dX0, dY0;

    // 确保起始点在矩形区域外部
    if (rand() % 2 == 0)
    {
        // 随机选择在上方或下方
        dX0 = mdX0 + (rand() % static_cast<int>(mdWidth));
        dY0 = (rand() % 2 == 0) ? mdY0 - 50 : mdY0 + mdHeight + 50;
    }
    else
    {
        // 随机选择在左侧或右侧
        dY0 = mdY0 + (rand() % static_cast<int>(mdHeight));
        dX0 = (rand() % 2 == 0) ? mdX0 - 50 : mdX0 + mdWidth + 50;
    }

    bool bValidLine = false; // 生成的射线是否穿过正方形区域
    while (!bValidLine)
    {
        // 随机生成一个角度（弧度）
        double angle = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI; // 0到2π之间
        GeoHashLine line(dX0, dY0, angle);
        auto intersection = line.Intersect(mdX0, mdY0, mdX0 + mdWidth, mdY0 + mdHeight);
        if (!std::isnan(intersection.first) && !std::isnan(intersection.second))
        {
            bValidLine = true;
            if(mpLine){
                delete mpLine;
                mpLine = nullptr;
            }
            mpLine = new GeoHashLine(dX0, dY0, angle);
        }
    }
}
