#include <QCoreApplication>
#include "ftr_ctr3speedctl.h"
#include <sys/types.h>//mkfifo
#include <sys/stat.h>//mkfifo
#include <licensecheck.h>
#include <PlatformParameter.h>

#include <QMutex>
// 输出信息至文件中（读写、追加形式）
QString rtlogName = "";
QFile file;

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

    if(!file.isOpen() || !file.exists())
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            cout<<file.fileName().toStdString()<<" file open sucessful"<<endl;
        }
        else
        {
            cout<<file.fileName().toStdString()<<" file open faile,Close it"<<endl;
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

/*各编码转换
QString -> QByteArray      QString.toUtf8();
QByteArray -> std::string  QByteArray.toStdString();
std::string -> char *        string.date();

//QTextStream 会引起mem leak

jq 使用   jq '.pair_mac="ff:65:90:f1:d9:b8"' settings.json >/tmp/settings.json & sudo cp -f /tmp/settings.json /home/pi/ftrCartCtl/settins.json
         jq 'del(.into_go_zone_dist1)' settings.json >/tmp/settings.json & sudo cp -f /tmp/settings.json /home/pi/ftrCartCtl/settins.json
         jq '.F1="New F1" | .F2="New F2"' t.json
         jq '.pair_mac' settins.json
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
    //create dir
    {
        QDir *dir = new QDir();
        if(!dir->exists("/home/pi/ftrCartCtl/rtlog"))
        {
            dir->mkdir("/home/pi/ftrCartCtl/rtlog");
            cout<<"/home/pi/ftrCartCtl/rtlog not exist,create it!"<<endl;
        }

        delete dir;
    }

    rtlogName = RTLOG_FILE_PATH + QString((QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss"))));
    file.setFileName(rtlogName);

    //int ret = 0;
    qInstallMessageHandler(myMessageOutput);
    //initLog();

    FTR_CTR3SpeedCtl ftrCartCtl;// = new FTR_CTR3SpeedCtl(nullptr);
    ftrCartCtl.setRtlogFileName(rtlogName);
#else
    FTR_CTR3SpeedCtl ftrCartCtl;// = new FTR_CTR3SpeedCtl(nullptr);
#endif
    return a.exec();
}
