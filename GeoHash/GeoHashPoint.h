#pragma once

#include <iostream>
#include <string>

// 圆形类，用于记录二维区域内的点相关信息
class GeoHashPoint
{
public:
    GeoHashPoint() : mdX(0.0), mdY(0.0), mdRadius(5.0), mdMaxCoord(1000.0) {}
    GeoHashPoint(double dX, double dY) : mdX(dX), mdY(dY), mdRadius(5.0), mdMaxCoord(1000.0) {}
    GeoHashPoint(double dX, double dY, double dRadius, double dMaxCoord) : mdX(dX), mdY(dY), mdRadius(dRadius), mdMaxCoord(dMaxCoord) {}
    ~GeoHashPoint() {}
    bool CalculateGeoHash(int nByteLength); // 获取 GeoHash 字段值
    std::string GetGeoHash();               // 获取哈希值
    double GetX();                          // 获取坐标值 X
    double GetY();                          // 获取坐标值 Y
    bool IsInBuffer(std::string strHash);   // 比较两个哈希值是否一致，判断点是否与目标点临近
    double GetRadius();                     // 获取圆形的半径
    bool GetState();                        // 获取点的状态
    void SetState(bool bState);             // 设置当前点的状态

private:
    std::string GetBinarySegment(int nSegmentIndex, int nByteLength); // 转换为二进制
    int GetSegmentIndex(double dX, int nByteLength);                  // 获取分段索引，判断当前位于第几段
    std::string EncodeBase32(const std::string &binaryString);        // GeoHash 编码

    double mdX = 0.0;
    double mdY = 0.0;
    double mdRadius = 5.0;      // 圆的半径
    std::string mstrHash;       // GeoHash 字段值，用于快速判断点与点之间的距离
    double mdMaxCoord = 1000.0; // 正方形区域坐标范围
    bool mbState = false;       // 标识点是否已经被选中
};