#include "../include/filtry.h"

FiltryDialog::FiltryDialog(QWidget *parent): QDialog(parent)
{
  buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  buttons->button(QDialogButtonBox::Ok)->setText("Zatwierdź");
  buttons->button(QDialogButtonBox::Cancel)->setText("Anuluj");
  niedostepne = new QCheckBox("Ukryj niedostępne kursy", this);
  miejscaW = new QCheckBox("Wykładowe", this);
  miejscaC = new QCheckBox("Ćwiczeniowe", this);
  miejscaL = new QCheckBox("Laboratoryjne", this);
  miejscaP = new QCheckBox("Projektowe", this);
  miejscaS = new QCheckBox("Seminariów", this);
  miejscaI = new QCheckBox("Innych", this);
  ocena = new QCheckBox("Ukryj kursy z oceną prowadzącego niższą niż", this);
  spinOcena = new QDoubleSpinBox(this);
  dniTygodnia = new QComboBox(this);
  godzP = new MySpinBox(this);
  godzK = new MySpinBox(this);
  minP = new MySpinBox(this);
  minK = new MySpinBox(this);
  dodaj = new QPushButton("DODAJ", this);

  dniTygodniaP = new QComboBox(this);
  godzPP = new MySpinBox(this);
  godzKP = new MySpinBox(this);
  minPP = new MySpinBox(this);
  minKP = new MySpinBox(this);
  dodajP = new QPushButton("DODAJ", this);
  
  layout = new QVBoxLayout(this);
  
  spinOcena->setSingleStep(0.1);
  spinOcena->setRange(2, 5.5);
  spinOcena->setEnabled(false);
  
  dniTygodnia->addItem("Poniedziałek");
  dniTygodnia->addItem("Wtorek");
  dniTygodnia->addItem("Środa");
  dniTygodnia->addItem("Czwartek");
  dniTygodnia->addItem("Piątek");
  
  godzP->setRange(7, 22);
  godzK->setRange(7, 22);
  minP->setRange(0, 55);
  minK->setRange(0, 55);
  minP->setSingleStep(5);
  minK->setSingleStep(5);

  dniTygodniaP->addItem("Poniedziałek");
  dniTygodniaP->addItem("Wtorek");
  dniTygodniaP->addItem("Środa");
  dniTygodniaP->addItem("Czwartek");
  dniTygodniaP->addItem("Piątek");
  
  godzPP->setRange(7, 22);
  godzKP->setRange(7, 22);
  minPP->setRange(0, 55);
  minKP->setRange(0, 55);
  minPP->setSingleStep(5);
  minKP->setSingleStep(5);
  
  QLabel *label = new QLabel("Ukryj pełne grupy:", this);
  QHBoxLayout *miejscaLayout = new QHBoxLayout();
  miejscaLayout->addWidget(miejscaW);
  miejscaLayout->addWidget(miejscaC);
  miejscaLayout->addWidget(miejscaL);
  miejscaLayout->addWidget(miejscaP);
  miejscaLayout->addWidget(miejscaS);
  miejscaLayout->addWidget(miejscaI);
  QHBoxLayout *ocenyLayout = new QHBoxLayout();
  ocenyLayout->addWidget(ocena);
  ocenyLayout->addWidget(spinOcena);
  
  QHBoxLayout *terminyLayout = new QHBoxLayout();
  QHBoxLayout *terminyPLayout = new QHBoxLayout();
  QLabel *label2 = new QLabel("Ukryj zajęcia w terminach:", this);
  QLabel *label3 = new QLabel(":", this);
  QLabel *label4 = new QLabel(" - ", this);
  QLabel *label5 = new QLabel(":", this);
  QLabel *label2P = new QLabel("Pokaż tylko zajęcia w terminach:", this);
  QLabel *label3P = new QLabel(":", this);
  QLabel *label4P = new QLabel(" - ", this);
  QLabel *label5P = new QLabel(":", this);
  
  terminyLayout->addWidget(dniTygodnia);
  terminyLayout->addSpacing(50);
  terminyLayout->addWidget(godzP);
  terminyLayout->addWidget(label3);
  terminyLayout->addWidget(minP);
  terminyLayout->addWidget(label4);
  terminyLayout->addWidget(godzK);
  terminyLayout->addWidget(label5);
  terminyLayout->addWidget(minK);
  terminyLayout->addSpacing(100);
  terminyLayout->addWidget(dodaj);

  terminyPLayout->addWidget(dniTygodniaP);
  terminyPLayout->addSpacing(50);
  terminyPLayout->addWidget(godzPP);
  terminyPLayout->addWidget(label3P);
  terminyPLayout->addWidget(minPP);
  terminyPLayout->addWidget(label4P);
  terminyPLayout->addWidget(godzKP);
  terminyPLayout->addWidget(label5P);
  terminyPLayout->addWidget(minKP);
  terminyPLayout->addSpacing(100);
  terminyPLayout->addWidget(dodajP);
  
  layout->addWidget(niedostepne);
  layout->addWidget(label);
  layout->addLayout(miejscaLayout);
  layout->addLayout(ocenyLayout);
  layout->addWidget(label2);
  layout->addLayout(terminyLayout);
  layout->addWidget(label2P);
  layout->addLayout(terminyPLayout);
  layout->addWidget(buttons);
  
  setLayout(layout);
  setModal(true);
  setWindowTitle("Opcje filtrowania");
  connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(dodaj, &QPushButton::clicked, this, &FiltryDialog::dodajClicked);
  connect(dodajP, &QPushButton::clicked, this, &FiltryDialog::dodajClickedP);
  connect(ocena, &QCheckBox::stateChanged,
	  this, [=](int state){spinOcena->setEnabled(state);});
}

void FiltryDialog::dodajClicked()
{
  int g1 = godzP->value();
  int g2 = godzK->value();
  int m1 = minP->value();
  int m2 = minK->value();
  if(g2 <= g1)
    {
      QMessageBox msg;
      msg.setText("Wybierz szerszy przedział czasowy");
      msg.exec();
      return;
    }
  QHBoxLayout *termin = new QHBoxLayout();
  QString str = QString("%1  %2:%3 - %4:%5").arg(dniTygodnia->currentText()).arg(g1, 2, 10, QChar('0')).arg(m1, 2, 10, QChar('0')).arg(g2, 2, 10, QChar('0')).arg(m2, 2, 10, QChar('0'));
  int index = dniTygodnia->currentIndex();
  QString kod = "2" + QString::number(index) + "0";
  kod += QString("%1%2%3%4").arg(g1, 2, 10, QChar('0')).arg(m1, 2, 10, QChar('0')).arg(g2, 2, 10, QChar('0')).arg(m2, 2, 10, QChar('0'));
  QLabel *lab = new QLabel(str, this);
  QPushButton *button = new QPushButton("USUŃ", this);
  termin->addWidget(lab);
  termin->setSpacing(250);
  termin->addWidget(button);
  layout->insertLayout(6, termin);
  obj.push_back(QPair<QLabel *, QPushButton *>(lab, button));
  wybraneTerminy.push_back(kod);
  connect(button, &QPushButton::clicked, this, &FiltryDialog::usunClicked);
}

void FiltryDialog::usunClicked()
{
  for(int i = 0; i < obj.size(); i++)
    if(obj.at(i).second == sender())
      {
	delete obj[i].first;
	delete obj[i].second;
	obj.removeAt(i);
	wybraneTerminy.removeAt(i);
	break;
      }
}

void FiltryDialog::usunClickedP()
{
  for(int i = 0; i < objP.size(); i++)
    if(objP.at(i).second == sender())
      {
	delete objP[i].first;
	delete objP[i].second;
	objP.removeAt(i);
	wybraneTerminyP.removeAt(i);
	break;
      }
}
void FiltryDialog::dodajClickedP()
{
  int g1 = godzPP->value();
  int g2 = godzKP->value();
  int m1 = minPP->value();
  int m2 = minKP->value();
  if(g2 <= g1)
    {
      QMessageBox msg;
      msg.setText("Wybierz szerszy przedział czasowy");
      msg.exec();
      return;
    }
  QHBoxLayout *termin = new QHBoxLayout();
  QString str = QString("%1  %2:%3 - %4:%5").arg(dniTygodniaP->currentText()).arg(g1, 2, 10, QChar('0')).arg(m1, 2, 10, QChar('0')).arg(g2, 2, 10, QChar('0')).arg(m2, 2, 10, QChar('0'));
  int index = dniTygodniaP->currentIndex();
  QString kod = "2" + QString::number(index) + "0";
  kod += QString("%1%2%3%4").arg(g1, 2, 10, QChar('0')).arg(m1, 2, 10, QChar('0')).arg(g2, 2, 10, QChar('0')).arg(m2, 2, 10, QChar('0'));
  QLabel *lab = new QLabel(str, this);
  QPushButton *button = new QPushButton("USUŃ", this);
  termin->addWidget(lab);
  termin->setSpacing(250);
  termin->addWidget(button);
  layout->insertLayout(layout->count() - 1, termin);
  objP.push_back(QPair<QLabel *, QPushButton *>(lab, button));
  wybraneTerminyP.push_back(kod);
  connect(button, &QPushButton::clicked, this, &FiltryDialog::usunClickedP);
}

bool FiltryDialog::ukryjZajete() const {return niedostepne->isChecked();}
bool FiltryDialog::ukryjPelneW() const {return miejscaW->isChecked();}
bool FiltryDialog::ukryjPelneC() const {return miejscaC->isChecked();}
bool FiltryDialog::ukryjPelneL() const {return miejscaL->isChecked();}
bool FiltryDialog::ukryjPelneP() const {return miejscaP->isChecked();}
bool FiltryDialog::ukryjPelneS() const {return miejscaS->isChecked();}
bool FiltryDialog::ukryjPelneI() const {return miejscaI->isChecked();}
bool FiltryDialog::ukryjOcena() const {return ocena->isChecked();}
float FiltryDialog::ukryjOcenaWartosc() const {return spinOcena->value();}
QList<QString> FiltryDialog::ukryjTerminy() const {return wybraneTerminy;}
QList<QString> FiltryDialog::pokazTerminy() const {return wybraneTerminyP;}
