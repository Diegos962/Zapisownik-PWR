#include "../include/edycja_prowadzacych.h"

EdycjaProwadzacychDialog::EdycjaProwadzacychDialog(const QList<QList<Kurs>> &lista,
						   const QList<Prowadzacy> &lista1)
{
  resize(1200, 500);
  setModal(true);
  setWindowTitle("Edytuj prowadzących");
  table = new QTableWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  QPushButton *zapisz = new QPushButton("Zapisz", this);
  QPushButton *anuluj = new QPushButton("Anuluj", this);
  
  kursy = lista;
  prowadzacy = lista1;
  
  buttonsLayout->addWidget(zapisz);
  buttonsLayout->addWidget(anuluj);
  layout->addWidget(table);
  layout->addLayout(buttonsLayout);
  
  table->setColumnCount(5);
  table->setHorizontalHeaderLabels
    (QStringList() << "Imię i nazwisko" << "Dopasowanie z polwro" << "Ocena" << "Ilość opinii" << "Link");
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  start();
  connect(anuluj, &QPushButton::clicked, this, &QDialog::reject);
  connect(zapisz, &QPushButton::clicked, this, &EdycjaProwadzacychDialog::zapiszClicked);
}

void EdycjaProwadzacychDialog::comboChanged(const QString &text)
{
  QComboBox *obj = static_cast<QComboBox *> (sender());
  QList<int> rows;
  for(int i = 0; i < table->rowCount(); i++)
    {
      QComboBox *tmp = static_cast<QComboBox *>(table->cellWidget(i, 1));
      if(tmp == obj)
	rows.push_back(i);
    }
  bool p = true;
  for(auto a: prowadzacy)
    if(a.nazwa() == text)
      {
	table->item(rows.at(0), 2)->setText(a.ocena());
	table->item(rows.at(0), 3)->setText(a.iloscOpinii());
	table->item(rows.at(0), 4)->setText(a.link());
	p = false;
	break;
      }
  if(p)
    for(int row: rows)
      {
	table->item(row, 2)->setText("0");
	table->item(row, 3)->setText("0");
	table->item(row, 4)->setText("");
      }
}

void EdycjaProwadzacychDialog::zapiszClicked()
{
  for(auto &b: kursy)
    for(auto &a: b)
      for(int i = 0; i < a.prowadzacy().size(); i++)
	{
	  auto x = table->findItems(a.prowadzacy().at(i), Qt::MatchExactly);
	  int row = table->row(x.at(0));
	  a.dane()[i].setNazwa((static_cast<QComboBox *>(table->cellWidget(row, 1)))->currentText());
	  a.dane()[i].setOcena(table->item(row, 2)->text());
	  a.dane()[i].setIloscOpinii(table->item(row, 3)->text());
	  a.dane()[i].setLink(table->item(row, 4)->text());
	}    
  QDialog::accept();
}

void EdycjaProwadzacychDialog::start()
{
  int i = 0;
  QList<QPair<QString, Kurs>> temp;
  for(auto y: kursy)
    for(auto x: y)
      for(int j = 0; j < x.prowadzacy().size(); j++)
	if(!czyNaLiscie(x.prowadzacy().at(j), temp))
	  {
	    table->insertRow(table->rowCount());
	    temp.push_back(QPair<QString, Kurs>(x.prowadzacy().at(j), x));
	    Prowadzacy xxx;
	    if(x.dane().size() > j)
	      xxx = x.dane().at(j);       

	    QTableWidgetItem *tmp = new QTableWidgetItem(x.prowadzacy().at(j));
	    QTableWidgetItem *tmp2= new QTableWidgetItem(xxx.ocena());
	    QTableWidgetItem *tmp3 = new QTableWidgetItem(xxx.iloscOpinii());
	    QTableWidgetItem *tmp4 = new QTableWidgetItem(xxx.link());
	    tmp->setFlags(tmp->flags() & ~Qt::ItemIsEditable);
	    tmp2->setFlags(tmp->flags() & ~Qt::ItemIsEditable);
	    tmp3->setFlags(tmp->flags() & ~Qt::ItemIsEditable);
	    tmp4->setFlags(tmp->flags() & ~Qt::ItemIsEditable);
	    QComboBox *combo = new QComboBox(this);
	    bool p = true;
	    if(xxx.nazwa().size() == 0)
	      {
		combo->addItem("");
		p = false;
	      }
	    if(x.lista().size() > j)
	      {
		for(auto y: x.lista().at(j))
		  combo->addItem(y.nazwa());
	      }
	    if(p)
	      combo->addItem("");
	    int selected = combo->findText(xxx.nazwa(), Qt::MatchExactly);
	    if(selected >= 0)
	      combo->setCurrentIndex(selected);
	    table->setItem(i, 0, tmp);
	    table->setCellWidget(i, 1, combo);
	    table->setItem(i, 2, tmp2);
	    table->setItem(i, 3, tmp3);
	    table->setItem(i, 4, tmp4);    
	    i++;
	    connect(combo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
		    this, &EdycjaProwadzacychDialog::comboChanged);
	  }
  table->resizeColumnsToContents();
}

bool czyNaLiscie(const QString &data, const QList<QPair<QString, Kurs>> &lista)
{
  for(auto z: lista)
    if(z.first == data)
      return true;
  return false;
}
