#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include "common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/school_badge.png");
    QSplashScreen screen(pixmap);
    screen.show();
    a.processEvents();
    screen.showMessage(QString("曲线磨床数控系统"));

    common::qSleep(2000);

    MainWindow w;
    w.show();
    screen.finish(&w);

    return a.exec();
}
