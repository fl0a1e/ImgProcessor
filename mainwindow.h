#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QImage>
#include <QMessageBox>
#include "imgprocessor.h"
#include "cropper.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage ImageCenter(QImage qimage, QLabel *qLabel);//调整图片比例
    QStringList srcDirPathList;//图片list
    int index =0; //图片index

private slots:
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_flower_triggered();
    void on_action_lena_triggered();
    void on_action_cameraman_triggered();
    void on_action_SP_triggered();
    void on_pushButton_select_clicked();
    void on_pushButton_gray_clicked();
    void on_pushButton_gamma_clicked();
    void on_pushButton_turnleft_clicked();
    void on_pushButton_turnright_clicked();
    void on_pushButton_mirrored_clicked();
    void on_pushButton_edge_detection_clicked();
    void on_horizontalSlider_Contrast_valueChanged(int value);
    void on_horizontalSlider_light_valueChanged(int value);
    void on_horizontalSlider_Saturation_valueChanged(int value);
    void on_pushButton_save_clicked();
    void on_pushButton_origin_clicked();
    void on_pushButton_equalizeHist_clicked();
    void on_horizontalSlider_gaussianFilter_valueChanged(int value);
    void on_pushButton_cropper_clicked();
    void cropImgShow();
    void on_pushButton_meanFilter_clicked();
    void on_pushButton_medianFilter_clicked();
    void on_checkBox_R_clicked();
    void on_checkBox_G_clicked();
    void on_checkBox_B_clicked();
    void on_action_ElegantDark_triggered();
    void on_action_Dark_triggered();
    void on_action_Ubuntu_triggered();

private:
    Ui::MainWindow *ui;
    QString origin_path;//原图路径
    QImage origin_img; //原图
    QImage cur_img;
    QMap<QString , int > slider_save; // 防止滑块反复触发保存当前图片
    ImgProcessor* imgProcessor; // 图像处理核心逻辑
    ImageCropperLabel* imgCropperLabel; // 裁剪模块
    bool cropper;
    QImage R_img;
    QImage G_img;
    QImage B_img;
};
#endif // MAINWINDOW_H
