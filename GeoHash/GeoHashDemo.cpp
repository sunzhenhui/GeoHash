
#include <iostream>

#include "GeoHashPoint.h"
#include "GeoHashGrid.h"
#include "GeoHashLine.h"

#define M_PI 3.1415926535

int main()
{
    GeoHashGrid grid(0, 0, 1000);
    grid.GeneratePoints(500);
    GeoHashLine line(-1, -1, M_PI / 4);
    line.GenerateIntervalPoints(5, 0, 0, 1000, 1000);

    return 0;
    GeoHashPoint point(100, 200, 5, 1000);
    point.CalculateGeoHash(6);
    std::cout << "Point Coordinates: (" << point.GetX() << ", " << point.GetY() << ")-->" << point.GetGeoHash() << std::endl;
    point = GeoHashPoint(300, 200, 5, 1000);
    point.CalculateGeoHash(3);
    std::cout << "Point Coordinates: (" << point.GetX() << ", " << point.GetY() << ")-->" << point.GetGeoHash() << std::endl;
    point = GeoHashPoint(890, 530, 5, 1000);
    point.CalculateGeoHash(7);
    std::cout << "Point Coordinates: (" << point.GetX() << ", " << point.GetY() << ")-->" << point.GetGeoHash() << std::endl;
    point.CalculateGeoHash(5);
    std::cout << "Point Coordinates: (" << point.GetX() << ", " << point.GetY() << ")-->" << point.GetGeoHash() << std::endl;
}
