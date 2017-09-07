#include "../include/login_window.h"

void LoginWindow::GUI_AKZ()
{
  resize(500, 170);
  labelHead->setVisible(false);
  jezyki->setVisible(true);
  wf->setVisible(true);
  inne->setVisible(true);
  download->setVisible(true);
  stacjonarne->setChecked(true);
  stopienI->setChecked(true);
  formGridLayout->addWidget(jezyki, 1, 0);
  formGridLayout->addWidget(wf, 2, 0);
  formGridLayout->addWidget(inne, 3, 0);
  formGridLayout->addWidget(download, 3, 4, Qt::AlignCenter);
  formGridLayout->addWidget(back, 0, 4, Qt::AlignRight);
  boxGridJezyki->addWidget(labelAngielski, 0, 0);
  boxGridJezyki->addWidget(labelNiemiecki, 0, 1);
  boxGridJezyki->addWidget(labelInneJezyki, 0, 2);
  boxGridJezyki->addWidget(AngA, 1, 0);
  boxGridJezyki->addWidget(AngB, 2, 0);
  boxGridJezyki->addWidget(AngC, 3, 0);
  boxGridJezyki->addWidget(NiemA, 1, 1);
  boxGridJezyki->addWidget(NiemB, 2, 1);
  boxGridJezyki->addWidget(NiemC, 3, 1);
  boxGridJezyki->addWidget(InneA, 1, 2);
  boxGridJezyki->addWidget(InneB, 2, 2);
  boxGridJezyki->addWidget(InneC, 3, 2);
  boxGridOgolne->addWidget(stacjonarne, 0, 0);
  boxGridOgolne->addWidget(niestacjonarne, 1, 0);
  boxGridOgolne->addWidget(stopienI, 0, 1);
  boxGridOgolne->addWidget(stopienII, 1, 1);
  boxGridWF->addWidget(sekcja, 0, 0);  
  boxAKZ->addTab(tabOgolne, "OGÓLNE");
  boxAKZ->addTab(tabJezyki, "JĘZYKI");
  boxAKZ->addTab(tabWF, "WF");
  for(int i = 1; i < 3; i++)
    boxAKZ->setTabEnabled(i, false);
  
  boxAKZ->setVisible(true);
  boxAKZ->setStyleSheet("QTabBar::tab { height: 25px;}");
  formGridLayout->addWidget(boxAKZ, 0, 1, 4, 3);

  connect(jezyki, &QCheckBox::stateChanged, this,
	  [=] {if (jezyki->isChecked()) boxAKZ->setTabEnabled(1, true);
      else {boxAKZ->setTabEnabled(1, false);boxAKZ->setCurrentIndex(0);}});
  connect(wf, &QCheckBox::stateChanged, this,
	  [=] {if (wf->isChecked()) boxAKZ->setTabEnabled(2, true);
      else {boxAKZ->setTabEnabled(2, false) ;boxAKZ->setCurrentIndex(0);}});
  connect(back, &QPushButton::clicked, this,
	  [=] {clear_window(); GUI_select_zapisy();});
  connect(download, &QPushButton::clicked,
	  this, &LoginWindow::AKZ_download);
}

void LoginWindow::filtrujJezyki(const Kurs &i)
{
  int pos;
  if( (pos = i.nazwa().indexOf("angielski")) >= 0
      && i.nazwa().indexOf("A", pos) >= 0)
    {
      if(AngA->isChecked())
	lista_kursow.push_back(i);
    }
  else if( (pos = i.nazwa().indexOf("angielski")) >= 0
	   && i.nazwa().indexOf("B", pos) >= 0)
    {
      if(AngB->isChecked())
	lista_kursow.push_back(i);
    }
  else if( (pos = i.nazwa().indexOf("angielski")) >= 0
	   && i.nazwa().indexOf("C", pos) >= 0)
    {
      if(AngC->isChecked())
	lista_kursow.push_back(i);
    }
  else if( (pos = i.nazwa().indexOf("niemiecki")) >= 0
	   && i.nazwa().indexOf("A", pos) >= 0)
    {
      if(NiemA->isChecked())
	lista_kursow.push_back(i);
    }
  else if( (pos = i.nazwa().indexOf("niemiecki")) >= 0
	   && i.nazwa().indexOf("B", pos) >= 0)
    {
      if(NiemB->isChecked())
	lista_kursow.push_back(i);
    }
  else if( (pos = i.nazwa().indexOf("niemiecki")) >= 0
	   && i.nazwa().indexOf("C", pos) >= 0)
    {
      if(NiemC->isChecked())
	lista_kursow.push_back(i);
    }
  else if(i.nazwa().indexOf("A") >= 0)
    {
      if(InneA->isChecked())
	lista_kursow.push_back(i);
    }
  else if(i.nazwa().indexOf("B") >= 0)
    {
      if(InneB->isChecked())
	lista_kursow.push_back(i);
    }
  else if(i.nazwa().indexOf("C") >= 0)
    {
      if(InneC->isChecked())
	lista_kursow.push_back(i);
    }
}

void LoginWindow::filtrujAKZ(QList<Kurs> &tmp)
{
  for(auto i: tmp)
    {
      if(i.stacjonarne().indexOf("Tak") >= 0)
	{
	  if(!stacjonarne->isChecked())
	    continue;
	}
      if(i.stacjonarne().indexOf("Nie") >= 0)
	{
	  if(!niestacjonarne->isChecked())
	    continue;
	}
      if(i.stopien() == "I II")
	{
	  if(stopienII->isChecked() || stopienI->isChecked());
	  else
	    continue;
	}
      else if(i.stopien() == "I")
	{
	  if(!stopienI->isChecked())
	    continue;
	}
      else if(i.stopien() == "II")
	{
	  if(!stopienII->isChecked())
	    continue;
	}
      if(i.kodKursu().at(0) == 'W')
	{
	  if(wf->isChecked())
	    {
	      if(i.nazwa().indexOf("/s") == -1)
		lista_kursow.push_back(i);
	      else
		if(!sekcja->isChecked())
		  lista_kursow.push_back(i);
	    }
	}
      else if(i.kodKursu().at(0) == 'J')
	{
	  if(!jezyki->isChecked())
	    continue;
	  else
	    filtrujJezyki(i);
	}
      else
	if(inne->isChecked())
	  lista_kursow.push_back(i);
    }
}

void LoginWindow::AKZ_download()
{
  boxAKZ->setEnabled(false);
  jezyki->setEnabled(false);
  wf->setEnabled(false);
  inne->setEnabled(false);
  download->setEnabled(false);
  back->setEnabled(false);
  QNetworkReply *reply = get_page("http://www.akz.pwr.edu.pl/katalog_zap.html");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      buttons->setEnabled(true);
      return;
    }
  QList<Kurs> tmp;
  QStringList lastline;
  QString line;
  while(reply->size() != 0)
    {
      line = reply->readLine();
      if(line.indexOf("<tr class=\"gradeX\">") >= 0)
	{
	  Kurs nowy;
	  nowy.setKodKursu(usun_tagi(reply->readLine()).simplified());
	  if(nowy.kodKursu().right(1) == "C")
	    nowy.setForma("Ćwiczenia");
	  else
	    nowy.setForma("Wykład");
	  
	  nowy.setKodGrupy(usun_tagi(reply->readLine()).simplified());
	  nowy.setNazwa(usun_tagi(reply->readLine()).simplified());

	  QString terms = "";
	  terminy(usun_tagi(reply->readLine()).simplified(), terms);
	  QStringList sprawdz = terms.split(";", QString::SkipEmptyParts);
	  terms = "";
	  for(auto &xyz: sprawdz)
	    {
	      xyz += ';';
	      if(sprawdzTerminy(xyz))
	        terms += xyz;
	    }
	  nowy.setTermin(terms);
	  QString tymczas = reply->readLine();
	  QStringList listaProw = tymczas.split("<br/>", QString::SkipEmptyParts);
	  for(auto &opk: listaProw)
	    {
	      opk = usun_tagi(opk).simplified();
	      if(opk.size() == 0)
		listaProw.removeOne(opk);
	    }
	  nowy.setProwadzacy(listaProw);
	  nowy.setMiejsca(usun_tagi(reply->readLine()).simplified());
	  line = reply->readLine();
	  lastline = line.split("</td>");
	  nowy.setStacjonarne(usun_tagi(lastline.at(1)).simplified());
	  nowy.setStopien(usun_tagi(lastline.at(2)).simplified());
	  nowy.setUwagi(usun_tagi(lastline.at(3)).simplified());
	  tmp.push_back(nowy);
	}
    }
  int stan1 = lista_kursow.size();
  filtrujAKZ(tmp);
  labelHead->setText("ZAKOŃCZONO POBIERANIE\nPOBRANO " + QString::number(lista_kursow.size() - stan1) + " KURSÓW");
  labelHead->setStyleSheet("QLabel {color: black; font-size: 20px;}");
  labelHead->setVisible(true);
  boxAKZ->setVisible(false);
  jezyki->setVisible(false);
  wf->setVisible(false);
  inne->setVisible(false);
  download->setVisible(false);
  ActiveWidget(buttonMajor);
  buttonMajor->setText("Zakończ");
  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
	  this, &QDialog::accept);
}
