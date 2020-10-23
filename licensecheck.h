#ifndef LICENSECHECK_H
#define LICENSECHECK_H

#include <QObject>
#include <QtNetwork/qnetworkinterface.h>
#include <QFile>

#define CPUINFO_FILE_NAME           tr("/proc/cpuinfo")
#define MACID_FILE_NAME             tr("/sys/class/net/eth0/address")
#define LICENSE_NAME                    ("/home/pi/ftrCartCtl/license")

class LicenseCheck : public QObject
{
    Q_OBJECT
public:
    explicit LicenseCheck(QObject *parent = nullptr);

signals:

public:
    QString CalcLicense();
    QString getLicence(void);
    bool CheckLicenseResult();
private:
    QString LicenseStrFromFile;
    QString LicenseStrFromCalc;

    QString cpuid;
    QString macid;
    QString licenceRawData;
    QString licence;
};

#endif // LICENSECHECK_H
