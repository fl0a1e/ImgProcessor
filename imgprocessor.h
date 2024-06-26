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
    QImage Sharpen(const QImage &inputImage) const; // 锐化

    QImage AdjustContrast(const QImage &inputImage, int value) const; //对比度
    QImage AdjustSaturation(const QImage &inputImage, int iSaturateValue) const; //饱和度

    QImage redChannel  (const QImage &inputImage) const;    // 提取红通道
    QImage greenChannel(const QImage &inputImage) const;    // 提取绿通道
    QImage blueChannel (const QImage &inputImage) const;    // 提取蓝通道

    QImage warm(const QImage &inputImage) const; // 暖色
    QImage cool(const QImage &inputImage) const; // 冷色
    QImage InverseColor(const QImage &inputImage) const; // 反色
    QImage old(const QImage &inputImage) const; // 老照片
    QImage PencilSketchFilter(const QImage &inputImage) const; // 铅笔画
    QImage MosaicFilter(const QImage &image, int blockSize) const; // 马赛克
    QImage MeltFilter(const QImage &image, int meltIntensity) const; // 融化
    QImage FreezeFilter(const QImage &image) const; // 冰冻

private:

};

#endif // IMGPROCESSOR_H
