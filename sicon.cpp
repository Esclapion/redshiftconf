#include "sicon.h"
#include <QMenu>
#include <QAction>
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QTest>

sicon::sicon(QObject *parent) : QObject(parent)
{
    icon = new QSystemTrayIcon(this);
    QProcess procrun;
    QMenu *menu  = new QMenu();
    QAction *conf = new QAction("Configure", 0);
    QAction *quit = new QAction("Quit", 0);
    menu->addAction(conf);
    menu->addAction(quit);
    connect(conf, SIGNAL(triggered()), this, SLOT(redconf()));
    connect(quit, SIGNAL(triggered()), this, SLOT(redquit()));
    
    connect(icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(redclick(QSystemTrayIcon::ActivationReason)));
    
    icon->setContextMenu(menu);
    
    procrun.start("pgrep redshift");
    procrun.waitForFinished();
    redshiftOn = procrun.exitCode();
    if(redshiftOn == 0)
        icon->setIcon(QIcon("/usr/share/icons/hicolor/scalable/apps/redshift-status-on.svg"));
    else
        icon->setIcon(QIcon("/usr/share/icons/hicolor/scalable/apps/redshift-status-off.svg"));
    icon->show();
    while(1)
        QTest::qWait(10000);

}

void sicon::redclick(QSystemTrayIcon::ActivationReason reason)
{
    QProcess comm;

    if(reason == QSystemTrayIcon::Trigger)
    {
        if(redshiftOn == 0)
        {
            comm.start("killall redshift");
            comm.waitForFinished();
            icon->setVisible(false);
            icon->setIcon(QIcon("/usr/share/icons/hicolor/scalable/apps/redshift-status-off.svg"));
            icon->setVisible(true);
            redshiftOn = 1;

        }
        else {
            QString redshiftComm;
            QDir dir(QDir::home());
            QFile file(dir.absoluteFilePath(".config/autostart/redshift.desktop"));
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                QString line = in.readLine();
                while (!line.isNull()) {
                    if(line.startsWith("Exec="))
                    {
                        line.remove(0, 5);
                        redshiftComm = line;
                        break;
                    }
                    line = in.readLine();
                }
                file.close();
                comm.startDetached(redshiftComm);
                comm.waitForStarted();
                icon->setVisible(false);
                icon->setIcon(QIcon("/usr/share/icons/hicolor/scalable/apps/redshift-status-on.svg"));
                icon->setVisible(true);
                redshiftOn = 0;
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("redshift is not configured !\n\n(right click on the icon)\n");
                msgBox.exec();
            }

        }
        
    }
}

void sicon::redconf()
{
    w.show();
}

void sicon::redquit()
{
    exit(0);
}
