#ifndef EDYCJA_KURSOW_H
#define EDYCJA_KURSOW_H

#include <QDialog>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include "kursy_grupy_bloki.h"
#include "poboczneTerminy.h"
#include "poboczneDopasowywanie.h"

class DodajKurs;
class EdycjaKursowDialog: public QDialog
{
  Q_OBJECT
 public:
  EdycjaKursowDialog(const QList<QList<Kurs>> &lista,
		     const QList<Prowadzacy> &lista1);
  QList<QList<Kurs>> stare;
  QList<Kurs> nowe;
  QList<Prowadzacy> prow;
  private slots:
    void dodajButton();
    void usunButton();
 private:
    QTableWidget *aktualne;
    void start();
    void ustawKurs(Kurs a, int i);
};

class DodajKurs: public QDialog
{
  Q_OBJECT
 public:
  DodajKurs(const QList<Prowadzacy> &lista,
	    const QList<QList<Kurs>> &lista1);
  QList<Prowadzacy> prow;
  QList<QList<Kurs>> kursy;
  Kurs nowyKurs() const;
  
 private:
  QLineEdit *kod_kursu;
  QLineEdit *kod_grupy;
  QLineEdit *nazwa;
  QComboBox *forma;
  QLineEdit *prowadzacy;
  QLineEdit *termin;
  QLineEdit *miejsca;
  QLineEdit *potok;
  QComboBox *polwro;
  Kurs dodany;
  void accepted(const QString &terminK);
  
  private slots:
    void findClicked();
    void dodajClicked();
};


#endif
