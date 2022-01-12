#ifndef PROGRESSWOBJECT_H
#define PROGRESSWOBJECT_H

#include <QObject>
#include "progressworker.h"
#include <QThread>
#include "general.h"

/**
 * brief :
 * This class created to connect a ProgressWriterObject in a separate thread into the main QML.
 * ProgressWriterObject uses for creating and controlling a ProgressWriterWorker object.
 * It creates a ProgressWriterWorker object and moves it into to new thread and listens signals of the ProgressWriterWorker object.
 * Then when the progress values is changed, it automatically changes QML object with new values. 
 * 
 * 
 * architecture:
 * This is a controller class for creating, connecting to a worker thread.
 * @link: https://doc.qt.io/qt-5/qthread.html
 */

class ProgressWriterObject : public QObject
{
    Q_OBJECT
    float m_current = 0.0; // current progress percentage
    int m_step = 0;        // current step
    int m_total_step = 0;  // total step
    progress_writer_handler m_handler = nullptr;
    QString m_fifo_name;
public:
    explicit ProgressWriterObject(QObject *parent = nullptr);
    ~ProgressWriterObject();
    float current();
    int totalStep();
    int currentStep();
    void setCurrent(float);
    void setTotalStep(int);
    void setCurrentStep(int);
    void initJob(progress_writer_handler handler, const QString file_name = "fifo_pipe");
    void initJob(std::function<void(ProgressWriterObject *)> handler, const QString file_name = "fifo_pipe");
    Q_INVOKABLE void prepareJob();

signals:
    void currentStepChanged(int);
    void totalStepChanged(int);
    Q_INVOKABLE void startJob();
    Q_INVOKABLE void endJob();
    void jobDone(const QString result);

private:
    QThread workerThread;
};

#endif // PROGRESSWOBJECT_H
