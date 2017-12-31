#ifndef CMDTHREAD_H
#define CMDTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>

class CmdThread : public QThread
{
        Q_OBJECT
public:
    explicit CmdThread(QObject* parent = nullptr);
    ~CmdThread();
    void run();

    void enableCplusplus(bool en = true);
    void enableJava(bool en = true);
    void enablePython(bool en = true);
    void enableRecurse(bool en = true);
    void enableKernel(bool en = true);

    void setOutName(const QString& name);
    void setWorkPath(const QString& path);

signals:
    void progress(int);
    void error();

private:
    QString outName_;
    QString workPath_;
    bool tagCplusplus_;
    bool tagJava_;
    bool tagPython_;
    bool tagRecurse;
    bool tagKernel;
};

#endif // CMDTHREAD_H
