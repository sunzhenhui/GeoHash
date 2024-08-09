#pragma once

#include <iostream>
#include <vector>

// 射线类，用于管理一个原点在正方形区域外部，穿过该区域的射线，射线由原点和弧度构成
class GeoHashPoint;
class GeoHashLine
{
public:
    GeoHashLine() : mdX0(0), mdY0(0), mdAngle(0) {}
    GeoHashLine(double dX0, double dY0, double dAngle) : mdX0(dX0), mdY0(dY0), mdAngle(dAngle) {}
    ~GeoHashLine() {}
    std::pair<double, double> Intersect(double dXMin, double dYMin, double dXMax, double dYMax);         // 计算射线与正方形区域的交点
    void GenerateIntervalPoints(double dRadius, double dXMin, double dYMin, double dXMax, double dYMax); // 从最近的交点开始，沿着射线方向生成等间距的间隔点

private:
    double mdX0, mdY0;                 // 原点坐标
    double mdAngle;                    // 射线的角度（弧度）
    std::vector<GeoHashPoint> mPoints; // 射线上生成的等间隔点，用于快速哈希
};