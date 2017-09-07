#ifndef POBOCZNE_H
#define POBOCZNE_H
#include <QStringList>

QString DecodeTermin(const QString &ter);
bool sprawdzTerminy(const QString &text);
QString terminy(QString text, QString &ret);
QString godziny(const QString &text, const int &start);

#endif
