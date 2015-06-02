#ifndef REDSHIFTCONF_H
#define REDSHIFTCONF_H

#include <QWidget>

#include <QTextStream> // TO SUPPRESS

namespace Ui {
class redshiftconf;
}

class redshiftconf : public QWidget
{
    Q_OBJECT

public:
    explicit redshiftconf(QWidget *parent = 0);
    ~redshiftconf();

private slots:
    void on_ButTest_clicked();

    void on_butConf_clicked();

private:
    Ui::redshiftconf *ui;
};

#endif // REDSHIFTCONF_H
