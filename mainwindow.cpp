#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::on_action_Open_triggered() {
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

        //状态栏显示图片路径
        //QLabel *label=ui->statusBar->findChild<QLabel *>("status");
        ui->statusbar->showMessage(srcDirPath);
    }
}

//选择图片
void MainWindow::on_pushButton_select_clicked() {
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "E:/Qt/qtworks/MainWindow/images", tr("图像文件(*.jpg *.png *.bmp)"));
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

        //状态栏显示图片路径
        //QLabel *label=ui->statusBar->findChild<QLabel *>("status");
        ui->statusbar->showMessage(srcDirPath);
    }
}


//保存
void MainWindow::on_pushButton_save_clicked() {
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
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//显示原图按钮
void MainWindow::on_pushButton_origin_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}


//灰度化
QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

    for(int y = 0; y < newImage.height(); y++)
    {
        for(int x = 0; x < newImage.width(); x++)
        {
            oldColor = QColor(image.pixel(x,y));
            int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    return newImage;
}

//灰度化
void MainWindow::on_pushButton_gray_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);

        QImage images=gray(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
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
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage newImage = EdgeDetection(image);
        QImage Image=ImageCenter(newImage,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//亮度滑块
void MainWindow::on_horizontalSlider_light_valueChanged(int value) {
    if(origin_path!=nullptr){
        QImage image(origin_path);
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
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
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
void MainWindow::on_horizontalSlider_Contrast_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage images=AdjustContrast(image,value);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
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
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage images=AdjustSaturation(image,value);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}