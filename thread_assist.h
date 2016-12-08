#ifndef THREAD_ASSIST_H
#define THREAD_ASSIST_H

#include <QObject>
#include "common.h"

class thread_assist : public QObject
{
    Q_OBJECT
private:
    void qSleep(const int ms);
    bool start_time_sending_loop;
public:
    explicit thread_assist(QObject *parent = 0);

signals:
    void return_current_time_str(QString*);
public slots:
   void send_current_Time();
   void receive_time_loop_stop_flag();
};

#endif // THREAD_ASSIST_H
