
#include <iostream>

#include "GeoHashPoint.h"
#include "GeoHashGrid.h"
#include "GeoHashLine.h"

#include <corecrt_math_defines.h>

int main()
{
    // 生成 GeoHash 编码
    /*
    GeoHashPoint point(100, 200, 5, 1000);
    point.CalculateGeoHash(6);
    std::cout << "Point Coordinates: (" << point.GetX() << ", " << point.GetY() << ")-->" << point.GetGeoHash() << std::endl;
    */
   
    // 生成正方形区域范围
    GeoHashGrid grid(0, 0, 1000);
    // 生成随机点于区域范围内
    grid.GeneratePoints(500);
    // 生成随机穿过区域的射线
    grid.GenerateLine();
    // 获取射线的间隔点
    grid.GetLine()->GenerateIntervalPoints(1, 0, 0, 1000, 1000);
    // 查找所有射线穿过的圆形
    grid.FindLineCrossCircle();
    // 输出所有符合要求的圆形
    grid.OutputCircles();   

    std::cout << "End!" << std::endl;
    return 0;
}