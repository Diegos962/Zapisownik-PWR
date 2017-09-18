#ifndef DROPBOX_H
#define DROPBOX_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QString>
#include <QProgressBar>
#include "../qtdropbox/qdropbox.h"
#include "../qtdropbox/qdropboxfile.h"

class Dropbox: public QObject
{
  Q_OBJECT
 public:
  Dropbox(const QString &key, const QString &secret, QObject* parent = 0);
  QByteArray downloadFile(const QString &file);
  void uploadData(const QString &file, const QByteArray &data);
  QList<QDropboxFileInfo> filterData(QString folder);
  
 private:
  QDropbox *db;
};

#endif
