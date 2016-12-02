#ifndef COMMON_H
#define COMMON_H

#include <QObject>

class common : public QObject
{
    Q_OBJECT
public:
    explicit common(QObject *parent = 0);

signals:

public slots:
    static void qSleep(const int ms);
};

#endif // COMMON_H
