#include "../include/dropbox.h"
#include <QDebug>

Dropbox::Dropbox(const QString &key, const QString &secret, QObject *parent): QObject(parent)
{
  db = new QDropbox(key, secret, QDropbox::Plaintext, "api.dropbox.com", this);
  db->setToken("vijl8futz5hsxpvc");
  db->setTokenSecret("1hhgsbk08v8rts1");
}

QList<QDropboxFileInfo> Dropbox::filterData(QString folder)
{
  folder = "/dropbox" + folder;
  QDropboxFile df(folder, db);
  QDropboxFileInfo info = df.metadata();
  QList<QDropboxFileInfo> contentInfo = info.contents();
  return contentInfo;
}

QByteArray Dropbox::downloadFile(const QString &file)
{
  QDropboxFile dFile(file, db);
  QByteArray ret;
  if(!dFile.open(QIODevice::ReadOnly))
    {
      qDebug() << "Brak danych na serwerze";
      return ret;
    }
  ret = dFile.readAll();
  dFile.close();
  return ret;
}
void Dropbox::uploadData(const QString &name, const QByteArray &data)
{
  QString full = "/dropbox" + name;
  QDropboxFile destination(full, db);
  if(!destination.open(QDropboxFile::WriteOnly))
    {
      qDebug() << "Nie mogę znaleźć pliku";
      return;
    }
  else
    {
      if(destination.write(data) < 0)
	qDebug() << "UPLOAD ERROR";
    }
  destination.close();
}
