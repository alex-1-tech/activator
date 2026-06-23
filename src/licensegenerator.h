#pragma once

#include <QObject>
#include <QString>

class LicenseGenerator : public QObject {
  Q_OBJECT

public:
  explicit LicenseGenerator(QObject *parent = nullptr);

  Q_INVOKABLE QString generate(QString deviceHwid);
  Q_INVOKABLE bool saveLicense(QString license);

private:
  QByteArray loadPrivateKey();
};