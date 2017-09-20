#ifndef POBOCZNE_H
#define POBOCZNE_H
#include <QStringList>
#include <numeric>

QString DecodeTermin(const QString &ter);
bool sprawdzTerminy(const QString &text);
QString terminy(QString text, QString &ret);
QString godziny(const QString &text, const int &start);
bool znajdzNumery(const QString &line, int &pos1);

#endif
