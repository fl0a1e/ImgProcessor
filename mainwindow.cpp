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

    imgProcessor = new ImgProcessor();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imgProcessor;
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

void MainWindow::on_action_SP_triggered() {
    QMESSAGE_BOX_CROPPING
        QString srcDirPath = ":/sys/images/Salt-and-pepper.png";
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
void MainWindow::on_pushButton_gray_clicked()
{
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->gray(image);

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

        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

//边缘检测
void MainWindow::on_pushButton_edge_detection_clicked()
{
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage newImage = imgProcessor->EdgeDetection(image);

        QImage Image=ImageCenter(newImage,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 伽马变换
void MainWindow::on_pushButton_gamma_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage images=imgProcessor->gamma(image);

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

//对比度滑块
void MainWindow::on_horizontalSlider_Contrast_valueChanged(int value) {
    QMESSAGE_BOX_CROPPING
    if(!value){return;} // 加速重置
    if(origin_path!=nullptr){
        if(slider_save["contrast"] == 0){
            cur_img = ui->label_show->pixmap().toImage();
        }
        QImage image(cur_img);
        QImage images=imgProcessor->AdjustContrast(image,value);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["contrast"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
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
        QImage images=imgProcessor->AdjustSaturation(image,value);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["saturation"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}


// 直方均衡
void MainWindow::on_pushButton_equalizeHist_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage newImage = imgProcessor->equalizeHistogram(image);

        QImage Image=ImageCenter(newImage,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
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
        QImage images=imgProcessor->GaussianFilter(image, static_cast<double>(value * 20.0 / 100.0));

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        slider_save["gaussian"] = value;
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 锐化
void MainWindow::on_pushButton_sharp_clicked()
{
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->Sharpen(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
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
void MainWindow::on_pushButton_meanFilter_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage images=imgProcessor->meanFilter(image, 3);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 中值滤波
void MainWindow::on_pushButton_medianFilter_clicked() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());
        QImage images=imgProcessor->medianFilter(image, 3);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

// 红通道
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
            R_img = imgProcessor->redChannel(image);
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
            G_img = imgProcessor->greenChannel(image);
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
            B_img = imgProcessor->blueChannel(image);
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


void MainWindow::on_action_ElegantDark_triggered() {
    QFile file(":/sys/ElegantDark.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());//读取样式表文件
        this->setStyleSheet(styleSheet);//把文件内容传参
        file.close();
    }
    else
    {
        QMessageBox::information(this,"tip","cannot find qss file");
    }
}

void MainWindow::on_action_Dark_triggered() {
    QFile file(":/sys/StyleSheet.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());//读取样式表文件
        this->setStyleSheet(styleSheet);//把文件内容传参
        file.close();
    }
    else
    {
        QMessageBox::information(this,"tip","cannot find qss file");
    }
}

void MainWindow::on_action_Ubuntu_triggered() {
    QFile file(":/sys/ubuntu.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());//读取样式表文件
        this->setStyleSheet(styleSheet);//把文件内容传参
        file.close();
    }
    else
    {
        QMessageBox::information(this,"tip","cannot find qss file");
    }
}

void MainWindow::on_action_warm_triggered() {
    QMESSAGE_BOX_CROPPING
    if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->warm(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

void MainWindow::on_action_cold_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->cool(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

void MainWindow::on_action_T_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->InverseColor(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

void MainWindow::on_action_old_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->old(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

void MainWindow::on_action_PencilSketchFilter_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->PencilSketchFilter(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

void MainWindow::on_action_MosaicFilter_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->MosaicFilter(image, 10);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}


void MainWindow::on_action_MeltFilter_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->MeltFilter(image, 10);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}

void MainWindow::on_action_FreezeFilter_triggered() {
    QMESSAGE_BOX_CROPPING
        if(origin_path!=nullptr){
        QImage image(ui->label_show->pixmap().toImage());

        QImage images=imgProcessor->FreezeFilter(image);

        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMESSAGE_BOX_NO_IMG
    }
}
