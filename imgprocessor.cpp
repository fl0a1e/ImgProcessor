#include "imgprocessor.h"
#include <QImage>
#include <QColor>
#include <qmath.h>

ImgProcessor::ImgProcessor() {}

// 灰度化
QImage ImgProcessor::gray(const QImage &inputImage) const {
    QImage newImage =inputImage.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

    for(int y = 0; y < newImage.height(); y++)
    {
        for(int x = 0; x < newImage.width(); x++)
        {
            oldColor = QColor(inputImage.pixel(x,y));
            int average = oldColor.red() * 0.11 + oldColor.green() * 0.59 + oldColor.blue() * 0.3;
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    return newImage;
}

//边缘检测
QImage ImgProcessor::EdgeDetection(const QImage &inputImage) const {
    QImage newImage =inputImage.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < inputImage.height() - 1; y++)
    {
        for(int x = 0; x < inputImage.width() - 1; x++)
        {
            color0 =   QColor ( inputImage.pixel(x,y));
            color1 =   QColor ( inputImage.pixel(x + 1,y));
            color2 =   QColor ( inputImage.pixel(x,y + 1));
            color3 =   QColor ( inputImage.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            newImage.setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}

// 伽马变换
QImage ImgProcessor::gamma(const QImage &inputImage) const {
    QImage newImage = inputImage;
    double d=1.2;
    QColor color;
    int height = inputImage.height();
    int width = inputImage.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(inputImage.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            newImage.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return newImage;
}

// 彩色图像直方图均衡
QImage ImgProcessor::equalizeHistogram(const QImage &inputImage) const {
    int height = inputImage.height();
    int width = inputImage.width();
    int numPixels = width * height;

    // 分离 R, G, B 通道
    QVector<int> histR(256, 0), histG(256, 0), histB(256, 0);
    QVector<int> cdfR(256, 0), cdfG(256, 0), cdfB(256, 0);
    QVector<int> equalizedR(256, 0), equalizedG(256, 0), equalizedB(256, 0);

    // 计算直方图
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = inputImage.pixelColor(x, y);
            histR[color.red()]++;
            histG[color.green()]++;
            histB[color.blue()]++;
        }
    }

    // 计算累积分布函数 (CDF)
    cdfR[0] = histR[0];
    cdfG[0] = histG[0];
    cdfB[0] = histB[0];

    for (int i = 1; i < 256; ++i) {
        cdfR[i] = cdfR[i - 1] + histR[i];
        cdfG[i] = cdfG[i - 1] + histG[i];
        cdfB[i] = cdfB[i - 1] + histB[i];
    }

    // 均衡化
    for (int i = 0; i < 256; ++i) {
        equalizedR[i] = qRound((cdfR[i] - cdfR[0]) * 255.0 / (numPixels - 1));
        equalizedG[i] = qRound((cdfG[i] - cdfG[0]) * 255.0 / (numPixels - 1));
        equalizedB[i] = qRound((cdfB[i] - cdfB[0]) * 255.0 / (numPixels - 1));
    }

    QImage newImage(inputImage.size(), inputImage.format());
    // 应用均衡化直方图
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = inputImage.pixelColor(x, y);
            int r = equalizedR[color.red()];
            int g = equalizedG[color.green()];
            int b = equalizedB[color.blue()];
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return newImage;
}

// 均值滤波
QImage ImgProcessor::meanFilter(const QImage &inputImage, int kernelSize) const {
    QImage newImage = inputImage;
    int halfKernelSize = kernelSize / 2;

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            int redSum = 0;
            int greenSum = 0;
            int blueSum = 0;
            int count = 0;

            // Iterate through the kernel
            for (int ky = -halfKernelSize; ky <= halfKernelSize; ++ky) {
                for (int kx = -halfKernelSize; kx <= halfKernelSize; ++kx) {
                    int pixelX = std::clamp(x + kx, 0, inputImage.width() - 1);
                    int pixelY = std::clamp(y + ky, 0, inputImage.height() - 1);

                    QColor color = inputImage.pixelColor(pixelX, pixelY);
                    redSum += color.red();
                    greenSum += color.green();
                    blueSum += color.blue();
                    ++count;
                }
            }

            // Calculate the mean color value
            int redMean = redSum / count;
            int greenMean = greenSum / count;
            int blueMean = blueSum / count;

            // Set the new pixel value
            newImage.setPixelColor(x, y, QColor(redMean, greenMean, blueMean));
        }
    }

    return newImage;
}

// Function to get the median value from a list of integers
int ImgProcessor::median(QVector<int> &values) const {
    std::sort(values.begin(), values.end());
    int middle = values.size() / 2;
    return values[middle];
}

// 中值滤波
QImage ImgProcessor::medianFilter(const QImage &inputImage, int kernelSize) const {
    QImage outputImage = inputImage;
    int halfKernelSize = kernelSize / 2;

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            QVector<int> redValues;
            QVector<int> greenValues;
            QVector<int> blueValues;

            // Iterate through the kernel
            for (int ky = -halfKernelSize; ky <= halfKernelSize; ++ky) {
                for (int kx = -halfKernelSize; kx <= halfKernelSize; ++kx) {
                    int pixelX = std::clamp(x + kx, 0, inputImage.width() - 1);
                    int pixelY = std::clamp(y + ky, 0, inputImage.height() - 1);

                    QColor color = inputImage.pixelColor(pixelX, pixelY);
                    redValues.push_back(color.red());
                    greenValues.push_back(color.green());
                    blueValues.push_back(color.blue());
                }
            }

            // Calculate the median color value
            int redMedian = median(redValues);
            int greenMedian = median(greenValues);
            int blueMedian = median(blueValues);

            // Set the new pixel value
            outputImage.setPixelColor(x, y, QColor(redMedian, greenMedian, blueMedian));
        }
    }
    return outputImage;
}

// 高斯滤波
QImage ImgProcessor::GaussianFilter(const QImage &inputImage, double sigma) const {

    // 生成高斯核, 默认 5*5 大小
    int kernelSize = 5;
    QVector<QVector<double>> kernel(kernelSize, QVector<double>(kernelSize, 0));
    double sum = 0.0;
    int halfSize = kernelSize / 2;
    double sigma2 = sigma * sigma;

    for (int y = -halfSize; y <= halfSize; ++y) {
        for (int x = -halfSize; x <= halfSize; ++x) {
            double value = exp(-(x * x + y * y) / (2 * sigma2)) / (2 * M_PI * sigma2);
            kernel[y + halfSize][x + halfSize] = value;
            sum += value;
        }
    }

    // 归一化核
    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            kernel[y][x] /= sum;
        }
    }

    // 应用
    QImage newImage(inputImage.size(), inputImage.format());

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            double sumR = 0, sumG = 0, sumB = 0;

            for (int ky = -halfSize; ky <= halfSize; ++ky) {
                for (int kx = -halfSize; kx <= halfSize; ++kx) {
                    int pixelX = qMin(qMax(x + kx, 0), inputImage.width() - 1);
                    int pixelY = qMin(qMax(y + ky, 0), inputImage.height() - 1);

                    QColor color = inputImage.pixelColor(pixelX, pixelY);
                    double kernelValue = kernel[ky + halfSize][kx + halfSize];

                    sumR += color.red() * kernelValue;
                    sumG += color.green() * kernelValue;
                    sumB += color.blue() * kernelValue;
                }
            }

            newImage.setPixelColor(x, y, QColor(static_cast<int>(sumR), static_cast<int>(sumG), static_cast<int>(sumB)));
        }
    }

    return newImage;
}

//对比度
QImage ImgProcessor::AdjustContrast(const QImage &inputImage, int value) const {
    int pixels = inputImage.width() * inputImage.height();
    QImage newImage = inputImage;
    unsigned int *data = (unsigned int *)newImage.bits();

    int red, green, blue, nRed, nGreen, nBlue;

    if (value > 0 && value < 256)
    {
        float param = 1 / (1 - value / 256.0) - 1;

        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * param;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * param;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * param;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    else
    {
        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * value / 100.0;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * value / 100.0;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * value / 100.0;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }

    return newImage;
}

//饱和度
QImage ImgProcessor::AdjustSaturation(const QImage &inputImage, int iSaturateValue) const {
    int red, green, blue, nRed, nGreen, nBlue;
    int pixels = inputImage.width() * inputImage.height();
    QImage newImage = inputImage;
    unsigned int *data = (unsigned int *)newImage.bits();

    float Increment = iSaturateValue/100.0;

    float delta = 0;
    float minVal, maxVal;
    float L, S;
    float alpha;

    for (int i = 0; i < pixels; ++i)
    {
        nRed = qRed(data[i]);
        nGreen = qGreen(data[i]);
        nBlue = qBlue(data[i]);

        minVal = qMin(qMin(nRed, nGreen), nBlue);
        maxVal = qMax(qMax(nRed, nGreen), nBlue);
        delta = (maxVal - minVal) / 255.0;
        L = 0.5*(maxVal + minVal) / 255.0;
        S = qMax(0.5*delta / L, 0.5*delta / (1 - L));

        if (Increment > 0)
        {
            alpha = qMax(S, 1 - Increment);
            alpha = 1.0 / alpha - 1;
            red = nRed + (nRed - L*255.0)*alpha;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - L*255.0)*alpha;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - L*255.0)*alpha;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }
        else
        {
            alpha = Increment;
            red = L*255.0 + (nRed - L * 255.0)*(1+alpha);
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = L*255.0 + (nGreen - L * 255.0)*(1+alpha);
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = L*255.0 + (nBlue - L * 255.0)*(1+alpha);
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }

        data[i] = qRgba(red, green, blue, qAlpha(data[i]));
    }

    return newImage;
}

// 提取红通道
QImage ImgProcessor::redChannel(const QImage &inputImage) const {
    QImage redChannelImage(inputImage.size(), QImage::Format_RGB32);

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            QColor color = inputImage.pixelColor(x, y);
            int r = color.red();
            redChannelImage.setPixelColor(x, y, QColor(r, 0, 0));
        }
    }

    return redChannelImage;
}


// 提取绿通道
QImage ImgProcessor::greenChannel(const QImage &inputImage) const {
    QImage greenChannelImage(inputImage.size(), QImage::Format_RGB32);

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            QColor color = inputImage.pixelColor(x, y);
            int g = color.green();
            greenChannelImage.setPixelColor(x, y, QColor(0, g, 0));
        }
    }

    return greenChannelImage;
}

// 提取蓝通道
QImage ImgProcessor::blueChannel(const QImage &inputImage) const {
    QImage blueChannelImage(inputImage.size(), QImage::Format_RGB32);

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            QColor color = inputImage.pixelColor(x, y);
            int b = color.blue();
            blueChannelImage.setPixelColor(x, y, QColor(0, 0, b));
        }
    }

    return blueChannelImage;
}
