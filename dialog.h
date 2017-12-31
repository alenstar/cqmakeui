#ifndef DIALOG_H
#define DIALOG_H

#include <cmdthread.h>
#include <QDialog>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btnBrowser_clicked();

    void on_btnCreate_clicked();

    void finished();
    void error();
    void progress(int prog);

private:
    Ui::Dialog *ui;
    QString pathname_;
    CmdThread cmd_;
};


#endif // DIALOG_H
