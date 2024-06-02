#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cropper.h"
#include <QToolButton>
#include <QApplication>
#include <QSpinBox>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QLabel>
using namespace std;
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QPaintDevice>
#include <QPainter>
#include <QImage>
#include <QtCore/qmath.h>
#include <QTimer>

#define QMESSAGE_BOX_NO_IMG QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
#define QMESSAGE_BOX_CROPPING if(cropper){QMessageBox::warning(nullptr, "提示", "请先完成裁剪！", QMessageBox::Yes |  QMessageBox::Yes);return;};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , imgCropperLabel(nullptr)
    , cropper(false)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

    // 初始化滑块参数
    slider_save["gaussian"] = 0;
    slider_save["light"] = 0;
    slider_save["contrast"] = 0;
    slider_save["saturation"] = 0;


}

MainWindow::~MainWindow()
{
    delete ui;
    delete imgCropperLabel;
}

//图片居中显示,图片大小与label大小相适应
QImage MainWindow::ImageCenter(QImage qimage,QLabel *qLabel)
{
    QImage image;
    QSize imageSize = qimage.size();
    QSize labelSize = qLabel->size();

    double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0*imageSize.height() / labelSize.height();
    if (dWidthRatio>dHeightRatio)
    {
        image = qimage.scaledToWidth(labelSize.width());
    }
    else
    {
        image = qimage.scaledToHeight(labelSize.height());
    }
    return image;
}


void MainWindow::on_action_open_triggered() {
    QMESSAGE_BOX_CROPPING
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "C:", tr("图像文件(*.jpg *.png *.bmp)"));
    if(srcDirPathListS.size()>0)
    {
        ui->tabWidget->setCurrentIndex(0);
    }
    if(srcDirPathListS.size()==1){
        srcDirPathList =srcDirPathListS;
        srcDirPathListS.clear();
        index =0;
        QString srcDirPath = srcDirPathList.at(index);
        QImage image(srcDirPath);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        origin_path=srcDirPath;

        ui->statusbar->showMessage(srcDirPath);
    }
}

void MainWindow::on_action_save_triggered() {
    QMESSAGE_BOX_CROPPING
    if(!ui->label_show->pixmap().isNull()){
        QString filename = QFileDialog::getSaveFileName(this,tr("保存图片"),"C:",tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
        if (filename.isEmpty()) {
            return;
        }
        else{
            if (!(ui->label_show->pixmap().toImage().save(filename))) //保存图像
            {
                QMessageBox::information(this,tr("图片保存成功！"),tr("图片保存失败！"));
                return;
            }
            ui->statusbar->showMessage("图片保存成功！");
        }
    }else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 快速开始
void MainWindow::on_action_flower_triggered() {
    QMESSAGE_BOX_CROPPING
    QString srcDirPath = ":/sys/images/flower.jpg";
    QImage image(srcDirPath);
    QImage Image=ImageCenter(image,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
    origin_path=srcDirPath;

    ui->statusbar->showMessage(srcDirPath);
}

void MainWindow::on_action_lena_triggered() {
    QMESSAGE_BOX_CROPPING
    QString srcDirPath = ":/sys/images/lena.png";
    QImage image(srcDirPath);
    QImage Image=ImageCenter(image,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
    origin_path=srcDirPath;

    ui->statusbar->showMessage(srcDirPath);
}

void MainWindow::on_action_cameraman_triggered() {
    QMESSAGE_BOX_CROPPING
    QString srcDirPath = ":/sys/images/cameraman.jpg";
    QImage image(srcDirPath);
    QImage Image=ImageCenter(image,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
    origin_path=srcDirPath;

    ui->statusbar->showMessage(srcDirPath);
}

//选择图片
void MainWindow::on_pushButton_select_clicked() {
    QMESSAGE_BOX_CROPPING
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "C:", tr("图像文件(*.jpg *.png *.bmp *.tif)"));
    if(srcDirPathListS.size()>0)
    {
        ui->tabWidget->setCurrentIndex(0);
    }
    if(srcDirPathListS.size()==1){
        srcDirPathList =srcDirPathListS;
        srcDirPathListS.clear();
        index =0;
        QString srcDirPath = srcDirPathList.at(index);
        QImage image(srcDirPath);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        origin_path=srcDirPath;

        ui->statusbar->showMessage(srcDirPath);
    }
}


//保存
void MainWindow::on_pushButton_save_clicked() {
    QMESSAGE_BOX_CROPPING
    if(!ui->label_show->pixmap().isNull()){
        QString filename = QFileDialog::getSaveFileName(this,tr("保存图片"),"C:",tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
        if (filename.isEmpty()) {
                return;
        }
        else{
            if (!(ui->label_show->pixmap().toImage().save(filename))) //保存图像
            {
                    QMessageBox::information(this,tr("图片保存成功！"),tr("图片保存失败！"));
                    return;
            }
            ui->statusbar->showMessage("图片保存成功！");
        }
    }else{
        QMESSAGE_BOX_NO_IMG
    }
}

//显示原图按钮
void MainWindow::on_pushButton_origin_clicked()
{
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        // ==== 先重置所有参数 ====
        ui->horizontalSlider_gaussianFilter->setValue(0);
        ui->horizontalSlider_light->setValue(0);
        ui->horizontalSlider_Contrast->setValue(0);
        ui->horizontalSlider_Saturation->setValue(0);
        slider_save["gaussian"] = 0;
        slider_save["light"] = 0;
        slider_save["contrast"] = 0;
        slider_save["saturation"] = 0;
        ui->checkBox_R->setChecked(true);
        ui->checkBox_G->setChecked(true);
        ui->checkBox_B->setChecked(true);
        // =========================================================
        // =========================================================

        // 直接读取原图
        QImage image(origin_path);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);

        // 重置 cur_img
        cur_img = ui->label_show->pixmap().toImage();

    }else{
        QMESSAGE_BOX_NO_IMG
    }
}


// 灰度
QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

    for(int y = 0; y < newImage.height(); y++)
    {
        for(int x = 0; x < newImage.width(); x++)
        {
            oldColor = QColor(image.pixel(x,y));
            int average = oldColor.red() * 0.11 + oldColor.green() * 0.59 + oldColor.blue() * 0.3;
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    return newImage;
}

// 灰度
void MainWindow::on_pushButton_gray_clicked()
{
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=gray(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 水平镜像
void MainWindow::on_pushButton_mirrored_clicked()
{
    QMESSAGE_BOX_CROPPING
    if(!ui->label_show->pixmap().isNull()){
        QImage images(ui->label_show->pixmap().toImage());
        images = images.mirrored(true, false);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
        //cur_img = ui->label_show->pixmap().toImage();
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 顺时针旋转
void MainWindow::on_pushButton_turnright_clicked() {
    QMESSAGE_BOX_CROPPING
    if(!ui->label_show->pixmap().isNull()){
        QImage images(ui->label_show->pixmap().toImage());
        QTransform matrix;
        matrix.rotate(90.0);//顺时针旋转90度
        images= images.transformed(matrix,Qt::FastTransformation);
        //QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 逆时针旋转
void MainWindow::on_pushButton_turnleft_clicked() {
    QMESSAGE_BOX_CROPPING
    if(!ui->label_show->pixmap().isNull()){
        QImage images(ui->label_show->pixmap().toImage());
        QTransform matrix;
        matrix.rotate(-90.0);//逆时针旋转90度
        images= images.transformed(matrix,Qt::FastTransformation);
        //QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

//边缘检测
QImage MainWindow::EdgeDetection(QImage image) {
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
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
    for( int y = 0; y < image.height() - 1; y++)
    {
        for(int x = 0; x < image.width() - 1; x++)
        {
            color0 =   QColor ( image.pixel(x,y));
            color1 =   QColor ( image.pixel(x + 1,y));
            color2 =   QColor ( image.pixel(x,y + 1));
            color3 =   QColor ( image.pixel(x + 1,y + 1));
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

//边缘检测
void MainWindow::on_pushButton_edge_detection_clicked()
{
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage newImage = EdgeDetection(image);
        QImage Image=ImageCenter(newImage,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 伽马变换
QImage MainWindow::gamma(QImage image) {
    double d=1.2;
    QColor color;
    int height = image.height();
    int width = image.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(image.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            image.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return image;
}

// 伽马变换
void MainWindow::on_pushButton_gamma_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage images=gamma(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 亮度滑块
void MainWindow::on_horizontalSlider_light_valueChanged(int value) {
    QMESSAGE_BOX_CROPPING
    if(!value){return;} // 加速重置
    if(origin_path!=nullptr){
        if(slider_save["light"] == 0){
            cur_img = ui->label_show->pixmap().toImage();
        }
        QImage image(cur_img);
        int red, green, blue;
        int pixels = image.width() * image.height();
        unsigned int *data = (unsigned int *)image.bits();

        for (int i = 0; i < pixels; ++i)
        {
            red= qRed(data[i])+ value;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green= qGreen(data[i]) + value;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue= qBlue(data[i]) + value;
            blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["light"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

//对比度
QImage MainWindow::AdjustContrast(QImage image, int value)
{
    int pixels = image.width() * image.height();
    unsigned int *data = (unsigned int *)image.bits();

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

    return image;
}

//对比度滑块
void MainWindow::on_horizontalSlider_Contrast_valueChanged(int value) {
    QMESSAGE_BOX_CROPPING
    if(!value){return;} // 加速重置
    if(origin_path!=nullptr){
        if(slider_save["contrast"] == 0){
            cur_img = ui->label_show->pixmap().toImage();
        }
        QImage image(cur_img);
        QImage images=AdjustContrast(image,value);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["contrast"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

//饱和度
QImage MainWindow::AdjustSaturation(QImage Img, int iSaturateValue)
{
    int red, green, blue, nRed, nGreen, nBlue;
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

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

        minVal = std::min(std::min(nRed, nGreen), nBlue);
        maxVal = std::max(std::max(nRed, nGreen), nBlue);
        delta = (maxVal - minVal) / 255.0;
        L = 0.5*(maxVal + minVal) / 255.0;
        S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

        if (Increment > 0)
        {
            alpha = std::max(S, 1 - Increment);
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

    return Img;
}


//饱和度滑块
void MainWindow::on_horizontalSlider_Saturation_valueChanged(int value) {
    QMESSAGE_BOX_CROPPING
    if(!value){return;} // 加速重置
    if(origin_path!=nullptr){
        if(slider_save["saturation"] == 0){
            cur_img = ui->label_show->pixmap().toImage();
        }
        QImage image(cur_img);
        QImage images=AdjustSaturation(image,value);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["saturation"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 彩色图像直方图均衡
QImage MainWindow::equalizeHistogram(QImage image) {
    int height = image.height();
    int width = image.width();
    int numPixels = width * height;

    // 分离 R, G, B 通道
    QVector<int> histR(256, 0), histG(256, 0), histB(256, 0);
    QVector<int> cdfR(256, 0), cdfG(256, 0), cdfB(256, 0);
    QVector<int> equalizedR(256, 0), equalizedG(256, 0), equalizedB(256, 0);

    // 计算直方图
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = image.pixelColor(x, y);
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

    QImage newImage(image.size(), image.format());
    // 应用均衡化直方图
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = image.pixelColor(x, y);
            int r = equalizedR[color.red()];
            int g = equalizedG[color.green()];
            int b = equalizedB[color.blue()];
            newImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return newImage;
}

void MainWindow::on_pushButton_equalizeHist_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage newImage = equalizeHistogram(image);
        QImage Image=ImageCenter(newImage,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 高斯滤波
QImage MainWindow::GaussianFilter(QImage image, double sigma) {

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
    QImage newImage(image.size(), image.format());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            double sumR = 0, sumG = 0, sumB = 0;

            for (int ky = -halfSize; ky <= halfSize; ++ky) {
                for (int kx = -halfSize; kx <= halfSize; ++kx) {
                    int pixelX = qMin(qMax(x + kx, 0), image.width() - 1);
                    int pixelY = qMin(qMax(y + ky, 0), image.height() - 1);

                    QColor color = image.pixelColor(pixelX, pixelY);
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

// 高斯模糊滑块
void MainWindow::on_horizontalSlider_gaussianFilter_valueChanged(int value) {
    QMESSAGE_BOX_CROPPING
    if(!value){return;} // 加速重置

    if(origin_path!=nullptr){
        if(slider_save["gaussian"] == 0){
            cur_img = ui->label_show->pixmap().toImage();
        }
        QImage image(cur_img);
        QImage images=GaussianFilter(image, static_cast<double>(value * 20.0 / 100.0));
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["gaussian"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 裁剪模块
void MainWindow::on_pushButton_cropper_clicked() {
    if(origin_path!=nullptr){
        if(!cropper) {
            // 创建控件，并设置固定大小
            QPixmap crop_img = ui->label_show->pixmap();
            imgCropperLabel = new ImageCropperLabel(200, 200, ui->label_show);
            connect(imgCropperLabel,SIGNAL(croppedImageChanged()),this,SLOT(cropImgShow())); // 信号绑定，接受裁剪框变化信号
            // 设置（圆形）裁剪器
            imgCropperLabel->setRectCropper();
            // 设置原始图像
            imgCropperLabel->setOriginalImage(crop_img);
            // 设置输出图像的形状
            // OutputShape::RECT     --> 矩形（正方形）
            // OutputShape::ELLIPSE  --> 椭圆形（圆形）
            imgCropperLabel->setOutputShape(OutputShape::RECT);
            // 启用不透明效果，裁剪区域外不透明显示（默认启用）
            imgCropperLabel->enableOpacity(true);
            // 设置不透明度(0~1的浮点数)
            imgCropperLabel->setOpacity(0.5);  // 默认: 0.6
            // 显示四个角(四条边)上的矩形方块，用于捕获鼠标，调整裁剪器的大小（默认显示）
            imgCropperLabel->setShowDragSquare(true);
            // 设置四个角(四条边)上的矩形方块的大小，颜色
            imgCropperLabel->setDragSquareEdge(8);	// 默认: 8
            imgCropperLabel->setDragSquareColor(Qt::green);  // 默认: Qt::white

            imgCropperLabel->show();

            cropper = true;
        }else{
            QImage Image=ImageCenter(ui->label_show->pixmap().toImage(),ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            imgCropperLabel->close();
            delete imgCropperLabel;
            imgCropperLabel = nullptr;
            cropper = false;
        }
    }
    else {
        QMESSAGE_BOX_NO_IMG
    }
}

// SIGNAL：void croppedImageChagned()
// 用户调整裁剪区域时，会触发 croppedImageChanged() 信号
// 调用 getCroppedImage() 可以获取裁剪区域的图像
void MainWindow::cropImgShow() {
    QPixmap resultImage = imgCropperLabel->getCroppedImage(/*OutputShape::RECT*/);
    // 显示
    //QImage Image=ImageCenter(resultImage.toImage(), ui->label_show);
    ui->label_show->setPixmap(resultImage);
    ui->label_show->setAlignment(Qt::AlignCenter);
}

// 均值滤波
QImage MainWindow::meanFilter(const QImage &inputImage, int kernelSize) {
    QImage outputImage = inputImage;
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
                    int pixelX = clamp(x + kx, 0, inputImage.width() - 1);
                    int pixelY = clamp(y + ky, 0, inputImage.height() - 1);

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
            outputImage.setPixelColor(x, y, QColor(redMean, greenMean, blueMean));
        }
    }

    return outputImage;
}

// 均值滤波
void MainWindow::on_pushButton_meanFilter_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage images=meanFilter(image, 3);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// Function to get the median value from a list of integers
int MainWindow::median(QVector<int> &values) {
    std::sort(values.begin(), values.end());
    int middle = values.size() / 2;
    return values[middle];
}

// 中值滤波
QImage MainWindow::medianFilter(const QImage &inputImage, int kernelSize) {
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
                    int pixelX = clamp(x + kx, 0, inputImage.width() - 1);
                    int pixelY = clamp(y + ky, 0, inputImage.height() - 1);

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

// 中值滤波
void MainWindow::on_pushButton_medianFilter_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage images=medianFilter(image, 3);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 红通道
QImage MainWindow::redChannel(const QImage &inputImage) {
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

void MainWindow::on_checkBox_R_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        if(ui->checkBox_R->isChecked()) {
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int r = R_img.pixelColor(x, y).red();
                    int g = color.green();
                    int b = color.blue();
                    image.setPixelColor(x, y, QColor(r, g, b));
                }
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        } else if(!ui->checkBox_R->isChecked()) {
            R_img = redChannel(image);
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int g = color.green();
                    int b = color.blue();
                    image.setPixelColor(x, y, QColor(0, g, b));
                }
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        }
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 绿通道
QImage MainWindow::greenChannel(const QImage &inputImage) {
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

void MainWindow::on_checkBox_G_clicked() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        if(ui->checkBox_G->isChecked()) {
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int r = color.red();
                    int g = G_img.pixelColor(x, y).green();
                    int b = color.blue();
                    image.setPixelColor(x, y, QColor(r, g, b));
                }
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        } else if(!ui->checkBox_G->isChecked()) {
            G_img = greenChannel(image);
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int r = color.red();
                    int b = color.blue();
                    image.setPixelColor(x, y, QColor(r, 0, b));
                }
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        }
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// blue通道
QImage MainWindow::blueChannel(const QImage &inputImage) {
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

void MainWindow::on_checkBox_B_clicked() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        if(ui->checkBox_B->isChecked()) {
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int r = color.red();
                    int g = color.green();
                    int b = B_img.pixelColor(x, y).blue();
                    image.setPixelColor(x, y, QColor(r, g, b));
                }
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        } else if(!ui->checkBox_B->isChecked()) {
            B_img = blueChannel(image);
            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor color = image.pixelColor(x, y);
                    int r = color.red();
                    int g = color.green();
                    image.setPixelColor(x, y, QColor(r, g, 0));
                }
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        }
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}
