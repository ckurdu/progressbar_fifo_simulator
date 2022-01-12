#ifndef PROGRESSWWORKER_H
#define PROGRESSWWORKER_H

#include <QObject>
#include <interfacenamedpipe.h>
#include <factory.h>
#include "general.h"

/**
 * Brief:
 * This class helps to simulate writing the progress of a working thread into a FIFO file. 
 * To use this object, handler should be set by calling setHandler method. 
 * Handler works in a different thread and can use this class as a library for writing purposes.
 * handler is not thread-safe. Be careful.
 *
 * More:
 * This class is the Qt bridge for the named fifo file handler object for only writing.
 * This class limited the real InterfaceNamedPipe derived class into only write access.
 * Real handler object created by a factory and this class behaves like an interface for every IntarfaceNamedPipe derived object.
 * This class uses InterfaceNamedPipe class for only writing purposes.
 *
 *
 * Architecture Description:
 * This class uses for thread signalling for the main Qt Application thread.
 * Object for this class should be created by a controller class and should be moved to a new thread.
 * In this way, we can start a long-term job in a separate thread and emit signals to the main Qt Application thread.
 */

class ProgressWriterWorker : public QObject
{
    Q_OBJECT
    QString m_file_name = "fifo_pipe";
    bool m_named = false;
    InterfaceNamedPipe *m_ptr_named_pipe = nullptr;
    progress_writer_handler m_handler = nullptr;
    bool m_finishing = false;

public:
    float m_progress = 0.0;
    int m_current = 0;
    int m_total_step = 10;

public:
    explicit ProgressWriterWorker(QObject *parent = nullptr);
    ~ProgressWriterWorker();
    /*  set FIFO file name before calling startJob   */
    void setInputFileName(const QString input, bool named = false);
    /* this handler called from nextstep method. progress_writer_handler do the real job. This class behave like library  */
    void setHandler(progress_writer_handler);
    bool isPipeExist();
    bool isBlocked();
    bool createPipe();
    bool isFinishing();
    /* write content into to the FIFO file in a standard way. non blocking */
    void writePipe(const QString content);

public slots:
    void startJob();
    void nextStep();
    void endJob();

signals:
    void jobEnded();
};

#endif // PROGRESSWWORKER_H
