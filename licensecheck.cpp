#include "licensecheck.h"
#include <QDebug>
#include <QCryptographicHash>

LicenseCheck::LicenseCheck(QObject *parent) : QObject(parent)
{
    this->LicenseStrFromCalc = this->CalcLicense();
    QFile *licenseFile = new QFile(LICENSE_NAME);
    if(licenseFile->exists() && !licenseFile->isOpen())
    {
        if(licenseFile->open(QIODevice::ReadOnly))
        {
            this->LicenseStrFromFile = licenseFile->readAll().replace("\n","");
            licenseFile->close();
            //qDebug()<<this->LicenseStrFromFile;
        }
        else
        {
            qDebug()<<"Open License File Err:";
        }
    }
    else
    {
        qDebug()<<"No License:"<<this->CalcLicense();
    }
}


QString LicenseCheck::getMacAddress()
{
    QString text="";
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if (!(interface.flags() & QNetworkInterface::IsRunning))
        {
          text = interface.hardwareAddress();
          return text;
        }
    }
}

QString LicenseCheck::CalcLicense()
{
     QString macStr=this->getMacAddress();
     if(!macStr.isEmpty())
     {
         QStringList macStrList = macStr.split(":");
         quint8 cnt = (macStrList.at(5).toUInt(0,16) & 0x0F);

         for(quint8 i=0;i<cnt;i++)
         {
             QByteArray bb;
             bb = QCryptographicHash::hash(macStr.toUtf8(), QCryptographicHash::Md5).toHex();
             macStr = bb;
         }
     }
     return macStr;
}

bool LicenseCheck::CheckLicenseResult()
{
    //qDebug()<<this->LicenseStrFromCalc<<this->LicenseStrFromFile<<(this->LicenseStrFromCalc == this->LicenseStrFromFile);
    return (bool)(this->LicenseStrFromCalc == this->LicenseStrFromFile);//QString::compare(this->LicenseStrFromCalc , this->LicenseStrFromFile,Qt::CaseInsensitive);
}
