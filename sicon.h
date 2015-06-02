#ifndef SICON_H
#define SICON_H

#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>
#include "redshiftconf.h"

class sicon : public QObject
{
    Q_OBJECT
public:
    explicit sicon(QObject *parent = 0);


private:
    int redshiftOn;
    QSystemTrayIcon *icon;
    redshiftconf w;



signals:

public slots:


private slots:
    void redclick(QSystemTrayIcon::ActivationReason reason);
    void redconf();
    void redquit();

};

#endif // SICON_H
