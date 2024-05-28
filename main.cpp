#include "unistd.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ImgProcessor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // 加载动画
    QPixmap pixmap(":/sys/images/loading.gif");
    QSplashScreen splash(pixmap);
    QLabel label(&splash);
    QMovie mv(":/sys/images/loading.gif");
    label.setMovie(&mv);
    mv.start();
    splash.show();
    splash.setCursor(Qt::BlankCursor);
    for(int i=0; i<5000; i+=mv.speed())
    {
        QCoreApplication::processEvents();
        usleep(500*static_cast<useconds_t>(mv.speed()));
    }
    MainWindow w;
    w.setWindowTitle(QObject::tr("图像处理平台"));
    w.show();
    splash.finish(&w);
    return a.exec();
}
