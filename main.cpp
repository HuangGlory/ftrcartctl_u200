#include <QCoreApplication>
#include "ftr_ctr3speedctl.h"

#if(0)
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
    QString strMessage = QString("%1:%2").arg(strDateTime).arg(localMsg.constData());
#endif
//    QTextStream stream(&this.logFile);
//    stream<<strMessage<<"\r\n";

    // 输出信息至文件中（读写、追加形式）
    QString logFileNameStr=LOG_PATH_NAME;
    logFileNameStr.append(QDateTime::currentDateTime().toString(QString("yyyyMMdd-HH-mm-ss")));

    static QFile file(logFileNameStr);

    if(!file.isOpen())
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            cout<<"file open sucessful"<<endl;
        }
        else
        {
            cout<<"file open faile"<<endl;
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
    //int ret = 0;
    //qInstallMessageHandler(myMessageOutput);
    FTR_CTR3SpeedCtl ftrCartCtl;// = new FTR_CTR3SpeedCtl(nullptr);

    //delete ftrCartCtl;
    //ret = a.exec();
    //delete ftrCartCtl;
    return a.exec();
}
