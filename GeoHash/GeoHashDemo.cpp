
#include <iostream>

#include "GeoHashPoint.h"

int main()
{
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

