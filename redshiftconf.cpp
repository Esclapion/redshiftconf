#include "redshiftconf.h"
#include "ui_redshiftconf.h"
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>

QTextStream cout(stdout, QIODevice::WriteOnly); // TO REMOVE !!!

redshiftconf::redshiftconf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::redshiftconf)
{
    ui->setupUi(this);
    QString Lat, Long;
    QProcess rcomm;
    QString resrcomm;
    QStringList listrcomm;
    rcomm.setReadChannel(QProcess::StandardOutput);
    rcomm.start("redshift -l geoclue2 -vop");
    if (rcomm.waitForFinished(1000) == true)
    {
        resrcomm = rcomm.readLine();
        listrcomm = resrcomm.split(' ', QString::KeepEmptyParts);
        Lat = listrcomm[1];
        if (listrcomm[2][0] == 'S')
            Lat.prepend("-");
        Long = listrcomm[3];
        if (listrcomm[4][0] == 'W')
            Long.prepend("-");
        ui->valLat->setValue(Lat.toDouble());
        ui->valLong->setValue(Long.toDouble());
    }
    else
    {
        rcomm.kill();
        rcomm.waitForFinished();
    }

}

redshiftconf::~redshiftconf()
{
    delete ui;
}

void buildCommand()
{
}

void redshiftconf::on_ButTest_clicked()
{
    QProcess process;
    QString command = QString("redshift -l %1:%2 -t %3:%4 -b %5:%6 -o")
                              .arg(ui->valLat->cleanText())
                              .arg(ui->valLong->cleanText())
                              .arg(ui->valDct->cleanText())
                              .arg(ui->valNct->cleanText())
                              .arg(ui->valDbri->cleanText())
                              .arg(ui->valNbri->cleanText());
    cout << command << endl;
    process.start(command);
    process.waitForFinished();
}

void redshiftconf::on_butConf_clicked()
{
    cout << "Configure" << endl;
    QDir dir(QDir::home());
    QString command = QString("Exec=redshift -l %1:%2 -t %3:%4 -b %5:%6")
                              .arg(ui->valLat->cleanText())
                              .arg(ui->valLong->cleanText())
                              .arg(ui->valDct->cleanText())
                              .arg(ui->valNct->cleanText())
                              .arg(ui->valDbri->cleanText())
                              .arg(ui->valNbri->cleanText());
    if(dir.exists(".config") == false)
        dir.mkdir(".config");
    dir.cd(".config");
    if(dir.exists("autostart") == false)
        dir.mkdir("autostart");
    dir.cd("autostart");
    cout << dir.absoluteFilePath("redshift.desktop") << endl;
    QFile file(dir.absoluteFilePath("redshift.desktop"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR : Can't open redshift.desktop for writing.");
        msgBox.exec();
    }
    else
    {
        QTextStream flux(&file);
        flux.setCodec("UTF-8");
        flux << "[Desktop Entry]" << endl;
        flux << "Encoding=UTF-8" << endl;
        flux << "Type=Application" << endl;
        flux << "Name=redshift" << endl;
        flux << command << endl;
        flux << "StartupNotify=false" << endl;
        flux << "Terminal=false" << endl;
        flux << "Hidden=false" << endl;
        file.close();
    }
}
