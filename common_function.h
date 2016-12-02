#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H
#include <QTime>
#include <QCoreApplication>

void qSleep(int ms)
{
    QTime* timer=new QTime();
    timer->start();
    while (timer->elapsed()<ms)
    {
        QCoreApplication::processEvents();
    }
    delete timer;
}

#endif // COMMON_FUNCTION_H
