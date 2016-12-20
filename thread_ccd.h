#ifndef THREAD_CCD_H
#define THREAD_CCD_H
#include <QObject>
#include "common.h"
#include "TlFactory.h"
#include "CameraParams.h"
#include "MvCameraControl.h"
#include "MvDeviceBase.h"
#include "MvGigEDevice.h"
#include "MvInclude.h"

class thread_CCD : public QObject
{
    Q_OBJECT
public:
    explicit thread_CCD(QObject *parent = 0);
    ~thread_CCD();
private:
    MvCamCtrl::CMvGigEDevice* MyDevice;
    int nRet;
    bool output_img;

    bool check_ccd_status();
    void qSleep(int);

signals:
    void image_captured(uchar*);
    void error_of_ccd(QString);

public slots:
    void initialize();
    void capture_image();
    void set_output_img();
};

#endif // THREAD_CCD_H
