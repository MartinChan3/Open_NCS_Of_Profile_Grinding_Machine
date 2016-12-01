#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColor>
#include <QGuiApplication>
#include <QDebug>
#include <QVariant>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Initialize the widgets of UI
    mainButton[0]=ui->pB_data;
    mainButton[1]=ui->pB_manul;
    mainButton[2]=ui->pB_program;
    mainButton[3]=ui->pB_manufacture;
    mainButton[4]=ui->pB_view;
    mainButton[5]=ui->pB_setting;
    subButton[0]=ui->pB2_0;
    subButton[1]=ui->pB2_1;
    subButton[2]=ui->pB2_2;
    subButton[3]=ui->pB2_3;
    subButton[4]=ui->pB2_4;
    sub2Button[0]=ui->pB3_0;
    sub2Button[1]=ui->pB3_1;
    sub2Button[2]=ui->pB3_2;
    sub2Button[3]=ui->pB3_3;
    sub2Button[4]=ui->pB3_4;
    sub2Button[5]=ui->pB3_5;
    sub2Button[6]=ui->pB3_6;
    sub2Button[7]=ui->pB3_7;

    ButtonGroup_main=new QButtonGroup();
    ButtonGroup_main->setExclusive(true);
    ButtonGroup_main->addButton(mainButton[0],0);
    ButtonGroup_main->addButton(mainButton[1],1);
    ButtonGroup_main->addButton(mainButton[2],2);
    ButtonGroup_main->addButton(mainButton[3],3);
    ButtonGroup_main->addButton(mainButton[4],4);
    ButtonGroup_main->addButton(mainButton[5],5);
    ButtonGroup_sub=new QButtonGroup();
    ButtonGroup_sub->setExclusive(true);
    ButtonGroup_sub->addButton(subButton[0],0);
    ButtonGroup_sub->addButton(subButton[1],1);
    ButtonGroup_sub->addButton(subButton[2],2);
    ButtonGroup_sub->addButton(subButton[3],3);
    ButtonGroup_sub->addButton(subButton[4],4);
    ButtonGroup_sub2=new QButtonGroup();
    ButtonGroup_sub2->setExclusive(true);
    ButtonGroup_sub2->addButton(sub2Button[0],0);
    ButtonGroup_sub2->addButton(sub2Button[1],1);
    ButtonGroup_sub2->addButton(sub2Button[2],2);
    ButtonGroup_sub2->addButton(sub2Button[3],3);
    ButtonGroup_sub2->addButton(sub2Button[4],4);
    ButtonGroup_sub2->addButton(sub2Button[5],5);
    ButtonGroup_sub2->addButton(sub2Button[6],6);
    ButtonGroup_sub2->addButton(sub2Button[7],7);
    for(int i=0;i<5;i++)
    {
    mainButton[i]->setAutoFillBackground(true);
    }
    for(int i=0;i<4;i++)
    {
    subButton[i]->setAutoFillBackground(true);
    }
    for(int i=0;i<7;i++)
    {
    sub2Button[i]->setAutoFillBackground(true);
    }

    this->setAutoFillBackground(true);

    Palette_Unpressed=QGuiApplication::palette();
    Palette_Pressed=QGuiApplication::palette();
    Palette_Unconnected=QGuiApplication::palette();
    Palette_Connected=QGuiApplication::palette();
    Palette_Pressed.setBrush(QPalette::Button,QColor(Qt::black));
    Palette_Pressed.setBrush(QPalette::ButtonText,QColor(Qt::red));
    Palette_Unconnected.setBrush(QPalette::Text,QColor(Qt::white));
    Palette_Unconnected.setBrush(QPalette::Background,QColor(Qt::red));
    Palette_Connected.setBrush(QPalette::Text,QColor(Qt::white));
    Palette_Connected.setBrush(QPalette::Background,QColor(Qt::green));

    current_ButtonID_main=-1;
    current_ButtonID_sub=-1;
    current_ButtonID_sub2=-1;
    mainStack=ui->Stacked_Pages_Main;
    editStack=ui->Stacked_Pages_Sub;
    Label_Connection_Status=ui->Label_Connection_Status;
    Label_Connection_Status->setAutoFillBackground(true);
    Label_Connection_Status->setPalette(Palette_Unconnected);

    dir_of_txt=QDir::root();
    Connection_Status_of_Trio=false;

    //Initialize things about Multi-Threads
    THREAD_CCD=new QThread();
    ccd=new thread_CCD();
    ccd->moveToThread(THREAD_CCD);

    //Default UI setting
    ui->Stacked_Pages_Main->setCurrentIndex(0);

    //Set Trio
    trio=new TrioPCLib::TrioPC();
    trio->SetHost(QString("127.0.0.1"));
    if(trio->Open(2,0))
    {
        Label_Connection_Status->setPalette(Palette_Connected);
        Label_Connection_Status->setText(QString("已连接"));
        Connection_Status_of_Trio=true;

        if(trio->TextFileLoader(QString("D:/code.txt"),0,QString("CODE"),0,0,0,0,0,0))
        {

//            for (int i=0;i<100;i++)
//            {
//                trio->SetVr(1000+i,0);
//            }
//            trio->SetVr(1000,49);
//            trio->SetVr(1001,50);
//            trio->SetVr(1002,51);
//            trio->Run("STR_TEST");

//            QString string;
//            trio->Dir(string);
//            QMessageBox::about(Q_NULLPTR,"ABOUT",string);
        }
    }
    else
    {
        Label_Connection_Status->setPalette(Palette_Unconnected);
        Connection_Status_of_Trio=false;
    }

    //Connect the signals and slots
    connect(ButtonGroup_main,SIGNAL(buttonClicked(int)),this,SLOT(pressed_mainButtonGroup(int)));
    connect(ButtonGroup_sub,SIGNAL(buttonClicked(int)),this,SLOT(pressed_subButtonGroup(int)));
    connect(ButtonGroup_sub2,SIGNAL(buttonClicked(int)),this,SLOT(pressed_sub2ButtonGroup(int)));

    connect(ui->pB_New_Txt,SIGNAL(clicked()),this,SLOT(txtfile_new_built()));
    connect(ui->pB_ReadIn_Txt,SIGNAL(clicked()),this,SLOT(txtfile_readin()));
    connect(ui->pB_Save_Txt,SIGNAL(clicked()),this,SLOT(txtfile_save()));
    connect(ui->pB_Undo_Txt,SIGNAL(clicked()),this,SLOT(txtfile_undo()));
    connect(ui->pB_GrammarCheck_Txt,SIGNAL(clicked()),this,SLOT(txtfile_grammar_check()));
    connect(ui->pB_Send_Txt,SIGNAL(clicked()),this,SLOT(txtfile_send_to_trio()));
    connect(this,SIGNAL(cB_Txt_Changed(QString)),this,SLOT(cB_Txt_Dir_Content(QString)));
    connect(ui->cB_Txt,SIGNAL(activated(QString)),this,SLOT(cB_current_index_changed(QString)));

    connect(this,SIGNAL(errors_in_runtime(int)),this,SLOT(errors_handled(int)));
    connect(ui->pB_Connection_of_Trio,SIGNAL(clicked()),this,SLOT(pB_Connection()));
}

MainWindow::~MainWindow()
{
    trio->close();

    delete trio;
    for(int i=0;i<=5;i++)
    {
    delete mainButton[i];
    }
    for(int i=0;i<=4;i++)
    {
        delete subButton[i];
    }
    for(int i=0;i<=7;i++)
    {
        delete sub2Button[i];
    }

    delete ccd;
    THREAD_CCD->quit();
    THREAD_CCD->wait();
    delete THREAD_CCD;

    delete ui;
}

//*******************About Buttons*******************//
void MainWindow::pressed_mainButtonGroup(int i)
{
    for(int j=0;j<=5;j++)
    {
        button_unpressed(mainButton[j]);
    }
    button_pressed(mainButton[i]);

    mainStack->setCurrentIndex(0);
    clear_button_text(ALL_SUB);
    switch (i) {
    case 0:
        subButton[0]->setText(QString("工具"));
        subButton[1]->setText(QString("其它"));
        break;
    case 1:
        subButton[0]->setText(QString("准备"));
        subButton[1]->setText(QString("MDI"));
        subButton[2]->setText(QString("示教"));
        break;
    case 2:
        subButton[0]->setText(QString("文件"));
        subButton[1]->setText(QString("图形"));
        subButton[2]->setText(QString("凸轮"));
        mainStack->setCurrentIndex(1);
        break;
    case 3:
        subButton[0]->setText(QString("文件"));
        subButton[1]->setText(QString("执行"));
        subButton[2]->setText(QString("模拟"));
        break;
    case 4:
        subButton[0]->setText(QString("CCD"));
        subButton[1]->setText(QString("坐标"));
        subButton[2]->setText(QString("维护"));
        subButton[3]->setText(QString("View"));
        break;
    case 5:

        break;

    default:
        clear_button_text(ALL_SUB);
        break;
    }
    current_ButtonID_main=i;//Remember the necessary main button Group ID
}

void MainWindow::pressed_subButtonGroup(int i)
{
    for(int j=0;j<=4;j++)
    {
        button_unpressed(subButton[j]);
    }
    button_pressed(subButton[i]);

    clear_button_text(SUB2);

    switch(current_ButtonID_main)
    {
    case 0:
        switch(i)
        {
        case 0:
            sub2Button[0]->setText(QString("刀具库"));
            sub2Button[1]->setText(QString("上偏差"));
            sub2Button[2]->setText(QString("下偏差"));
            sub2Button[3]->setText(QString("补偿量"));
            break;
        case 1:
            sub2Button[0]->setText(QString("零点偏置"));
            break;
        default:

            break;
        }
        break;
    case 1:
        switch(i)
        {
        case 0:
            sub2Button[0]->setText(QString("原点复归"));
            sub2Button[1]->setText(QString("坐标设定"));
            sub2Button[2]->setText(QString("绝对移动"));
            sub2Button[3]->setText(QString("相对移动"));
            sub2Button[4]->setText(QString("M指令"));
            sub2Button[5]->setText(QString("F指令"));
            break;
        case 1:

            break;
        case 2:
            sub2Button[0]->setText(QString("开始"));
            sub2Button[1]->setText(QString("直线终点"));
            sub2Button[2]->setText(QString("圆弧1点"));
            sub2Button[3]->setText(QString("圆弧2点"));
            sub2Button[4]->setText(QString("结束"));
            sub2Button[5]->setText(QString("保存"));
            sub2Button[5]->setText(QString("描绘"));
            break;
        default:

            break;
        }
        break;
    case 2:
        switch(i)
        {
        case 0:
            sub2Button[0]->setText(QString("拷贝"));
            sub2Button[1]->setText(QString("删除"));
            sub2Button[2]->setText(QString("重命名"));
            sub2Button[3]->setText(QString("保护"));
            break;
        case 1:
            sub2Button[0]->setText(QString("载入"));
            sub2Button[1]->setText(QString("描绘"));
            sub2Button[2]->setText(QString("参数设置"));
            break;
        case 2:
            sub2Button[0]->setText(QString("生成法"));
            sub2Button[1]->setText(QString("参数曲线"));
            sub2Button[2]->setText(QString("点信息"));
            break;
        default:
            break;
        }
        break;
    case 3:
        switch(i)
        {
        case 0:
            break;
        case 1:
            sub2Button[0]->setText(QString("单段"));
            sub2Button[1]->setText(QString("连续"));
            break;
        case 2:
            sub2Button[0]->setText(QString("超程检测"));
            break;
        default:
            break;
        }
        break;
    case 4:
        switch(i)
        {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            sub2Button[0]->setText(QString("坐标系"));
            sub2Button[1]->setText(QString("刀具"));
            sub2Button[2]->setText(QString("补偿"));
        default:
            break;
        }
        break;
    default:
        break;
    }
    current_ButtonID_sub=i;
}

void MainWindow::pressed_sub2ButtonGroup(int i)
{
    for(int j=0;j<=7;j++)
    {
        button_unpressed(sub2Button[j]);
    }
    button_pressed(sub2Button[i]);

    switch(current_ButtonID_main)
    {
    case 0:
        switch (current_ButtonID_sub) {
        case 0:
            switch (i) {
            case 0:
                editStack->setCurrentIndex(1);
                break;
            case 1:
                editStack->setCurrentIndex(2);
                break;
            case 2:
                editStack->setCurrentIndex(3);
                break;
            case 3:
                editStack->setCurrentIndex(4);
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (i) {
            case 0:
                editStack->setCurrentIndex(5);
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (i) {
            case 0:
                editStack->setCurrentIndex(6);
                break;
            case 1:
                editStack->setCurrentIndex(7);
                break;
            case 2:
                editStack->setCurrentIndex(8);
                break;
            case 3:
                editStack->setCurrentIndex(9);
                break;
            case 4:
                editStack->setCurrentIndex(10);
                break;
            case 5:
                editStack->setCurrentIndex(11);
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (i) {
            case 0:

                break;
            case 1:

                break;
            default:
                break;
            }
        default:
            break;
        }

        break;
    case 1:
        switch (current_ButtonID_sub) {
        case 0:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    case 2:
        switch (current_ButtonID_sub) {
        case 0:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    case 3:
        switch (current_ButtonID_sub) {
        case 0:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (i) {
            case 0:
                break;
            case 1:
            {
//                for (int i=0;i<100;i++)
//                {
//                    trio->SetVr(1000+i,0);
//                }
//                QString str="TRANSFER_FILE";
//                for (uint i=0;i<sizeof(str);i++)
//                {
//                    trio->SetVr(1000+i,QString(str.at(i)).toInt());
//                    qDebug()<<QString(str.at(i)).toInt();
//                }
                trio->Run("LOADTEXT");
            }
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (i) {
            case 0:
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void MainWindow::button_pressed(QPushButton *button)
{
    button->setPalette(Palette_Pressed);
}

void MainWindow::button_unpressed(QPushButton *button)
{
    button->setPalette(Palette_Unpressed);
}

void MainWindow::clear_button_text(BUTTON_GROUP_TYPE group_type)
{
    switch(group_type)
    {
    case MAIN:
        mainButton[0]->setText(QString(" "));
        mainButton[1]->setText(QString(" "));
        mainButton[2]->setText(QString(" "));
        mainButton[3]->setText(QString(" "));
        mainButton[4]->setText(QString(" "));
        mainButton[5]->setText(QString(" "));
        button_unpressed(mainButton[0]);
        button_unpressed(mainButton[1]);
        button_unpressed(mainButton[2]);
        button_unpressed(mainButton[3]);
        button_unpressed(mainButton[4]);
        button_unpressed(mainButton[5]);
        break;
    case SUB:
        subButton[0]->setText(QString(" "));
        subButton[1]->setText(QString(" "));
        subButton[2]->setText(QString(" "));
        subButton[3]->setText(QString(" "));
        subButton[4]->setText(QString(" "));
        button_unpressed(subButton[0]);
        button_unpressed(subButton[1]);
        button_unpressed(subButton[2]);
        button_unpressed(subButton[3]);
        button_unpressed(subButton[4]);
        break;
    case SUB2:
        sub2Button[0]->setText(QString(" "));
        sub2Button[1]->setText(QString(" "));
        sub2Button[2]->setText(QString(" "));
        sub2Button[3]->setText(QString(" "));
        sub2Button[4]->setText(QString(" "));
        sub2Button[5]->setText(QString(" "));
        sub2Button[6]->setText(QString(" "));
        sub2Button[7]->setText(QString(" "));
        button_unpressed(sub2Button[0]);
        button_unpressed(sub2Button[1]);
        button_unpressed(sub2Button[2]);
        button_unpressed(sub2Button[3]);
        button_unpressed(sub2Button[4]);
        button_unpressed(sub2Button[5]);
        button_unpressed(sub2Button[6]);
        button_unpressed(sub2Button[7]);
        break;
    case ALL_SUB:
        subButton[0]->setText(QString(" "));
        subButton[1]->setText(QString(" "));
        subButton[2]->setText(QString(" "));
        subButton[3]->setText(QString(" "));
        subButton[4]->setText(QString(" "));
        sub2Button[0]->setText(QString(" "));
        sub2Button[1]->setText(QString(" "));
        sub2Button[2]->setText(QString(" "));
        sub2Button[3]->setText(QString(" "));
        sub2Button[4]->setText(QString(" "));
        sub2Button[5]->setText(QString(" "));
        sub2Button[6]->setText(QString(" "));
        sub2Button[7]->setText(QString(" "));
        button_unpressed(subButton[0]);
        button_unpressed(subButton[1]);
        button_unpressed(subButton[2]);
        button_unpressed(subButton[3]);
        button_unpressed(subButton[4]);
        button_unpressed(sub2Button[0]);
        button_unpressed(sub2Button[1]);
        button_unpressed(sub2Button[2]);
        button_unpressed(sub2Button[3]);
        button_unpressed(sub2Button[4]);
        button_unpressed(sub2Button[5]);
        button_unpressed(sub2Button[6]);
        button_unpressed(sub2Button[7]);
        break;
    case ALL:
        mainButton[0]->setText(QString(" "));
        mainButton[1]->setText(QString(" "));
        mainButton[2]->setText(QString(" "));
        mainButton[3]->setText(QString(" "));
        mainButton[4]->setText(QString(" "));
        mainButton[5]->setText(QString(" "));
        subButton[0]->setText(QString(" "));
        subButton[1]->setText(QString(" "));
        subButton[2]->setText(QString(" "));
        subButton[3]->setText(QString(" "));
        subButton[4]->setText(QString(" "));
        sub2Button[0]->setText(QString(" "));
        sub2Button[1]->setText(QString(" "));
        sub2Button[2]->setText(QString(" "));
        sub2Button[3]->setText(QString(" "));
        sub2Button[4]->setText(QString(" "));
        sub2Button[5]->setText(QString(" "));
        sub2Button[6]->setText(QString(" "));
        sub2Button[7]->setText(QString(" "));
        button_unpressed(mainButton[0]);
        button_unpressed(mainButton[1]);
        button_unpressed(mainButton[2]);
        button_unpressed(mainButton[3]);
        button_unpressed(mainButton[4]);
        button_unpressed(mainButton[5]);
        button_unpressed(subButton[0]);
        button_unpressed(subButton[1]);
        button_unpressed(subButton[2]);
        button_unpressed(subButton[3]);
        button_unpressed(subButton[4]);
        button_unpressed(sub2Button[0]);
        button_unpressed(sub2Button[1]);
        button_unpressed(sub2Button[2]);
        button_unpressed(sub2Button[3]);
        button_unpressed(sub2Button[4]);
        button_unpressed(sub2Button[5]);
        button_unpressed(sub2Button[6]);
        button_unpressed(sub2Button[7]);
        break;
    }
}

void MainWindow::txtfile_new_built()
{

}

void MainWindow::txtfile_readin()
{
    QString fileDir=QFileDialog::getExistingDirectory(this,QString("请选择目录"),QString("目录为"));
    emit cB_Txt_Changed(fileDir);
}

void MainWindow::txtfile_save()
{
     QString fileName_Str,txt_file_absolute_path;
     fileName_Str=ui->cB_Txt->currentText();
     txt_file_absolute_path=dir_of_txt.absolutePath()+"/"+fileName_Str;

     QFile txt_file(txt_file_absolute_path);
     if (!txt_file.open(QIODevice::WriteOnly|QIODevice::Text))
     {
         emit errors_in_runtime(2);
         return;
     }

     QTextStream txt_stream(&txt_file);
     txt_stream<<ui->pTE_GCode->toPlainText();
     txt_file.close();
}

void MainWindow::txtfile_undo()
{
    ui->pTE_GCode->undo();
}

void MainWindow::txtfile_grammar_check()
{

}

void MainWindow::txtfile_send_to_trio()
{

    QString fileName_Str,txt_file_absolute_path;
    fileName_Str=ui->cB_Txt->currentText();
    txt_file_absolute_path=dir_of_txt.absolutePath()+"/"+fileName_Str;
    if(!trio->TextFileLoader(txt_file_absolute_path,0,QString("TEMP_FILE"),0,0,0,0,0,0))
    {
       emit errors_in_runtime(3);
    }else
    {
        QString string;
        trio->Dir(string);
        QMessageBox::about(Q_NULLPTR,"ABOUT",string);
    }
}


//*******************About Buttons*******************//
void MainWindow::cB_Txt_Dir_Content(QString str)
{
    QDir dir(str);
    if (!dir.exists())
    {
        emit errors_in_runtime(1);
        return;
    }

    dir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::Readable|QDir::Writable);
    dir.setSorting(QDir::Name);

    QStringList txt_Name_List=dir.entryList();
    ui->cB_Txt->clear();
    foreach(QString str,txt_Name_List)
    {
        if ((str.right(4)==".txt")|(str.right(4)==".TXT"))
        {
            ui->cB_Txt->addItem(str);
        }
    }
    dir_of_txt=dir;
}

void MainWindow::cB_current_index_changed(QString fileName_Str)
{
    QString txt_file_absolute_path=dir_of_txt.absolutePath()+"/"+fileName_Str;
    QFile txt_file(txt_file_absolute_path);
    if (!txt_file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        emit errors_in_runtime(2);
        return;
    }

    ui->pTE_GCode->clear();
    QTextStream txt_stream(&txt_file);
    const QString all_txt=txt_stream.readAll();
    ui->pTE_GCode->setPlainText(all_txt);
    txt_file.close();
}



void MainWindow::errors_handled(int error_type)
{
    QString error_title,error_content;
    error_title=QString("发生错误");
    error_content=QString("未知错误");
    switch (error_type) {
    case 1:
        error_content=QString("选择的目录不存在！");
        break;
    case 2:
        error_content=QString("选择的txt文件无法打开！");
        break;
    default:
        break;
    }

    QMessageBox::warning(this,error_title,error_content);
}

void MainWindow::pB_Connection()
{
    if (Connection_Status_of_Trio==true)
    {
        return;
    }

    trio->SetHost(QString("127.0.0.1"));
    if(trio->Open(2,0))
    {
        Label_Connection_Status->setPalette(Palette_Connected);
        Label_Connection_Status->setText(QString("已连接"));
        Connection_Status_of_Trio=true;
    }
    else
    {
        Label_Connection_Status->setPalette(Palette_Unconnected);
        Label_Connection_Status->setText(QString("未连接"));
        Connection_Status_of_Trio=false;
    }
}
