#include "GeoHashPoint.h"

#include <cmath>
#include <bitset>
#include <vector>

const std::string Base32 = "0123456789bcdefghjkmnpqrstuvwxyz";

// 根据预设的二进制位数，将横纵坐标分别转换为对应的二进制，然后依次将 X 和 Y 的二进制交叉拼接，转换成对应的字符串
/*
Hash 方法：每个正方形区域划分为四块，分别用 00、01、10、11 进行二进制编码，其中（X 和 Y）各占用一半的字符，然后按照 X1Y1X2Y2……XnYn 的方式组合
+----+----+----+----+
|0000|0001|         |
+----+----+  01…… +
|0100|0101|         |
+----+----+----+----+
|         |         |
+ 10……  +  11…… +
|         |         |
+----+----+----+----+
*/
bool GeoHashPoint::CalculateGeoHash(int nByteLength)
{
    if (mdX < 0 || mdX >= mdMaxCoord || mdY < 0 || mdY >= mdMaxCoord)
    {
        return false;
    }

    // 将坐标归一化
    double dNormalizedX = mdX / mdMaxCoord;
    double dNormalizedY = mdY / mdMaxCoord;

    // 更新 nByteLength 为最近的 2 的 n 次方
    if (nByteLength <= 1)
        nByteLength = 2;

    // 字符串转换
    std::string vXBits, vYBits;
    vXBits = GetBinarySegment(GetSegmentIndex(dNormalizedX, static_cast<int>(powf(2, static_cast<float>(nByteLength)))), nByteLength);
    vYBits = GetBinarySegment(GetSegmentIndex(dNormalizedY, static_cast<int>(powf(2, static_cast<float>(nByteLength)))), nByteLength);

    // 生成 GeoHash 字符串
    std::string vGeoHash;
    for (int i = 0; i < nByteLength; i++)
    {
        vGeoHash.push_back(vXBits[i]);
        vGeoHash.push_back(vYBits[i]);
    }

    mstrHash = EncodeBase32(vGeoHash);
    return true;
}

std::string GeoHashPoint::GetGeoHash()
{
    return mstrHash;
}

double GeoHashPoint::GetX()
{
    return mdX;
}

double GeoHashPoint::GetY()
{
    return mdY;
}

bool GeoHashPoint::IsInBuffer(std::string strHash)
{
    // GeoHash 字符串长度越长，相对应点的坐标更精细，当两个字符串在前面固定阈值的字符完全一致，则说明它们邻近
    int nSize = static_cast<int>(strHash.size());
    if (nSize < mstrHash.size())
    {
        // 目前转换出的 GeoHash 字符串精度不符合要求，无法进行判断
        return false;
    }
    for (int i = 0; i < nSize; i++)
    {
        if (strHash[i] != mstrHash[i])
            return false;
    }
    return true;
}

std::string GeoHashPoint::GetBinarySegment(int nSegmentIndex, int nByteLength)
{
    std::bitset<32> binary(nSegmentIndex); // 使用 32 位以防溢出
    std::string strBinaryString = binary.to_string();
    // 取最后 nByteLength 位
    return strBinaryString.substr(32 - nByteLength, nByteLength);
}

// 计算段索引的函数
int GeoHashPoint::GetSegmentIndex(double dValue, int nByteLength)
{
    // 计算每段的宽度
    double dSegmentWidth = 1.0 / nByteLength;

    // 计算 dX 所在的段
    int nSegmentIndex = static_cast<int>(dValue / dSegmentWidth); // 向下取整
    if (nSegmentIndex >= nByteLength)
    {
        nSegmentIndex = nByteLength - 1; // 确保索引不超出范围
    }

    return nSegmentIndex; // 返回计算得到的段索引
}

std::string GeoHashPoint::EncodeBase32(const std::string &binaryString)
{
    std::string base32String;
    int nBbuffer = 0;
    int nBitsInBuffer = 0;

    for (char bit : binaryString)
    {
        nBbuffer <<= 1;          // 左移一位
        nBbuffer |= (bit - '0'); // 添加当前位
        nBitsInBuffer++;

        // 每5位生成一个 Base32 字符
        if (nBitsInBuffer == 5)
        {
            base32String.push_back(Base32[nBbuffer]);
            nBbuffer = 0;
            nBitsInBuffer = 0;
        }
    }

    // 处理剩余的位
    if (nBitsInBuffer > 0)
    {
        nBbuffer <<= (5 - nBitsInBuffer); // 左移填充0
        base32String.push_back(Base32[nBbuffer]);
    }

    return base32String;
}

double GeoHashPoint::GetRadius()
{
    return mdRadius;
}

bool GeoHashPoint::GetState()
{
    return mbState;
}

void GeoHashPoint::SetState(bool bState)
{
    mbState = bState;
}
