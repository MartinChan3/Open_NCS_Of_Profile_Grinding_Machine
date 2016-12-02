#ifndef THREAD_TRIO_H
#define THREAD_TRIO_H

#include <QObject>
#include "trioactivex.h"
#include "common.h"

class thread_Trio : public QObject
{
    Q_OBJECT
private:
    TrioPCLib::TrioPC *trio;
    void qSleep(int ms);
public:
    explicit thread_Trio(QObject *parent = 0);
    ~thread_Trio();
signals:
    void return_error_of_trio(int,QString,QString,QString);
    void return_axis_paras(double*);
public slots:
    void connect_Trio(bool*);
    void send_txt_to_Trio(bool*,const QString,const QString);
    void run_program_of_Trio(bool*,QString);
    void grab_axis_paras(bool*,double*);

};

#endif // THREAD_TRIO_H
