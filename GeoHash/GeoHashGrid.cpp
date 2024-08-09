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
            if (mpLine)
            {
                delete mpLine;
                mpLine = nullptr;
            }
            mpLine = new GeoHashLine(dX0, dY0, angle);
        }
    }
}

void GeoHashGrid::GenerateGeoHash()
{
    // 这里需要根据圆形的半径计算所需要的 GeoHash 位数，这个位数必须为 2 的 n 次方，用于生成 GeoHash 字符串
    double dRadius = mvPoints[0].GetRadius();
    double dEdgeLength = mdHeight >= mdWidth ? mdHeight : mdWidth;
    int nBitNums = 0;
    while (dEdgeLength >= dRadius)
    {
        dEdgeLength = dEdgeLength / 2.0;
        nBitNums++;
    }
    if (nBitNums <= 0)
        nBitNums = 1;

    // 根据需要的 GeoHash 位数生成哈希值
    for (int i = 0; i < mvPoints.size(); i++)
    {
        mvPoints[i].CalculateGeoHash(nBitNums);
    }
    mpLine->CalculateGeoHash(nBitNums);

    // 根据 GeoHash 字段初始化哈希表，筛选一定符合要求的点
    mMap.clear();
    for (int i = 0; i < mvPoints.size(); i++)
    {
        std::string strHash = mvPoints[i].GetGeoHash();
        auto it = mMap.find(strHash);
        if (it != mMap.end())
        {
            // 已经存在，直接添加至列表
            it->second.push_back(i);
        }
        else
        {
            // 添加新的哈希键值对
            mMap[strHash] = std::vector<int>(1, i);
        }
    }

    // 计算距离阈值，其中 <= dRadius (即 GeoHash 字段完全对应的圆形) 一定符合标准，[dRadius, dRadius × sqrt(2)] 的圆形可能符合标准，> dRadius 一定不符合标准
    // 找到 <= dRadius (即 GeoHash 字段完全对应的圆形)
    mpLine->FindLineCrossCircle(mvPoints, mMap);

    // 更新 GeoHash 字符长度
    nBitNums = -1; // 这里相当于向上取整，识别出来的字符串可能大于 dRadius × sqrt(2)，保证覆盖到所有 [dRadius, dRadius × sqrt(2)] 的点
    dEdgeLength = mdHeight >= mdWidth ? mdHeight : mdWidth;
    dRadius = dRadius * sqrt(2);
    while (dEdgeLength >= dRadius)
    {
        dEdgeLength = dEdgeLength / 2.0;
        nBitNums++;
    }
    if (nBitNums <= 0)
        nBitNums = 1;

    mpLine->CalculateGeoHash(nBitNums);
    // 找到 [dRadius, dRadius × sqrt(2)] 的圆形
    mpLine->FindSuspectedLineCrossCircle(mvPoints, mMap);
}

void GeoHashGrid::FindLineCrossCircle()
{
    GenerateGeoHash();
}

GeoHashLine *GeoHashGrid::GetLine()
{
    return mpLine;
}

void GeoHashGrid::OutputCircles()
{
    std::cout << "Grid (" << mdX0 << "," << mdY0 << "," << mdX0 + mdWidth << "," << mdY0 + mdHeight << ")" << std::endl;
    std::cout << "Line (" << mpLine->GetX0() << "," << mpLine->GetY0() << "), Angle:" << mpLine->GetAngle() << std::endl;
    for (int i = 0; i < mvPoints.size(); i++)
    {
        if (mvPoints[i].GetState())
        {
            std::cout << "(" << mvPoints[i].GetX() << "," << mvPoints[i].GetY() << ") ";
        }
    }
    std::cout << std::endl;
}
