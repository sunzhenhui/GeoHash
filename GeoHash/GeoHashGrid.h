#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

// 网格类，用于记录当前显示的正方形区域，并存储所有点的信息
class GeoHashPoint;
class GeoHashLine;
class GeoHashGrid
{
public:
    GeoHashGrid() : mdX0(0), mdY0(0), mdWidth(1000), mdHeight(1000) {}
    GeoHashGrid(double dX0, double dY0, double dWidth, double dHeight) : mdX0(dX0), mdY0(dY0), mdWidth(dWidth), mdHeight(dHeight) {}
    GeoHashGrid(double dX0, double dY0, double dEdgeLength) : mdX0(dX0), mdY0(dY0), mdWidth(dEdgeLength), mdHeight(dEdgeLength) {}
    ~GeoHashGrid();
    void GeneratePoints(int nPointNum); // 生成固定数目的圆形
    void GenerateLine();                // 生成原点位于正方形区域外，但是穿过该区域的射线
    void GenerateGeoHash();             // 生成所有圆形与射线所有节点的 GeoHash 值，用于快速筛选距离少于阈值的圆形

private:
    double mdX0, mdY0;                         // 原点坐标
    double mdWidth, mdHeight;                  // 矩形区域的宽和高
    std::vector<GeoHashPoint> mvPoints;        // 矩形范围区域内所有圆形
    int mnCircleCount;                         // 矩形范围区域内所有圆形的数目
    GeoHashLine *mpLine = nullptr;             // 原点位于正方形区域外，但是穿过该区域的射线
    std::unordered_map<std::string, int> mMap; // 用于存储 GeoHash 与对应节点的键值对，便于快速筛选小于固定阈值的圆形
    std::vector<GeoHashPoint> mvLinePoints;    // 线段上的等间隔点
};