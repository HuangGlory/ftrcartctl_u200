#include "mapinfo.h"
#include <QObject>
#include <QDebug>
#include <QFile>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>

MapInfo::MapInfo()
{
    this->loadMap(MAP_FILE_NAME);
}

void MapInfo::loadMap(QString fileName)
{
    qDebug()<<"loadMap:"<<fileName;

    QFile *file = new QFile(fileName);
    if(!file->isOpen() && file->exists())
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

                QJsonObject::iterator iteratorJson = obj.begin();
                while(iteratorJson != obj.end())
                {
                    StationAttribute stationHead = StationAttribute("0");

                    if(!iteratorJson.key().contains("stations_num"))
                    {
                        QString keyStr = iteratorJson.key();
                        QString valueStr = iteratorJson.value().toString();
                        QStringList keyList = keyStr.split("->");
                        QStringList valueList = valueStr.split(",");

                        if((keyList.size() == 2) && (valueList.size() == 2))
                        {
                            stationHead.SetName(keyList.at(1));
                            quint32 dist = valueList.at(0).toInt();
                            qint16  speed= valueList.at(1).toInt();

                            if(keyList.at(0) == keyList.at(1))//0->0
                            {
                                stationHead.SetBDist(0);
                                stationHead.SetBSpeed(0);

                                stationHead.SetFDist(dist);
                                stationHead.SetFSpeed(speed);

                            }
                            else
                            {
                                StationAttribute preStation = this->mapList.at(this->mapList.size()-1);

                                stationHead.SetBDist(dist);
                                stationHead.SetBSpeed(speed);

                                preStation.SetFDist(dist);
                                preStation.SetFSpeed(speed);

                                this->mapList.replace(this->mapList.size() - 1,preStation);
                            }
                            this->mapList.append(stationHead);
                        }
                        else
                        {
                            qDebug()<<"map err:";
                        }
                    }
                    else if(iteratorJson.key().contains("stations_num"))
                    {                        
                        this->stationsNum = iteratorJson.value().toVariant().toInt();
                    }
                    else if(iteratorJson.key().contains("type"))
                    {
                        this->mapType = (MapType_t)(iteratorJson.value().toVariant().toInt());
                    }
                    iteratorJson++;
                }
            }
        }
    }
    delete file;

    qDebug()<<"Stations:"<<this->stationsNum;
    QList<StationAttribute>::iterator i;//使用只读迭代器
    for(i=this->mapList.begin(); i !=this->mapList.end();i++)
    {
        qDebug()<<i->name<<i->BDist<<i->BSpeed<<i->FDist<<i->FSpeed;

    }
}

MapInfo::~MapInfo()
{

}
