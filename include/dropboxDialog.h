#ifndef DROPBOXDIALOG_H
#define DROPBOXDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include "dropbox.h"
#include "kursy_grupy_bloki.h"

class dropboxDialog: public QDialog
{
  Q_OBJECT
 public:
  dropboxDialog(Dropbox *db, const QString &path, QWidget *parent);
  QList<Kurs> listaK;
 private:
  QTreeWidget *tree;
  Dropbox *dropbox;
  void dane(const QString &path);
  void ustawWidget(const QString &path, QTreeWidgetItem *parent);
  private slots:
    void addChilds(QTreeWidgetItem *parent, int);
    void pobierz();
};

#endif
