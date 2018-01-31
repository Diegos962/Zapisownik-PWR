#ifndef KURSY_GRUPY_BLOKI_H
#define KURSY_GRUPY_BLOKI_H
#include <QString>
#include <QList>

class KursyGrupyBloki
{
 private:
  QString Rodzaj;
  QString Kod;
  QString Nazwa;
  QString Link;
 public:
  KursyGrupyBloki();
  void setRodzaj(const QString &r);
  void setNazwa(const QString &n);
  void setKod(const QString &k);
  void setLink(const QString &l);
  QString rodzaj() const;
  QString nazwa() const;
  QString link() const;
  QString kod() const;
};

class Prowadzacy
{
 private:
  QString Nazwa;
  QString Link;
  QString Ocena;
  QString ilosc_opinii;
  float Match;
 public:
  Prowadzacy();
  void setNazwa(const QString &n);
  void setLink(const QString &l);
  void setOcena(const QString &o);
  void setIloscOpinii(const QString &iO);
  void setMatch(const float &m);
  QString nazwa() const;
  QString link() const;
  QString ocena() const;
  QString iloscOpinii() const;
  float match() const;
};

class Kurs
{
 private:
  QString Kod_kursu;
  QString Kod_grupy;
  QString Nazwa;
  QString Forma;
  QStringList ProwadzacyList;
  QString Termin;
  QString Miejsca;
  QString Potok;
  QString Stacjonarne;
  QString Stopien;
  QString Uwagi;
  QString Lokalizacja;
  QList<Prowadzacy> Dane;
  QList<QList<Prowadzacy>> Lista;
 public:
  Kurs();
  void setKodKursu(const QString &k);
  void setKodGrupy(const QString &k);
  void setNazwa(const QString &k);
  void setForma(const QString &k);
  void setProwadzacy(const QStringList &k);
  void setTermin(const QString &k);
  void setMiejsca(const QString &k);
  void setPotok(const QString &k);
  void setStacjonarne(const QString &k);
  void setStopien(const QString &k);
  void setUwagi(const QString &k);
  void setLokalizacja(const QString &k);
  QString kodKursu() const;
  QString kodGrupy() const;
  QString nazwa() const;
  QString forma() const;
  QStringList &prowadzacy();
  QString termin() const;
  QString miejsca() const;
  QString potok() const;
  QString stacjonarne() const;
  QString stopien() const;
  QString uwagi() const;
  QString lokalizacja() const;
  QList<Prowadzacy> &dane();
  QList<QList<Prowadzacy>> &lista();
};

#endif
