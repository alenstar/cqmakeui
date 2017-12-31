#include "cmdthread.h"
#include <QProcess>
#include <QStringList>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <QFile>
#include <QDebug>


CmdThread::CmdThread(QObject *parent):QThread (parent)
{
    tagCplusplus_ = true;
    tagJava_ = false;
    tagPython_ = false;
}

CmdThread::~CmdThread()
{

}

void CmdThread::run()
{
    QStringList cmds;

    cmds << QString("touch /tmp/%1.cscope.files").arg((unsigned long)getpid());

    if(tagCplusplus_) {
    cmds << QString("find %1 -iname '*.c' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.cc' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.cpp' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.cxx' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.h' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.hh' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.hpp' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    cmds << QString("find %1 -iname '*.hxx' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    }
    if(tagJava_) {
        cmds << QString("find %1 -iname '*.java' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    }
    if(tagPython_) {
        cmds << QString("find %1 -iname '*.py' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
        cmds << QString("find %1 -iname '*.py3' >> /tmp/%2.cscope.files").arg(workPath_).arg((unsigned long)getpid());
    }

    QString cscope = QString("cscope -cb");// -F/tmp/%1.cscope.files -f/tmp/%2.cscope.out").arg((unsigned long)getpid()).arg((unsigned long)getpid());
    if (tagKernel) {
       cscope += "k ";
    }
    if(tagRecurse) {
       cscope += "R ";
    }
    cscope += QString(" -i/tmp/%1.cscope.files -f/tmp/%2.cscope.out").arg((unsigned long)getpid()).arg((unsigned long)getpid());
    cmds << cscope;

    cmds << QString("ctags --fields=+i -n -R -L /tmp/%1.cscope.files -f /tmp/%2.tags").arg((unsigned long)getpid()).arg((unsigned long)getpid());

    cmds << QString("cqmakedb -s %1 -c /tmp/%2.cscope.out -t /tmp/%3.tags -p").arg(outName_).arg((unsigned long)getpid()).arg((unsigned long)getpid());

    int sz = cmds.size();
    for(int i = 0; i < sz; i++) {
        int prog = 100 * (i + 1) / sz;

        int rc = system(cmds[i].toUtf8().data());
        if (rc != 0) {
            emit error();
            return;
        }
        emit progress(prog);
    }
    QFile::remove(QString("/tmp/%1.cscope.files").arg((unsigned long)getpid()));
    QFile::remove(QString("/tmp/%1.cscope.out").arg((unsigned long)getpid()));
    QFile::remove(QString("/tmp/%1.tags").arg((unsigned long)getpid()));
}

void CmdThread::enableCplusplus(bool en)
{
   tagCplusplus_ = en;
}

void CmdThread::enableJava(bool en)
{

   tagJava_ = en;
}

void CmdThread::enablePython(bool en)
{

    tagPython_ = en;
}

void CmdThread::enableRecurse(bool en)
{
    tagRecurse = en;
}

void CmdThread::enableKernel(bool en)
{
    tagKernel = en;
}

void CmdThread::setOutName(const QString &name)
{
   if(!name.isEmpty()) {
        outName_ = name;
   }
}

void CmdThread::setWorkPath(const QString& path)
{
   if(!path.isEmpty()) {
        workPath_ = path;
   }
}
