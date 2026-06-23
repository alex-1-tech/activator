#include "licensegenerator.h"

#include <QCoreApplication>
#include <QDate>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

static QByteArray canonical(QJsonObject obj) {
  QJsonObject sorted;

  QStringList keys = obj.keys();
  keys.sort();

  for (auto &k : keys) {
    sorted[k] = obj[k];
  }

  return QJsonDocument(sorted).toJson(QJsonDocument::Compact);
}

LicenseGenerator::LicenseGenerator(QObject *p) : QObject(p) {}

QByteArray LicenseGenerator::loadPrivateKey() {
  QFile f(QCoreApplication::applicationDirPath() + "/keys/private.pem");

  if (!f.open(QIODevice::ReadOnly))
    return {};

  return f.readAll();
}

QString LicenseGenerator::generate(QString deviceHwid) {

  QByteArray keyData = loadPrivateKey();

  if (keyData.isEmpty())
    return "PRIVATE KEY ERROR";

  BIO *bio = BIO_new_mem_buf(keyData.data(), keyData.size());

  EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);

  BIO_free(bio);

  if (!pkey)
    return "KEY LOAD ERROR";

  QJsonObject payload;
  payload["ver"] = "1.0.0";
  payload["product"] = "Kalmar32";
  payload["company_name"] = "technovotum";
  payload["host_hwid"] = "";
  payload["device_hwid"] = deviceHwid;
  payload["exp"] = "2099-12-31";
  payload["features"] = QJsonObject();

  QByteArray raw = canonical(payload);

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();

  EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, pkey);

  size_t size = 0;

  EVP_DigestSign(ctx, nullptr, &size, (unsigned char *)raw.data(), raw.size());

  QByteArray signature(size, 0);

  EVP_DigestSign(ctx, (unsigned char *)signature.data(), &size,
                 (unsigned char *)raw.data(), raw.size());

  signature.resize(size);

  EVP_MD_CTX_free(ctx);
  EVP_PKEY_free(pkey);

  QByteArray b1 = raw.toBase64(QByteArray::Base64UrlEncoding);

  QByteArray b2 = signature.toBase64(QByteArray::Base64UrlEncoding);

  qDebug() << saveLicense(QString("%1.%2").arg(QString(b1), QString(b2)));

  return QString("%1.%2").arg(QString(b1), QString(b2));
}

bool LicenseGenerator::saveLicense(QString license) {

  QSettings settings("technovotum", "ManualApp");

  settings.setValue("license/license_key", license);

  settings.sync();

  return settings.status() == QSettings::NoError;
}