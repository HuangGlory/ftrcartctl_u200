#ifndef CLEARPIPE_THREAD_H
#define CLEARPIPE_THREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDebug>

class ClearPipe_Thread : public QThread
{
    Q_OBJECT
public:
    ClearPipe_Thread();
    ClearPipe_Thread(QString pipeFileName);
    ~ClearPipe_Thread();

public:
    QFile *pipeFile;
    QString pipeFileName;

private:
    virtual void run() override;


public:
    void SetPipeFileName(QString pipeFileName);
    void stop(void);
    bool isRunable;
};

#endif // CLEARPIPE_THREAD_H
