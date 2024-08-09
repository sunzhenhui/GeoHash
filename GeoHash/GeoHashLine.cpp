#include "GeoHashLine.h"
#include "GeoHashPoint.h"
#include "Trie.h"

#include <cmath>
#include <utility>

std::pair<double, double> GeoHashLine::Intersect(double dXMin, double dYMin, double dXMax, double dYMax)
{
    // 计算射线的方向向量
    double dx = cos(mdAngle);
    double dy = sin(mdAngle);

    // 射线的参数方程: (x, y) = (mdX0 + t * dx, mdY0 + t * dy)
    // t 为参数
    std::pair<double, double> closestIntersection = {NAN, NAN};
    double dMinDistance = std::numeric_limits<double>::max();

    // 计算与四条边的交点
    // 与左边交点
    double t1 = (dXMin - mdX0) / dx;
    double y1 = mdY0 + t1 * dy;
    if (t1 >= 0 && y1 >= dYMin && y1 <= dYMax)
    {
        double distance = sqrt(pow(dXMin - mdX0, 2) + pow(y1 - mdY0, 2));
        if (distance < dMinDistance)
        {
            dMinDistance = distance;
            closestIntersection = {dXMin, y1};
        }
    }

    // 与右边交点
    double t2 = (dXMax - mdX0) / dx;
    double y2 = mdY0 + t2 * dy;
    if (t2 >= 0 && y2 >= dYMin && y2 <= dYMax)
    {
        double distance = sqrt(pow(dXMax - mdX0, 2) + pow(y2 - mdY0, 2));
        if (distance < dMinDistance)
        {
            dMinDistance = distance;
            closestIntersection = {dXMax, y2};
        }
    }

    // 与下边交点
    double t3 = (dYMin - mdY0) / dy;
    double x3 = mdX0 + t3 * dx;
    if (t3 >= 0 && x3 >= dXMin && x3 <= dXMax)
    {
        double distance = sqrt(pow(x3 - mdX0, 2) + pow(dYMin - mdY0, 2));
        if (distance < dMinDistance)
        {
            dMinDistance = distance;
            closestIntersection = {x3, dYMin};
        }
    }

    // 与上边交点
    double t4 = (dYMax - mdY0) / dy;
    double x4 = mdX0 + t4 * dx;
    if (t4 >= 0 && x4 >= dXMin && x4 <= dXMax)
    {
        double distance = sqrt(pow(x4 - mdX0, 2) + pow(dYMax - mdY0, 2));
        if (distance < dMinDistance)
        {
            dMinDistance = distance;
            closestIntersection = {x4, dYMax};
        }
    }

    // 返回距离原点最近的交点
    return closestIntersection;
}

void GeoHashLine::GenerateIntervalPoints(double dRadius, double dXMin, double dYMin, double dXMax, double dYMax)
{
    // 计算射线的方向向量
    double dx = cos(mdAngle);
    double dy = sin(mdAngle);

    // 找到最近的交点
    auto intersection = Intersect(dXMin, dYMin, dXMax, dYMax);
    if (std::isnan(intersection.first) || std::isnan(intersection.second))
    {
        return;
    }

    // 从交点开始生成等间距的点
    double dStartX = intersection.first;
    double dStartY = intersection.second;

    // 计算从交点到正方形边界的最小距离
    double dDistanceToLeft = (dXMin - dStartX) / dx;
    double dDistanceToRight = (dXMax - dStartX) / dx;
    double dDistanceToBottom = (dYMin - dStartY) / dy;
    double dDistanceToTop = (dYMax - dStartY) / dy;

    // 计算最大有效距离
    double dMaxDistance = std::max({dDistanceToLeft, dDistanceToRight, dDistanceToBottom, dDistanceToTop});

    // 根据间隔长度计算可以生成的点的数量
    int nPointsCount = static_cast<int>(dMaxDistance / dRadius);
    if (nPointsCount <= 0)
    {
        return;
    }

    // 生成等间距的点，从1开始，以避免重复交点
    for (int i = 1; i <= nPointsCount; i++)
    {
        double _X = dStartX + i * dRadius * dx;
        double _Y = dStartY + i * dRadius * dy;
        mPoints.emplace_back(GeoHashPoint(_X, _Y));
    }
}

void GeoHashLine::CalculateGeoHash(int nByteLength)
{
    for (int i = 0; i < mPoints.size(); i++)
    {
        mPoints[i].CalculateGeoHash(nByteLength);
    }
}

void GeoHashLine::FindLineCrossCircle(std::vector<GeoHashPoint> &vPoints, std::unordered_map<std::string, std::vector<int>> &Map)
{
    // 找到当前节点直接穿过的点，也就是射线等间隔取点，判断依旧邻近的点（<= dRadius）
    for (int i = 0; i < mPoints.size(); i++)
    {
        std::string strHash = mPoints[i].GetGeoHash();
        auto it = Map.find(strHash);
        if (it != Map.end())
        {
            for (int j = 0; j < it->second.size(); j++)
            {
                vPoints[it->second[j]].SetState(true);
            }
            Map.erase(it); // 减少后续判断数据量
        }
    }
}

void GeoHashLine::FindSuspectedLineCrossCircle(std::vector<GeoHashPoint> &vPoints, std::unordered_map<std::string, std::vector<int>> Map)
{
    // 这个是筛选疑似穿过的点，也就是所有 [dRadius, dRadius × sqrt(2)] 的点，同时完成最终判断
    Trie trie;
    for (const auto &pair : Map)
    {
        trie.insert(pair.first, pair.second);
    }

    // 遍历 mPoints，查找对应的前缀
    for (int i = 0; i < mPoints.size(); i++)
    {
        std::string strHash = mPoints[i].GetGeoHash();
        std::vector<int> values = trie.findPrefix(strHash);
        if (!values.empty())
        {
            // 前缀一致，处理 values
            for (int j = 0; j < values.size(); j++)
            {
                if (DistanceToRay(vPoints[values[j]].GetX(), vPoints[values[j]].GetY()) < vPoints[values[j]].GetRadius())
                {
                    vPoints[values[j]].SetState(true);
                }
            }
        }
    }
}

double GeoHashLine::DistanceToRay(double dPointX, double dPointY)
{
    // 射线的方向向量
    double dx = cos(mdAngle);
    double dy = sin(mdAngle);

    // 计算点到起始点的向量
    double dapx = dPointX - mdX0; // P - A
    double dapy = dPointY - mdY0;

    // 计算投影长度
    double dDotProduct = dapx * dx + dapy * dy; // AP · D
    double dProjectionLength = dDotProduct;     // 射线的投影长度

    // 计算投影点的坐标
    double dProjectionX = mdX0 + dProjectionLength * dx;
    double dProjectionY = mdY0 + dProjectionLength * dy;

    // 计算点到投影点的距离
    double dDistance = sqrt(pow(dPointX - dProjectionX, 2) + pow(dPointY - dProjectionY, 2));
    return dDistance;
}

double GeoHashLine::GetX0()
{
    return mdX0;
}

double GeoHashLine::GetY0()
{
    return mdY0;
}

double GeoHashLine::GetAngle()
{
    return mdAngle;
}
