#include "progresswriterworker.h"
#include "general.h"
#include <QTimer>
#include <QThread>
#include <QCoreApplication>

ProgressWriterWorker::ProgressWriterWorker(QObject *parent)
    : QObject{parent}
{
}

ProgressWriterWorker::~ProgressWriterWorker()
{
    if (m_ptr_named_pipe != nullptr)
    {
        delete m_ptr_named_pipe;
    }
}

void ProgressWriterWorker::setInputFileName(const QString file_name, bool named)
{
    this->m_file_name = file_name;
    this->m_named = named;
}

void ProgressWriterWorker::setHandler(progress_writer_handler handler)
{
    m_handler = handler;
}


bool ProgressWriterWorker::isPipeExist()
{
    if (this->m_ptr_named_pipe == nullptr)
    {
        return false;
    }

    return this->m_ptr_named_pipe->isExist();
}

bool ProgressWriterWorker::createPipe()
{
    if (this->m_ptr_named_pipe != nullptr)
    {
        delete this->m_ptr_named_pipe;
    }

    this->m_ptr_named_pipe = Factory::createNamedPipeObject(this->m_file_name);

    if (this->m_ptr_named_pipe != nullptr)
    {
        m_ptr_named_pipe->setName(m_file_name);
        return true;
    }

    return false;
}


void ProgressWriterWorker::writePipe(const QString content)
{
    if(!isPipeExist())
    {
        return;
    }

    m_ptr_named_pipe->pipeWrite(content);
}

void ProgressWriterWorker::nextStep()
{
    if (m_handler == nullptr)
    {
        return;
    }
    m_handler(this);
}

void ProgressWriterWorker::startJob()
{
    set_thread_name(m_file_name + " writer thread");
    if(!createPipe())
    {
        qDebug()<<"error creation pipe adapter"<<endl;
        endJob();
        return;
    }
    nextStep();
}

bool ProgressWriterWorker::isFinishing()
{
    return m_finishing;
}

void ProgressWriterWorker::endJob()
{
    qDebug()<<"ending writer job"<<endl;
    m_finishing = true;
    QThread * current = QThread::currentThread();
    current->quit();
}
