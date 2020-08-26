#ifndef READINPUTPIPE_THREAD_H
#define READINPUTPIPE_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"
#include <QFile>

class ReadInputPipe_Thread : public QThread
{
    Q_OBJECT
public:
    ReadInputPipe_Thread();
    ReadInputPipe_Thread(QString pipeFileName);
    ~ReadInputPipe_Thread();

public:
    QFile *pipeFile;
    QString pipeFileName;

private:
    virtual void run() override;
    void SetPipeFileName(QString pipeFileName);

signals:
    void UpdateInfoSignal(QString);

public:
    void stop(void);
    bool isRunable;
    //void clearPipe(void)
};

#endif // READINPUTPIPE_THREAD_H
