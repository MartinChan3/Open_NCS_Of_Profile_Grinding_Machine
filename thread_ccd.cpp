#include "thread_ccd.h"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <qt_windows.h>
#define CAM1_IP 0xC0A80101
#define MAX_BUF_SIZE 2448*2058

LARGE_INTEGER before;
LARGE_INTEGER after;
LARGE_INTEGER frequency;

using namespace MvCamCtrl;

thread_CCD::thread_CCD(QObject *parent) : QObject(parent)
{
    MyDevice=NULL;
    nRet=MV_OK;
    output_img=true;
}

thread_CCD::~thread_CCD()
{   
    if (MyDevice!=NULL)
    {
        if (MyDevice->IsOpen())
        {
            MyDevice->Close();
        }
        CTlFactory::GetInstance().DestroyDevice(MyDevice);
    }else
    {
        delete MyDevice;
    }
}

bool thread_CCD::check_ccd_status()
{
    if (MyDevice==NULL)
    {
        return false;
    }else
    {
        if (!MyDevice->IsOpen())
        {
            return false;
        }else
        {
            return true;
        }
    }

}

void thread_CCD::qSleep(int ms)
{
    common::qSleep(ms);
}

void thread_CCD::initialize()
{
    QueryPerformanceFrequency(&frequency);
    //    QueryPerformanceCounter(&before);
    bool output_flag(true);

    CTlFactory &tlFactory=CTlFactory::GetInstance();
    MV_CC_DEVICE_INFO_LIST stDevList;
    unsigned int nSupportedTls=tlFactory.EnumerateTls();
    //    if (MV_GIGE_DEVICE==(nSupportedTls&MV_GIGE_DEVICE))
    //    {
    memset(&stDevList,0,sizeof(MV_CC_DEVICE_INFO_LIST));
    nRet=tlFactory.EnumDevices(MV_GIGE_DEVICE,stDevList);
    if(MV_OK !=nRet)
    {
        printf("error: EnumDevices [%x]\n", nRet);
    }
    unsigned int j=stDevList.nDeviceNum;
    unsigned int i=0;
    qDebug("The number of device is %d",j);
    if (stDevList.nDeviceNum==0)
    {
        qDebug("No camera found\n");
        qSleep(100000000);
    }
    else
    {
        for (unsigned int n=0;n<stDevList.nDeviceNum;n++)
        {
            qDebug("%d . %s\n", n, stDevList.pDeviceInfo[n]->SpecialInfo.stGigEInfo.chModelName);
        }
    }
    if (i < 0 || i >= stDevList.nDeviceNum)
    {
        printf("error : input\n");
        qSleep(10000000);
    }

    //        CTlFactory &tlFactory=CTlFactory::GetInstance();
    CMvGigEDevice* MyDevice = dynamic_cast<CMvGigEDevice*>(tlFactory.CreateDevice(*(stDevList.pDeviceInfo[0])));
    if (NULL==MyDevice){
        qDebug("error:CreatDevices\n");
    }

    nRet=MyDevice->ForceIp(CAM1_IP);
    if (nRet!=MV_OK)
    {
        qDebug("error:ForceIp[%x]\n",nRet);

    }

    if(!MyDevice->IsOpen())
    {
        nRet=MyDevice->Open();
        if (MV_OK != nRet)
        {
            printf("error: Open [%x]\n", nRet);
        }

    }
    nRet=MyDevice->StartGrabbing();
    if(MV_OK!=nRet)
    {
        printf("error: StartGrabbing [%x]\n", nRet);

    }else
    {
        unsigned int nTestFrameSize=0;
        MV_FRAME_OUT_INFO stInfo;
        memset(&stInfo,0,sizeof(MV_FRAME_OUT_INFO));
        //核心，抓取图像模块，建议多线程采集，并且上位机需控制好采集频率
        QueryPerformanceCounter(&before);
        int i(0);
        while(output_img){
            while(1)
            {
                if (nTestFrameSize > 99)
                {
                    break;
                }
                uchar* g_pFrameBuf=NULL;
                g_pFrameBuf=new uchar[MAX_BUF_SIZE];
                nRet = MyDevice->GetOneFrame(g_pFrameBuf, MAX_BUF_SIZE, &stInfo);
                if (MV_OK != nRet)
                {
                    delete g_pFrameBuf;
                }
                else
                {
                    //...图像数据处理
                    emit image_captured(g_pFrameBuf);
                    if (output_flag)
                    {
                        QueryPerformanceCounter(&after);
                        double milliseconds=((double)after.QuadPart-(double)before.QuadPart)/((double)frequency.QuadPart);
                        QString str=QString::fromLocal8Bit("总体消耗时间")+QString::number(milliseconds);
                        qDebug()<<str;

                        before=after;
                        i++;
                        if (i>10)
                            output_flag=false;
                    }
                    break;
                }
                nTestFrameSize++;
            }
            qSleep(100);
        }

        MyDevice->StopGrabbing();
    }
    MyDevice->Close();
    //    }
}


void thread_CCD::capture_image()
{
    ////    if (!check_ccd_status())
    ////    {
    ////        emit error_of_ccd("CCD open error");
    ////        return;
    ////    }
    //    unsigned int nTestFrameSize=0;
    //    MV_FRAME_OUT_INFO stInfo;
    //    memset(&stInfo,0,sizeof(MV_FRAME_OUT_INFO));
    //    MV_NETTRANS_INFO stNetTransInfo={0};
    //    //核心，抓取图像模块，建议多线程采集，并且上位机需控制好采集频率
    //    while(1)
    //    {
    //        if (nTestFrameSize > 99)
    //        {
    //            break;
    //        }
    //        uchar* g_pFrameBuf=NULL;
    //        g_pFrameBuf=new uchar[MAX_BUF_SIZE];
    //        nRet = MyDevice->GetOneFrame(g_pFrameBuf, MAX_BUF_SIZE, &stInfo);
    //        if (MV_OK != nRet)
    //        {
    //            delete g_pFrameBuf;
    //        }
    //        else
    //        {
    //            //...图像数据处理
    //            emit image_captured(g_pFrameBuf);
    //            break;
    //            // 获取网络传输相关信息
    ////            if ((nTestFrameSize % 10) == 0)
    ////            {
    ////                nRet = MyDevice->GetNetTransInfo(&stNetTransInfo);
    ////                printf("GetDataSize [%I64d], nRet[%x]\n", stNetTransInfo.nReviceDataSize, nRet);
    ////            }
    //        }
    //        nTestFrameSize++;
    //    }

}

void thread_CCD::set_output_img()
{
    output_img=false;
}


