#ifndef THREAD_CCD_H
#define THREAD_CCD_H
#include <QObject>

class thread_CCD : public QObject
{
    Q_OBJECT
public:
    explicit thread_CCD(QObject *parent = 0);
    ~thread_CCD();
signals:
    void image_captured(bool);

public slots:
    void initialize();
    void capture_image(char*);
};

#endif // THREAD_CCD_H
