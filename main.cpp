#include <QCoreApplication>
#include <progresswriterworker.h>
#include <progresswriterobject.h>
#include "general.h"
#include <QTimer>

void named_pipe_progress_writer_handler(ProgressWriterWorker *ptr_pw);

void named_pipe_progress_writer_handler(ProgressWriterWorker *ptr_pw)
{
    show_thread();
    int step = ptr_pw->m_current;
    int total_step = ptr_pw->m_total_step;

    if (!ptr_pw->isPipeExist())
    {
        if (!ptr_pw->isFinishing())
        {
            // not possible to write. Wait for fifo pipe readiness.
            QTimer::singleShot(1000, ptr_pw, &ProgressWriterWorker::nextStep);
            return;
        }
        // nothing to do. Thread is finishing.
        return;
    }

    if (step > total_step || ptr_pw->isFinishing())
    {
        ptr_pw->writePipe("finished\n");
        ptr_pw->endJob();
        return;
    }

    QString line = QString("Step %1 of 10\n").arg(step);
    ptr_pw->writePipe(line);
    QThread::sleep(5);
    qDebug() << "Piped file content writed: " << line << endl;
    ptr_pw->m_current++;
    QTimer::singleShot(1000, ptr_pw, &ProgressWriterWorker::nextStep);
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ProgressWriterObject prog_writer_obj;
    prog_writer_obj.initJob(named_pipe_progress_writer_handler, "fifo_pipe");
    prog_writer_obj.prepareJob();

    QMetaObject::invokeMethod( &prog_writer_obj, "startJob", Qt::QueuedConnection );
    return a.exec();
}
