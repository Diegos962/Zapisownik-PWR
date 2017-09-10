#ifndef FILTRY_H
#define FILTRY_H
#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QList>
#include <QPair>

class MySpinBox: public QSpinBox 
{
  Q_OBJECT
 public:
  MySpinBox(QWidget *parent = 0): QSpinBox(parent){}
  QString textFromValue(int value) const
  {
    return QString("%1").arg(value, 2 , 10, QChar('0'));
  }
};

class FiltryDialog: public QDialog
{
  Q_OBJECT
 public:
  FiltryDialog(QWidget *parent = 0);
  bool ukryjZajete() const;
  bool ukryjPelneW() const;
  bool ukryjPelneC() const;
  bool ukryjPelneL() const;
  bool ukryjPelneP() const;
  bool ukryjPelneS() const;
  bool ukryjPelneI() const;
  bool ukryjOcena() const;
  float ukryjOcenaWartosc() const;
  QList<QString> ukryjTerminy() const;

 private:
  QCheckBox *niedostepne;
  QCheckBox *miejscaW;
  QCheckBox *miejscaC;
  QCheckBox *miejscaL;
  QCheckBox *miejscaP;
  QCheckBox *miejscaS;
  QCheckBox *miejscaI;
  QCheckBox *ocena;
  QDoubleSpinBox *spinOcena;
  QComboBox *dniTygodnia;
  MySpinBox *godzP;
  MySpinBox *minP;
  MySpinBox *godzK;
  MySpinBox *minK;
  QPushButton *dodaj;
  QDialogButtonBox *buttons;
  QVBoxLayout *layout;
  QList<QPair<QLabel *, QPushButton *>> obj;
  QList<QString> wybraneTerminy;
  private slots:
    void usunClicked();
    void dodajClicked();
};


#endif
