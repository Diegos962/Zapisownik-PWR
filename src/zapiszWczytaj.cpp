#include "../include/main_window.h"

void Kolizje(bool &all, int &val, Kurs &a, Kurs tmp, bool &kolizja)
{
  if(!all)
    {
      QMessageBox msg;
      msg.setStandardButtons(QMessageBox::No | QMessageBox::NoToAll | QMessageBox::Yes | QMessageBox::YesToAll);
      msg.button(QMessageBox::No)->setText("Nie");
      msg.button(QMessageBox::NoToAll)->setText("Nie na wszystkie");
      msg.button(QMessageBox::Yes)->setText("Tak");
      msg.button(QMessageBox::YesToAll)->setText("Tak na wszystkie");
      val = msg.question(nullptr, "Kolizja",
			 QString("Kurs %1 już znajduje się na liście.\nNadpisać?").arg(tmp.kodGrupy()), msg.standardButtons());
    }
  if(val == QMessageBox::Yes)
    a = tmp;
  else if(val == QMessageBox::YesToAll)
    {
      all = true;
      val = QMessageBox::Yes;
      a = tmp;
    }
  else if(val == QMessageBox::NoToAll)
    all = true;
  kolizja = true;
}

void Window::zapiszPlan()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Zapisz plan", QDir::currentPath(), "Pliki planu (*.pln)");
  if (!fileName.endsWith(".pln"))
    fileName += ".pln";
  if(fileName.size() == 0)
    return;
  QFile file(fileName);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
      return;
    }
  QTextStream out(&file);
   for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = parent->child(j);
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  if(box->isChecked())
	    out << child->text(1) << '\n';
	}
    }
}

void Window::wczytajPlan()
{
  auto fileName = QFileDialog::getOpenFileName(this, "Wybierz plan do wczytania", QDir::currentPath(), "Pliki planu (*.pln)");
  if(fileName.size() == 0)
    return;
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
      return;
    }
  QList<QString> kody;
  QTextStream in(&file);
  while(!in.atEnd())
    kody.push_back(in.readLine());
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = parent->child(j);
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  for(auto x: kody)
	    if(child->text(1) == x)
	      {
		if(!box->isChecked())
		  box->animateClick();
		kody.removeOne(x);
		break;
	      }
	}
    }
}

void Window::wczytajPlanKolegi()
{
  auto fileName = QFileDialog::getOpenFileName(this, "Wybierz plan kolegi do wczytania", QDir::currentPath(), "Pliki planu (*.pln)");
  if(fileName.size() == 0)
    return;
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
      return;
    }
  QList<QString> kody;
  QTextStream in(&file);
  while(!in.atEnd())
    kody.push_back(in.readLine());
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = parent->child(j);
	  for(auto x: kody)
	    if(child->text(1) == x)
	      {
		for(int k = 1; k < 8; k++)
		  child->setBackground(k, QBrush(QColor(0, 0, 255, 127)));
		kody.removeOne(x);
		break;
	      }
	}
    }
}

void Window::wczytajProwadzacychZPliku()
{
  QList<Prowadzacy> temp;
  auto fileName = QFileDialog::getOpenFileName(this, "Wybierz plik z prowadzącymi do wczytania", QDir::currentPath(), "Pliki prowadzących (*.prw)");
  if(fileName.size() == 0)
    return;
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
    }
  else
    {
      QTextStream in(&file);
      int lines = 0;
      while(!in.atEnd())
	{
	  in.readLine();
	  lines++;
	}
      if(lines%4 != 0)
	{
	  QMessageBox msg;
	  msg.critical(this, "Błąd", QString("Błąd odczytu: pliku:\n%1").arg(file.fileName()));
	}
      else
	{
	  in.seek(0);
	  while(!in.atEnd())
	    {
	      Prowadzacy tmp;
	      tmp.setNazwa(in.readLine());
	      tmp.setOcena(in.readLine());
	      tmp.setIloscOpinii(in.readLine());
	      tmp.setLink(in.readLine());		  
	      temp.push_back(tmp);
	    }
	}
    }
  if(temp.size() != 0)
    {
      prow = temp;
      if(przedmioty.size() != 0)
	{
	  QMessageBox msg;
	  int ret = msg.question(this, "Informacja", QString("Wczytano: %1 prowadzących.\nDopasować do kursów?").arg(temp.size()));
	  if(ret == QMessageBox::Yes)
	    {
	      for(auto &a: przedmioty)
		przypisz_prowadzacego(a);
	      update_tree();
	    }
	}
    }
}

void Window::wczytajProwadzacychZPolwro()
{
  LoginWindow l;
  l.PolwroGUI();
  l.show();
  int ret = l.exec();	    
  if(ret)
    if(l.lista_prowadzacych.size() != 0)
      {
	prow = l.lista_prowadzacych;
	if(przedmioty.size() != 0)
	  {
	    QMessageBox msg;
	    int retx = msg.question(this, "Informacja", QString("Wczytano: %1 prowadzących.\nDopasować do kursów?").arg(prow.size()));
	    if(retx == QMessageBox::Yes)
	      {
		for(auto &a: przedmioty)
		  przypisz_prowadzacego(a);
		update_tree();
	      }
	  }
      }
}

void Window::wczytajKursyZPliku()
{
  bool load = false;
  QList<Kurs> temp;
  auto files = QFileDialog::getOpenFileNames(this, "Wybierz pliki z kursami do wczytania", QDir::currentPath(), "Pliki kursów (*.krs)");
  for(auto x: files)
    {
      if(x.size() == 0)
	continue;
      QFile file(x);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	  QMessageBox msg;
	  msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
	}
      else
	{
	  QTextStream in(&file);
	  int lines = 0;
	  while(!in.atEnd())
	    {
	      in.readLine();
	      lines++;
	    }
	  if(lines%8 != 0)
	    {
	      QMessageBox msg;
	      msg.critical(this, "Błąd", QString("Błąd odczytu: pliku:\n%1").arg(file.fileName()));
	    }
	  else
	    {
	      load = true;
	      in.seek(0);
	      bool all = false;
	      int val = 0;
	      while(!in.atEnd())
		{
		  bool kolizja = false;
		  Kurs tmp;
		  tmp.setKodKursu(in.readLine());
		  tmp.setKodGrupy(in.readLine());
		  tmp.setNazwa(in.readLine());
		  tmp.setForma(in.readLine());
		  tmp.setProwadzacy(in.readLine().split("||", QString::SkipEmptyParts));
		  tmp.setTermin(in.readLine());
		  tmp.setMiejsca(in.readLine());
		  tmp.setPotok(in.readLine());
		  for(auto b: przedmioty)
		  for(auto &a: b)
		    if(a.kodGrupy() == tmp.kodGrupy())
		      {
			Kolizje(all, val, a, tmp, kolizja);
			break;
		      }
		  if(!kolizja)		  
		    temp.push_back(tmp);
		}
	    }
	}
    }
  if(load)
    {
      if(prow.size() != 0 && temp.size() != 0)
	{
	  QMessageBox msg;
	  int ret = msg.question(this, "Informacja", QString("Wczytano: %1 kursów.\nDopasować prowadzących z polwro?").arg(temp.size()));
	  if(ret == QMessageBox::Yes)
	    przypisz_prowadzacego(temp);
	}
      kursy += temp;
      reload();
    }
}

void Window::wczytajKursyZEdukacji()
{
  LoginWindow l;
  l.GUI_login();
  l.show();
  int ret = l.exec();	    
  if(ret)
    WczytywanieEduAKZ(l.lista_kursow);
}

void Window::wczytajKursyZAKZ()
{
  LoginWindow l;
  l.GUI_AKZ();
  l.show();
  int ret = l.exec();	    
  if(ret)
    WczytywanieEduAKZ(l.lista_kursow);
}

void Window::WczytywanieEduAKZ(QList<Kurs> lista)
{
  QList<Kurs> tmp = lista;
  QList<Kurs> temp;
  bool all = false;
  int val = 0;
  for(auto a: tmp)
    {
      bool kolizja = false;
      for(auto c: przedmioty)
	for(auto &b: c)
	  if(a.kodGrupy() == b.kodGrupy())
	    {
	      Kolizje(all, val, b, a, kolizja);
	      break;
	    }
      if(!kolizja)		  
	temp.push_back(a);
    }
  if(prow.size() != 0)
    {
      QMessageBox msg;
      int retx = msg.question(this, "Informacja", QString("Wczytano: %1 kursów.\nDopasować prowadzących z polwro?").arg(temp.size()));
      if(retx == QMessageBox::Yes)
	przypisz_prowadzacego(temp);
    }
  kursy += temp;

  if(temp.size() != 0)
    reload();
}

void Window::ZapiszKursy()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Zapisz plik", QDir::currentPath(), "Pliki kursów (*.krs)");
  if(fileName.size() == 0)
    return;
  if (!fileName.endsWith(".pln"))
    fileName += ".pln";
  QFile file(fileName);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
      return;
    }
  QTextStream out(&file);
  for(auto y: przedmioty)
  for(auto x: y)
    {
      QString data;
      out << x.kodKursu() << '\n';
      out << x.kodGrupy() << '\n';
      out << x.nazwa() << '\n';
      out << x.forma() << '\n';
      data = x.prowadzacy().at(0);
      for(int i = 1; i < x.prowadzacy().size(); i++)
	data += "||" + x.prowadzacy().at(i);
      out << data << '\n';
      out << x.termin() << '\n';
      out << x.miejsca() << '\n';
      out << x.potok() << '\n';
    }
}

void Window::ZapiszProwadzacych()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Zapisz plik", QDir::currentPath(), "Pliki prowadzących (*.prw)");
  if(fileName.size() == 0)
    return;
  if (!fileName.endsWith(".pln"))
    fileName += ".pln";
  QFile file(fileName);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox msg;
      msg.critical(this, "Błąd", QString("Nie można otworzyć pliku:\n%1").arg(file.fileName()));
      return;
    }
  QTextStream out(&file);
  for(auto x: prow)
    {
      out << x.nazwa() << '\n';
      out << x.ocena() << '\n';
      out << x.iloscOpinii() << '\n';
      out << x.link() << '\n';
    }
}
