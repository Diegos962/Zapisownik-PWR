#include "../include/kursy_grupy_bloki.h"

KursyGrupyBloki::KursyGrupyBloki()
{
  Rodzaj = "";
  Kod = "";
  Nazwa = "";
  Link = "";
}

void KursyGrupyBloki::setLink(const QString &l){Link = l;}
void KursyGrupyBloki::setNazwa(const QString &n){Nazwa = n;}
void KursyGrupyBloki::setRodzaj(const QString &r){Rodzaj = r;}
void KursyGrupyBloki::setKod(const QString &k){Kod = k;}

QString KursyGrupyBloki::link() const{return Link;}
QString KursyGrupyBloki::nazwa() const{return Nazwa;}
QString KursyGrupyBloki::rodzaj() const{return Rodzaj;}
QString KursyGrupyBloki::kod() const{return Kod;}

Prowadzacy::Prowadzacy()
{
  Nazwa = "";
  Link = "";
  Ocena = "";
  ilosc_opinii = "";
  Match = 0;
}

void Prowadzacy::setNazwa(const QString &n){Nazwa = n;}
void Prowadzacy::setLink(const QString &l){Link = l;}
void Prowadzacy::setOcena(const QString &o){Ocena = o;}
void Prowadzacy::setIloscOpinii(const QString &iO){ilosc_opinii = iO;}
void Prowadzacy::setMatch(const float &m){Match = m;}

QString Prowadzacy::nazwa() const{return Nazwa;}
QString Prowadzacy::link() const{return Link;}
QString Prowadzacy::ocena() const{return Ocena;}
QString Prowadzacy::iloscOpinii() const{return ilosc_opinii;}
float Prowadzacy::match() const{return Match;}

Kurs::Kurs()
{
  Kod_kursu = "";
  Kod_grupy = "";
  Nazwa = "";
  Forma = "";
  Termin = "";
  Miejsca = "";
  Potok = "";
  Stacjonarne = "";
  Stopien = "";
  Uwagi = "";
  Lokalizacja = "";
}

void Kurs::setKodKursu(const QString &k){Kod_kursu = k;}
void Kurs::setKodGrupy(const QString &k){Kod_grupy = k;}
void Kurs::setNazwa(const QString &k){Nazwa = k;}
void Kurs::setForma(const QString &k){Forma = k;}
void Kurs::setProwadzacy(const QStringList &k){ProwadzacyList = k;}
void Kurs::setTermin(const QString &k){Termin = k;}
void Kurs::setMiejsca(const QString &k){Miejsca = k;}
void Kurs::setPotok(const QString &k){Potok = k;}
void Kurs::setStacjonarne(const QString &k){Stacjonarne = k;}
void Kurs::setStopien(const QString &k){Stopien = k;}
void Kurs::setUwagi(const QString &k){Uwagi = k;}
void Kurs::setLokalizacja(const QString &k){Lokalizacja = k;}

QString Kurs::kodKursu() const {return Kod_kursu;}
QString Kurs::kodGrupy() const {return Kod_grupy;}
QString Kurs::nazwa() const {return Nazwa;}
QString Kurs::forma() const {return Forma;}
QStringList& Kurs::prowadzacy() {return ProwadzacyList;}
QString Kurs::termin() const {return Termin;}
QString Kurs::miejsca() const {return Miejsca;}
QString Kurs::potok() const {return Potok;}
QString Kurs::stacjonarne() const {return Stacjonarne;}
QString Kurs::stopien() const {return Stopien;}
QString Kurs::uwagi() const {return Uwagi;}
QString Kurs::lokalizacja() const {return Lokalizacja;}
QList<Prowadzacy>& Kurs:: dane(){return Dane;}
QList<QList<Prowadzacy>>& Kurs::lista(){return Lista;}
