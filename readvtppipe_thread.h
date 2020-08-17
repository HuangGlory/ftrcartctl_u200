#ifndef READVTPPIPE_THREAD_H
#define READVTPPIPE_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"
#include <QFile>

class ReadVTPPipe_Thread : public QThread
{
    Q_OBJECT
public:
    ReadVTPPipe_Thread();
    ReadVTPPipe_Thread(QString pipeFileName);
    ~ReadVTPPipe_Thread();

public:
    VTPInfo_t VTPInfo;
    QFile *pipeFile;
    QString pipeFileName;

private:
    virtual void run() override;
    void UpdateInfo(void);
    void SetPipeFileName(QString pipeFileName);

signals:
    void UpdateInfoSignal(VTPInfo_t);

public:
    void stop(void);
    bool isRunable;
};

#endif // READVTPPIPE_THREAD_H
