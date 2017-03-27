#include "thread_assist.h"
#include "QDateTime"
void thread_assist::qSleep(const int ms)
{
    common::qSleep(ms);
}

thread_assist::thread_assist(QObject *parent) : QObject(parent)
{
    start_time_sending_loop=true;
}

void thread_assist::send_current_Time()
{
    while(start_time_sending_loop)
    {
        QString* str=new QString();
        QDateTime current_one=QDateTime::currentDateTime();
        *str=current_one.toString(QString("yyyy MM dd hh:mm:ss"));

//        emit return_current_time_str(str);
        qSleep(1000);
    }
}

void thread_assist::receive_time_loop_stop_flag()
{
    start_time_sending_loop=false;
}

