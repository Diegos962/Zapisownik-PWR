#ifndef EDYCJA_PROWADZACYCH_H
#define EDYCJA_PROWADZACYCH_H
#include <QDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QComboBox>
#include "kursy_grupy_bloki.h"
#include <QPair>

bool czyNaLiscie(const QString &data, const QList<QPair<QString, Kurs>> &lista);
class EdycjaProwadzacychDialog: public QDialog
{
  Q_OBJECT
 public:
  EdycjaProwadzacychDialog(const QList<QList<Kurs>> &lista,
			   const QList<Prowadzacy> &lista1);
  QList<QList<Kurs>> kursy;
  QList<Prowadzacy> prowadzacy;
  private slots:
    void comboChanged(const QString &text);
    void zapiszClicked();
 private:
    QTableWidget *table;
    void start();
};



#endif
