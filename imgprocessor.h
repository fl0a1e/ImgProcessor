#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <QImage>

class ImgProcessor
{
public:
    ImgProcessor();
    QImage gray(const QImage& inputImage)const;             // 灰度
    QImage EdgeDetection(const QImage &inputImage)const;    // 边缘检测
    QImage gamma(const QImage &inputImage) const;           // gamma
    QImage equalizeHistogram(const QImage &inputImage) const; // 彩色图像直方图均衡
    QImage meanFilter(const QImage &inputImage, int kernelSize) const;   // 均方滤波
    QImage medianFilter(const QImage &inputImage, int kernelSize) const; // 中值滤波
    QImage GaussianFilter(const QImage &inputImage, double sigma) const; // 高斯滤波

    QImage AdjustContrast(const QImage &inputImage, int value) const; //对比度
    QImage AdjustSaturation(const QImage &inputImage, int iSaturateValue) const; //饱和度

    QImage redChannel  (const QImage &inputImage) const;    // 提取红通道
    QImage greenChannel(const QImage &inputImage) const;    // 提取绿通道
    QImage blueChannel (const QImage &inputImage) const;    // 提取蓝通道

private:
    int median(QVector<int> &values) const; // 计算中值

};

#endif // IMGPROCESSOR_H
