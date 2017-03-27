#include "thread_trio.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#define IP_Simulator "127.0.0.1"
#define IP_MC664 "192.168.0.250"

void thread_Trio::qSleep(int ms)
{
    common::qSleep(ms);
}

thread_Trio::thread_Trio(QObject *parent) : QObject(parent)
{
    trio=new TrioPCLib::TrioPC();

    connect(trio, SIGNAL(exception(int, QString, QString, QString)),
            this, SIGNAL(return_error_of_trio(int,QString,QString,QString)));
}

thread_Trio::~thread_Trio()
{
    if (trio->IsOpen(2))
    {
        trio->Run("END");
    }
    qSleep(300);

    delete trio;
}

void thread_Trio::connect_Trio(bool *ok)
{
    trio->SetHost(IP_MC664);
    if(trio->Open(2,0))
    {
        *ok=true;
        trio->Op(QVariant(32),QVariant(1));
    }else
    {
        *ok=false;
    }
}

void thread_Trio::close_Trio()
{
    if (trio->IsOpen(2))
    {
        trio->Close();
    }
}

void thread_Trio::send_txt_to_Trio(bool *ok, const QString source_file_path,
                                   const QString destination_file_path)
{
    if (!trio->IsOpen(2))
    {
        return;
    }

    if(trio->TextFileLoader(source_file_path,0,destination_file_path,0,0,0,0,0,0))
    {
        *ok=true;
    }else
    {
        *ok=false;
    }

}

void thread_Trio::run_program_of_Trio(bool *ok, QString program_name)
{
    if (!trio->IsOpen(2))
    {
        return;
    }

    if(trio->Run(program_name))
    {
        *ok=true;
    }else
    {
        *ok=false;
    }

    double MTYPE_x(-1),MTYPE_y(-1);
    bool first_time_delay(true);
    do
    {
        qSleep(500);
        double* axis_position_paras=new double[5];
        for (uint i=0;i<5;i++)
        {
            axis_position_paras[i]=0;
        }

        trio->GetAxisVariable(QString("MPOS"),1,axis_position_paras[1]);
        trio->GetAxisVariable(QString("MPOS"),3,axis_position_paras[3]);
        emit return_axis_paras(axis_position_paras);
        if(first_time_delay)
        {
            qSleep(800);
            first_time_delay=false;
        }
        trio->GetAxisVariable(QString("MTYPE"),1,MTYPE_x);
        trio->GetAxisVariable(QString("MTYPE"),3,MTYPE_y);
    }while((MTYPE_x!=0)|(MTYPE_y!=0));
}

void thread_Trio::run_program_MANUAL_MODE(bool *ok)
{
    if (!trio->IsOpen(2))
    {
        return;
    }

    if(trio->Run("MANUAL_MODE",5))
    {
        *ok=true;
    }else
    {
        *ok=false;
    }
}

void thread_Trio::grab_axis_paras(bool* ok, double* paras_array)
{
    if (!trio->IsOpen(2))
    {
        return;
    }

    *ok=true;

    if(trio->GetAxisVariable(QString("MPOS"),1,*(paras_array+1)))
    {

    }else
    {
        *ok=false;
    }

    if(trio->GetAxisVariable(QString("MPOS"),3,*(paras_array+3)))
    {

    }else
    {
        *ok=false;
    }

    if(*ok)
    {
        emit return_axis_paras(paras_array);
    }
}

