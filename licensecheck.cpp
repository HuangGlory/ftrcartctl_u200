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
        qDebug()<<"No License:";
    }
}

QString LicenseCheck::CalcLicense()
{
    QFile *MACIDFile = new QFile(MACID_FILE_NAME);
    QFile *CPUIDFile = new QFile(CPUINFO_FILE_NAME);

    if(MACIDFile->exists() && CPUIDFile->exists())
    {
        //get MAC address
        if(MACIDFile->open(QIODevice::ReadOnly))
        {
            this->macid = MACIDFile->readLine().toUpper().replace("\n","");
            MACIDFile->close();
        }
        else
        {
            qDebug()<<"Read MACID Err:";
        }

        //get CPU ID
        if(CPUIDFile->open(QIODevice::ReadOnly))
        {
            do
            {
                QString lineStr = CPUIDFile->readLine();
                if(lineStr.contains("serial",Qt::CaseInsensitive))
                {
                    this->cpuid = lineStr.replace("Serial","",Qt::CaseInsensitive).simplified().replace(":","").toUpper().simplified();
                    break;
                }
            }while(1);
        }
        else
        {
            qDebug()<<"Read CPUID Err:";
        }
        this->licenceRawData.append(this->cpuid).append(":").append(this->macid);
        this->getLicence();
    }
    else
    {
        qDebug()<<"Licence Calc Err:";
    }

    delete MACIDFile;
    delete CPUIDFile;

     return this->licence;
}

QString LicenseCheck::getLicence(void)
{
    this->licence = this->licenceRawData;

    if(!this->licenceRawData.isEmpty())
    {
        QStringList strList = this->licenceRawData.split(":");
        quint8 cnt = (strList.at(5).toUInt(0,16) & 0x0F);
        //qDebug()<<cnt<<this->licence;
        for(quint8 i=0;i<cnt;i++)
        {
            QByteArray bb;
            bb = QCryptographicHash::hash(this->licence.toUtf8(), QCryptographicHash::Md5).toHex();
            this->licence = bb;
        }
    }
    else
    {
        qDebug()<<"Empty Err:";
    }
    return this->licence;
}

bool LicenseCheck::CheckLicenseResult()
{
    //qDebug()<<this->LicenseStrFromCalc<<this->LicenseStrFromFile<<(this->LicenseStrFromCalc == this->LicenseStrFromFile);
    return (bool)(this->LicenseStrFromCalc == this->LicenseStrFromFile);//QString::compare(this->LicenseStrFromCalc , this->LicenseStrFromFile,Qt::CaseInsensitive);
}
