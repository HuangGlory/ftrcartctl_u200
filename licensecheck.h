#ifndef LICENSECHECK_H
#define LICENSECHECK_H

#include <QObject>
#include <QtNetwork/qnetworkinterface.h>
#include <QFile>

#define LICENSE_NAME                    ("/home/pi/ftrCartCtl/license")

class LicenseCheck : public QObject
{
    Q_OBJECT
public:
    explicit LicenseCheck(QObject *parent = nullptr);

signals:

public:
    QString getMacAddress();
    QString CalcLicense();
    bool CheckLicenseResult();
private:
    QString LicenseStrFromFile;
    QString LicenseStrFromCalc;
};

#endif // LICENSECHECK_H
