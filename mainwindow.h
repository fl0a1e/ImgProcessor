#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QImage>
#include <QMessageBox>

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

    QImage ImageCenter(QImage  qimage,QLabel *qLabel);//调整图片比例
    QImage gray(QImage image);//灰度化
    QImage AdjustContrast(QImage image, int value);//调整对比度
    QImage AdjustSaturation(QImage image, int value);//调整饱和度
    QImage EdgeDetection(QImage image);//边缘检测
    QStringList srcDirPathList;//图片list
    int index =0; //图片index

private slots:
    void initDockSize();
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_pushButton_select_clicked();
    void on_pushButton_gray_clicked();
    void on_pushButton_edge_detection_clicked();
    void on_horizontalSlider_Contrast_valueChanged(int value);
    void on_horizontalSlider_light_valueChanged(int value);
    void on_horizontalSlider_Saturation_valueChanged(int value);
    void on_pushButton_save_clicked();
    void on_pushButton_origin_clicked();

private:
    Ui::MainWindow *ui;
    QString origin_path;//原图
};
#endif // MAINWINDOW_H
