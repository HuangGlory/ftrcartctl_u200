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


signals:
    void UpdateInfoSignal(QString);

public:
    void SetPipeFileName(QString pipeFileName);
    void stop(void);
    bool isRunable;
    //void clearPipe(void)
};

#endif // READINPUTPIPE_THREAD_H
