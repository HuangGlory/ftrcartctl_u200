#ifndef READVTKPIPE_THREAD_H
#define READVTKPIPE_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "motorctrlinfo.h"
#include "PlatformParameter.h"
#include <QFile>

class ReadVTKPipe_Thread : public QThread
{
    Q_OBJECT
public:
    ReadVTKPipe_Thread();
    ReadVTKPipe_Thread(QString pipeFileName);
    ~ReadVTKPipe_Thread();

public:
    VTKInfo_t VTKInfo;
    QFile *pipeFile;
    QString pipeFileName;

private:
    virtual void run() override;
    void UpdateInfo(void);


signals:
    void UpdateInfoSignal(VTKInfo_t);

public:
    void SetPipeFileName(QString pipeFileName);
    void stop(void);
    bool isRunable;
    //void clearPipe(void);
};

#endif // READVTKPIPE_THREAD_H
