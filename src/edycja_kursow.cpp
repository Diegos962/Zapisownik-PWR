#include "../include/edycja_kursow.h"

EdycjaKursowDialog::EdycjaKursowDialog(const QList<QList<Kurs>> &lista,
				       const QList<Prowadzacy> &lista1)
{
  resize(1200, 500);
  setModal(true);
  setWindowTitle("Edytor bazy kursów");
  QVBoxLayout *Vlayout = new QVBoxLayout(this);
  QHBoxLayout *layoutButtons = new QHBoxLayout();
  aktualne = new QTableWidget(this);
  QPushButton *usun = new QPushButton("Usuń", this);
  QPushButton *dodaj = new QPushButton("Dodaj", this);
  QPushButton *anuluj = new QPushButton("Anuluj", this);
  QPushButton *zapisz = new QPushButton("Zapisz", this);
  
  stare = lista;
  prow = lista1;
  
  layoutButtons->addWidget(usun);
  layoutButtons->addWidget(dodaj);
  layoutButtons->addStretch(100);
  layoutButtons->addWidget(zapisz);
  layoutButtons->addWidget(anuluj);
  Vlayout->addWidget(aktualne);
  Vlayout->addLayout(layoutButtons);

  aktualne->setColumnCount(7);
  aktualne->setHorizontalHeaderLabels
    (QStringList() << "Kod kursu" << "Kod grupy" << "Nazwa" << "Forma zajęć" << "Prowadzący" << "Prowadzący z polwro" << "Termin");
  aktualne->setSelectionBehavior(QAbstractItemView::SelectRows);
  aktualne->setEditTriggers(QAbstractItemView::NoEditTriggers);

  start();
  connect(usun, &QPushButton::clicked, this, &EdycjaKursowDialog::usunButton);
  connect(dodaj, &QPushButton::clicked, this, &EdycjaKursowDialog::dodajButton);
  connect(anuluj, &QPushButton::clicked, this, &QDialog::reject);
  connect(zapisz, &QPushButton::clicked, this, &QDialog::accept);
}

void EdycjaKursowDialog::ustawKurs(Kurs a, int i)
{
  QTableWidgetItem *tmp = new QTableWidgetItem(a.kodKursu());
  QTableWidgetItem *tmp1 = new QTableWidgetItem(a.kodGrupy());
  QTableWidgetItem *tmp2= new QTableWidgetItem(a.nazwa());
  QTableWidgetItem *tmp3 = new QTableWidgetItem(a.forma());
  QString prowadzacy = "";
  if(a.prowadzacy().size() > 0)
    prowadzacy = a.prowadzacy().at(0);
  for(int i = 1; i < a.prowadzacy().size(); i++)
    prowadzacy += "   " + a.prowadzacy().at(i);
  QString polwro = "";
  if(a.dane().size() > 0)
    polwro = a.dane().at(0).nazwa();
  for(int i = 1; i < a.dane().size(); i++)
    polwro += "   " + a.dane().at(i).nazwa();
  QTableWidgetItem *tmp4 = new QTableWidgetItem(prowadzacy);
  QTableWidgetItem *tmp5 = new QTableWidgetItem(polwro);
  QTableWidgetItem *tmp6 = new QTableWidgetItem(DecodeTermin(a.termin()));
  aktualne->setItem(i, 0, tmp);
  aktualne->setItem(i, 1, tmp1);
  aktualne->setItem(i, 2, tmp2);
  aktualne->setItem(i, 3, tmp3);
  aktualne->setItem(i, 4, tmp4);
  aktualne->setItem(i, 5, tmp5);
  aktualne->setItem(i, 6, tmp6);  
}

void EdycjaKursowDialog::start()
{
  int i = 0;
  int size = 0;
  for(int i = 0; i < stare.size(); i++)
    for(int j = 0; j < stare.at(i).size(); j++)
      size++;
  aktualne->setRowCount(size);
  for(auto b: stare)
    for(auto a: b)
      {
	ustawKurs(a, i);
	i++;
      }
  aktualne->resizeColumnsToContents();
}

void EdycjaKursowDialog::dodajButton()
{
  DodajKurs nowy(prow, stare);
  nowy.show();
  int val = nowy.exec();
  if(val)
    {
      Kurs x = nowy.nowyKurs();
      nowe.push_back(x);
      int i = aktualne->rowCount();
      aktualne->insertRow(i);
      ustawKurs(x, i);
      aktualne->setCurrentItem(aktualne->item(i, 0));
    }
}

void EdycjaKursowDialog::usunButton()
{
  auto tmp = aktualne->selectedItems();
  int size = tmp.size();
  if(size == 0)
    return;
  for(int k = 1; k < tmp.size(); k+=7)
    {
      for(auto &a: stare)
	for(int i = 0; i < a.size(); i++)
	  {
	    if(a[i].kodGrupy() == tmp.at(k)->text()
	       && a[i].kodKursu() == tmp.at(k-1)->text())
	      {
		a.removeAt(i);
		break;
	      }
	  }
    }
  for(int k = 1; k < tmp.size(); k+=7)
    {
      for(int i = 0; i < nowe.size(); i++)
	if(nowe[i].kodGrupy() == tmp.at(1)->text()
	   && nowe[i].kodKursu() == tmp.at(0)->text())
	  {
	    nowe.removeAt(i);
	    break;
	  }
    }
  QSet<int> selectedRows;
  for(auto item: tmp)
    selectedRows.insert(item->row());
  QList<int> rows = selectedRows.toList();
  qSort(rows.begin(), rows.end(), qGreater<int>());
  foreach(int row, rows)
    aktualne->removeRow(row);
}

bool unikat(const QString &kod, const QList<QList<Kurs>> &lista)
{
  for(auto a: lista)
    for(auto b: a)
      if(kod == b.kodGrupy())
	return false;
  return true;
}

DodajKurs::DodajKurs(const QList<Prowadzacy> &lista,
		     const QList<QList<Kurs>> &lista1)
{
  prow = lista;
  kursy = lista1;
  setModal(true);
  setWindowTitle("Dodaj kurs");
  QVBoxLayout *layout = new QVBoxLayout(this);
  kod_kursu = new QLineEdit(this);
  kod_kursu->setPlaceholderText("Kod kursu");
  kod_grupy = new QLineEdit(this);
  kod_grupy->setPlaceholderText("Kod grupy");
  nazwa = new QLineEdit(this);
  nazwa->setPlaceholderText("Nazwa");
  prowadzacy = new QLineEdit(this);
  prowadzacy->setPlaceholderText("Prowadzący");
  termin = new QLineEdit(this);
  termin->setPlaceholderText("Termin");
  miejsca = new QLineEdit(this);
  miejsca->setPlaceholderText("Ilość wolnych miejsc");
  potok = new QLineEdit(this);
  potok->setPlaceholderText("Potok");
  forma = new QComboBox(this);
  forma->addItem("Wykład");
  forma->addItem("Cwiczenia");
  forma->addItem("Laborki");
  forma->addItem("Projekt");
  forma->addItem("Seminarium");
  forma->addItem("Inne");
  
  polwro = new QComboBox(this);
  QPushButton *find = new QPushButton("Dopasuj prowadzącego z polwro", this);
  QHBoxLayout *pLayout = new QHBoxLayout();
  pLayout->addWidget(polwro);
  pLayout->addWidget(find);
  QPushButton *dodaj = new QPushButton("Dodaj", this);
  QPushButton *anuluj = new QPushButton("Anuluj", this);
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(dodaj);
  buttonLayout->addWidget(anuluj);  
  layout->addWidget(kod_kursu);
  layout->addWidget(kod_grupy);
  layout->addWidget(nazwa);
  layout->addWidget(forma);
  layout->addWidget(prowadzacy);
  layout->addLayout(pLayout);
  layout->addWidget(termin);
  layout->addWidget(miejsca);
  layout->addWidget(potok);
  layout->addLayout(buttonLayout);
  
  setFixedSize(width(), height());
  connect(anuluj, &QPushButton::clicked, this, &QDialog::reject);
  connect(dodaj, &QPushButton::clicked, this, &DodajKurs::dodajClicked);
  connect(find, &QPushButton::clicked, this, &DodajKurs::findClicked);
}

void DodajKurs::findClicked()
{
  for(int i = 0; i < polwro->count();)
    polwro->removeItem(0);
  if(dodany.prowadzacy().size() == 0)
    dodany.prowadzacy() << prowadzacy->text();
  else
    dodany.prowadzacy()[0] = prowadzacy->text();
  dodany.lista().clear();
  dopasuj(dodany, prow);
  for(auto a: dodany.lista())
    for(auto x: a)
      polwro->addItem(x.nazwa());
  polwro->addItem("");
}

void DodajKurs::dodajClicked()
{
  bool neg = false;
  QString terminK = "";
  terminy(termin->text(), terminK);
  QStringList sprawdz = terminK.split(";", QString::SkipEmptyParts);
  terminK = "";
  for(auto &xyz: sprawdz)
    {
      xyz += ';';
      if(sprawdzTerminy(xyz))
	terminK += xyz;
      else
	neg = true;
    }
  bool ok;
  miejsca->text().toInt(&ok);
  if(terminK.size() == 0)
    neg = true;
  if(kod_kursu->text().right(1) != forma->currentText().left(1))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", "Niezgodność kodu kursu i formy zajeć");
    }
  else if(neg)
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", "Niepoprawny termin zajęć");
    }
  else if(!unikat(kod_grupy->text(), kursy))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", "Podany kod grupy już istnieje");
    }
  else if(!ok)
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", "Błędna ilość miejsc");
    }
  else if(kod_kursu->text().size() != 0 && kod_grupy->text().size() != 0
	  && nazwa->text().size() != 0 && prowadzacy->text().size() != 0
	  && termin->text().size() != 0 && miejsca->text().size() != 0)
    accepted(terminK);
  else
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", "Wypełnij wszystkie pola");
    }
}

void DodajKurs::accepted(const QString &terminK)
{
  dodany.setKodKursu(kod_kursu->text());
  dodany.setKodGrupy(kod_grupy->text());
  dodany.setNazwa(nazwa->text());
  if(forma->currentText() == "Laborki")
    dodany.setForma("Zajęcia laboratiryjne");
  else
    dodany.setForma(forma->currentText());
  dodany.prowadzacy() << (prowadzacy->text());
  dodany.setTermin(terminK);
  dodany.setMiejsca(miejsca->text());
  dodany.setPotok(potok->text());
  if(polwro->currentIndex() >= 0)
    if(dodany.lista().size() > 0)
      {
	dodany.dane().push_back(dodany.lista().at(0).at(polwro->currentIndex()));
	dodany.prowadzacy()[0] = dodany.dane()[0].nazwa();
      }
  QDialog::accept();
}

Kurs DodajKurs::nowyKurs() const
{
  return dodany;
}
