#include "progresswriterobject.h"
#include "progresswriterworker.h"

ProgressWriterObject::ProgressWriterObject(QObject *parent)
    : QObject{parent}
{
    workerThread.setObjectName("progressWriterThread");
}

float ProgressWriterObject::current()
{
    return m_current;
}

int ProgressWriterObject::currentStep()
{
    return m_step;
}

int ProgressWriterObject::totalStep()
{
    return m_total_step;
}

void ProgressWriterObject::setCurrent(float current)
{
    m_current = current;
}

void ProgressWriterObject::setTotalStep(int total)
{
    m_total_step = total;
}

void ProgressWriterObject::setCurrentStep(int current)
{
    m_current = current;
}

void ProgressWriterObject::initJob(progress_writer_handler handler, QString file_name)
{
    m_handler = handler;
    m_fifo_name = file_name;
}

void ProgressWriterObject::initJob(std::function<void (ProgressWriterObject *)> handler, const QString file_name)
{
    //@todo : Not implemented yet!
}

void ProgressWriterObject::prepareJob()
{
    ProgressWriterWorker *worker = new ProgressWriterWorker;

    worker->setInputFileName(m_fifo_name);
    if (m_handler != nullptr)
    {
        worker->setHandler(m_handler);
    }

    worker->moveToThread(&workerThread);

    connect(this, &ProgressWriterObject::startJob, worker, &ProgressWriterWorker::startJob);
    connect(this, &ProgressWriterObject::endJob, worker, &ProgressWriterWorker::endJob);
    //connect(worker,&ProgressWriterObject::jobEnded, this, &ProgressWriterObject:jobEndHandler );


    workerThread.start();
}


ProgressWriterObject::~ProgressWriterObject()
{
    emit this->endJob();
    workerThread.wait();
    qDebug()<<"end of writing process"<<endl;
}
