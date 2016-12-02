#include "thread_trio.h"

thread_Trio::thread_Trio(QObject *parent) : QObject(parent)
{
    trio=new TrioPCLib::TrioPC();

    connect(trio, SIGNAL(exception(int, QString, QString, QString)),
            this, SIGNAL(return_error_of_trio(int,QString,QString,QString)));
}

thread_Trio::~thread_Trio()
{
    delete trio;
}

void thread_Trio::connect_Trio(bool *ok)
{
    trio->SetHost("127.0.0.1");
    if(trio->Open(2,0))
    {
        *ok=true;
    }else
    {
        *ok=false;
    }
}

void thread_Trio::send_txt_to_Trio(bool *ok, const QString source_file_path,
                                   const QString destination_file_path)
{
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
    if(trio->Run(program_name))
    {
        *ok=true;
    }else
    {
        *ok=false;
    }
}

