#include "common.h"
#include <QTime>
#include <QCoreApplication>
#include "qt_windows.h"
common::common(QObject *parent) : QObject(parent)
{

}

void common::qSleep(const int ms)
{
        QTime timer;
        timer.start();
        while (timer.elapsed()<ms)
        {
            QCoreApplication::processEvents();
        }
}
