#pragma once

#include <iostream>
#include <vector>

// 网格类，用于记录当前显示的正方形区域，并存储所有点的信息
class GeoHashPoint;
class GeoHashGrid
{
public:
    GeoHashGrid() {}
    GeoHashGrid(double dX0, double dY0, double dWidth, double dHeight) : mdX0(dX0), mdY0(dY0), mdWidth(dWidth), mdHeight(dHeight) {}
    GeoHashGrid(double dX0, double dY0, double dEdgeLength) : mdX0(dX0), mdY0(dY0), mdWidth(dEdgeLength), mdHeight(dEdgeLength) {}
    ~GeoHashGrid() {}
    void GeneratePoints(int nPointNum); // 生成固定数目的圆形

private:
    double mdX0, mdY0;                  // 原点坐标
    double mdWidth, mdHeight;           // 矩形区域的宽和高
    std::vector<GeoHashPoint> mvPoints; // 矩形范围区域内所有圆形
    int mnCircleCount;                  // 矩形范围区域内所有圆形的数目
};