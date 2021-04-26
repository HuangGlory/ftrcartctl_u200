#include <QCoreApplication>
#include "ftr_ctr3speedctl.h"
#include <sys/types.h>//mkfifo
#include <sys/stat.h>//mkfifo
#include <licensecheck.h>
#include <PlatformParameter.h>
#include <fstream>

#if(1)
#include <QMutex>
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    }

    // 设置输出信息格式
#if(0)
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString strMessage = QString("Message:%1 File:%2  Line:%3  Function:%4  DateTime:%5")
            .arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);
#else
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
    QString strMessage = QString("%1 %2").arg(strDateTime).arg(localMsg.constData());
#endif

    // 输出信息至文件中（读写、追加形式）
    static QFile file(LOG_FILE_NAME);

    if(!file.isOpen() || !file.exists())
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            cout<<LOG_FILE_NAME<<" file open sucessful"<<endl;
        }
        else
        {
            cout<<LOG_FILE_NAME<<" file open faile,Close it"<<endl;
            file.close();
        }
    }

    if(file.isOpen())
    {
        QTextStream stream(&file);
        stream << strMessage << "\r\n";
        file.flush();
    }
    // 解锁
    mutex.unlock();
}
#else
void  qtLogToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const std::string logTypes[] = { "Debug", "Warning", "Critical", "Fatal", "Info" };
    std::cout << "[Qt]  [" << logTypes[type] << "]  " << msg.toLocal8Bit().constData();
#ifndef NDEBUG
    std::cout << "  (" << context.file << ": " << context.line << ", " << context.function;
#endif
    std::cout << std::endl;
}
void initLog()
{
//    QString logDir = "/tmp/ftrCartCtllog";
//    QDir dir;
//    if(!dir.exists(logDir))
//        dir.mkdir(logDir);
    QString logFile = "/tmp/ftrCartCtlLog";
    std::string logPath = logFile.toStdString();

    std::ofstream* _log = new std::ofstream(logPath.c_str());
    if(!_log)
        return;
    // Redirect std iostream
    std::cout.rdbuf(_log->rdbuf());
    std::cerr.rdbuf(_log->rdbuf());
    // Redirect qt logs to stdout, thus to our log file
    qInstallMessageHandler(qtLogToFile);
}
#endif

/*各编码转换
QString -> QByteArray      QString.toUtf8();
QByteArray -> std::string  QByteArray.toStdString();
std::string -> char *        string.date();

//QTextStream 会引起mem leak
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if(LICENSE_USED)
    qDebug()<<"License Used:";
    LicenseCheck *licCheck = new LicenseCheck;
    bool licenseCheckResult = licCheck->CheckLicenseResult();
    qDebug()<<"licensed:"<<licenseCheckResult;
    if(!licenseCheckResult)
    {
       return a.exec();
    }

    delete licCheck;
#else
    qDebug()<<"License Not Used:";
#endif

    umask(0);
    int result = mkfifo(FTRCARTCTL_IN_PIPE_NAME,0777);
    result = mkfifo(FTRCARTCTL_OUT_PIPE_NAME,0777);
#if(RT_LOG_MAINTAIN_USED)
    //int ret = 0;
    qInstallMessageHandler(myMessageOutput);
    //initLog();
#endif
    FTR_CTR3SpeedCtl ftrCartCtl;// = new FTR_CTR3SpeedCtl(nullptr);

    return a.exec();
}
