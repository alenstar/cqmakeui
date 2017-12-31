#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(&this->cmd_, &CmdThread::error, this, &Dialog::error);
    connect(&this->cmd_, &CmdThread::finished, this, &Dialog::finished);
    connect(&this->cmd_, &CmdThread::progress, this, &Dialog::progress);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnBrowser_clicked()
{
   QString pathname = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QString::fromUtf8(qgetenv("HOME")));
   if(!pathname.isEmpty()) {
      pathname_ = pathname;
      ui->lePath->setText(pathname);
   }
}

void Dialog::on_btnCreate_clicked()
{
    if(ui->lePath->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warnning !"), tr("source code path is empty"), QMessageBox::Ok);
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), QString::fromUtf8(qgetenv("HOME")), tr("CodeQueryDB(*.db)"));
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".db")) {
           filename += ".db";
        }
        if(QFile::exists(filename)) {
            QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Question @"), tr("the file is exists, replace ?"), QMessageBox::Ok|QMessageBox::Cancel);
            if (btn == QMessageBox::Cancel) {
                return;
            }
            QFile::remove(filename);
        }

        cmd_.setWorkPath(pathname_);
        cmd_.setOutName(filename);

        if(ui->cbCPP->isChecked()) {
           cmd_.enableCplusplus();
        } else {
           cmd_.enableCplusplus(false);
        }
        if(ui->cbPython->isChecked()) {
           cmd_.enablePython();
        } else {
           cmd_.enablePython(false);
        }
        if(ui->cbJava->isChecked()) {
           cmd_.enableJava();
        } else {
           cmd_.enableJava(false);
        }
        if(ui->cbKernel->isChecked()) {
           cmd_.enableKernel();
        } else {
           cmd_.enableKernel(false);
        }
        if(ui->cbRecurse->isChecked()) {
           cmd_.enableRecurse();
        } else {
           cmd_.enableRecurse(false);
        }

        cmd_.start();

        ui->btnBrowser->setEnabled(false);
        ui->btnCreate->setEnabled(false);
    }
}

void Dialog::finished()
{

    ui->btnBrowser->setEnabled(true);
    ui->btnCreate->setEnabled(true);
}

void Dialog::error()
{
    ui->btnBrowser->setEnabled(true);
    ui->btnCreate->setEnabled(true);
    QMessageBox::warning(this, tr("error"), tr("create a CodeQuery database failed !"), QMessageBox::Ok);
}

void Dialog::progress(int prog)
{
    ui->progressBar->setValue(prog);
}
