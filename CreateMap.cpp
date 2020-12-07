#include "ftr_ctr3speedctl.h"

void FTR_CTR3SpeedCtl::CreateMapSlot(QString station,quint16 dist,qint16 maxSpeed, bool face,bool isFirstStation)
{
    QFile *file = new QFile(MAP_TEMP_FILE_NAME);
    if(!file->isOpen())
    {
        if(file->open(QIODevice::ReadOnly))
        {
            QByteArray data = file->readAll();
            file->close();

            //使用json文件对象加载字符串
            QJsonParseError json_error;
            QJsonDocument doc=QJsonDocument::fromJson(data,&json_error);

            if(json_error.error != QJsonParseError::NoError)
            {
                qDebug()<<"Setting JSON ERROR!";
                return;
            }

            //判断是否对象
            if(doc.isObject())
            {
                //把json文档转换为json对象
                QJsonObject obj=doc.object();

                QString mapItemStr;
                if(face)
                {
                    mapItemStr = QString("%1->%2").arg(station.toUInt()-1).arg(station);
                    obj.insert("stations_num",obj.value("stations_num").toInt()+1);
                }
                else
                {
                    mapItemStr = QString("%1->%2").arg(station).arg(station.toUInt()+1);
                }
                QJsonValue jsonValue = QString("%1,%2").arg(dist).arg(maxSpeed);

                if(!isFirstStation)
                {
                    obj.insert(mapItemStr,jsonValue);
                }

                doc.setObject(obj);
                file->open(QIODevice::WriteOnly|QIODevice::Truncate);
                file->seek(0);
                file->write(doc.toJson());
                file->flush();
                file->close();

                qDebug()<<"Update map:"<<mapItemStr<<jsonValue.toString();
            }
        }
    }
    delete file;
}
