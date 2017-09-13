#include "../include/main_window.h"

QPair<int, int> convertDate(QString text)
{
  int hours = 0, hours2 = 0;
  hours += text[0].digitValue()*600;
  hours += text[1].digitValue()*60;
  hours += text.mid(2, 2).toInt();
  hours2 += text[4].digitValue()*600;
  hours2 += text[5].digitValue()*60;
  hours2 += text.mid(6, 2).toInt();
  QPair<int, int> ret;
  ret.first = (hours - 450)/5;
  ret.second = hours2 - hours;
  return ret;
}

bool Window::porownajGodziny(QString g1, QString g2) //g2 - filtr
{
  if(g1[1].digitValue() != g2[1].digitValue())
    return false;
  int P1 = g1[3].digitValue()*600 + g1[4].digitValue()*60 + g1[5].digitValue()*10 + g1[6].digitValue();
  int K1 = g1[7].digitValue()*600 + g1[8].digitValue()*60 + g1[9].digitValue()*10 + g1[10].digitValue();
  int P2 = g2[3].digitValue()*600 + g2[4].digitValue()*60 + g2[5].digitValue()*10 + g2[6].digitValue();
  int K2 = g2[7].digitValue()*600 + g2[8].digitValue()*60 + g2[9].digitValue()*10 + g2[10].digitValue();
  if( (P1 > P2 && P1 < K2) || (K1 > P2 && K1 < K2))
    return true;
  else
    return false;
}

void Window::odznaczWTablicy(int start, int len, int dzien, bool typ, int tydzien)
{
  if(tydzien == 2)
    {
      for(int j = 0; j < tydzien; j++)
	for(int i = start; i < start+(len/5); i++)
	  tablicaCzasu[j][dzien][i] = typ;
    }
  else
    for(int i = start; i < start+(len/5); i++)
      tablicaCzasu[tydzien][dzien][i] = typ;
}
bool Window::CzyPozycjaWolna(int start, int len, int dzien, int tydzien)
{
  if(tydzien == 2)
    {
      for(int j = 0; j < tydzien; j++)
	for(int i = start; i < start+(len/5); i++)
	  if(tablicaCzasu[j][dzien][i])
	    return false;
    }
  else
    for(int i = start; i < start+(len/5); i++)
      if(tablicaCzasu[tydzien][dzien][i])
	return false;
  return true;
}

void Window::ZablokujNakladajace()
{
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = static_cast<TreeWidgetItem *> (tree->topLevelItem(i)->child(j));
	  QStringList data = child->kodTermin().split(";", QString::SkipEmptyParts);
	  for(QString str: data)
	    {
	      QPair<int, int> godziny = convertDate(str.mid(3, 8));
	      CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	      if(!CzyPozycjaWolna(godziny.first, godziny.second, str[1].digitValue(), str[0].digitValue()) || !SprawdzPotok(child->text(7), parent->text(9), parent))
		{
		  child->setBackground(0, QBrush("red"));
		  box->setEnabled(false);
		  break;
		}
	      else
		{
		  child->setBackground(0, QBrush("white"));
		  box->setEnabled(true);
		}
	    }
	}
    }
}

void Window::ZmienPotomkow()
{
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = parent->child(j);
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  if(box->isChecked())
	    {
	      for(int k = 0; k < parent->childCount(); k++)
		{
		  auto child2 = parent->child(k);
		  if(child2 != child)
		    static_cast<CheckBox *> (tree->itemWidget(child2, 0))->setEnabled(false);
		}
	      break;
	    }
	}
    }
}

bool Window::SprawdzPotok(const QString &potok, const QString &kodK, QTreeWidgetItem *wybrany) const
{
  QString kodKursu = kodK.left(kodK.size()-1);
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      if(parent != wybrany)
	{
	  for(int j = 0; j < parent->childCount(); j++)
	    {
	      auto child = static_cast<TreeWidgetItem *> (parent->child(j));
	      QString tmpKodKursu = parent->text(9);
	      tmpKodKursu = tmpKodKursu.left(tmpKodKursu.size()-1);
	      if(tmpKodKursu != kodKursu)
		break;
	      CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	      if(box->isChecked())
		{
		  if(child->text(7) == potok)
		    return true;
		  else
		    return false;
		}
	    }
	}
    }
  return true;
}

void Window::Zaznaczono()
{
  CheckBox *box = static_cast<CheckBox *> (sender());
  auto child = box->treeItem;
  auto parent = child->parent();
  
  QStringList data = child->kodTermin().split(";", QString::SkipEmptyParts);
  if(box->isChecked())
    {
      for(QString str: data)
	{
	  if(str.size() != 11)
	    return;
	  QPair<int, int> godziny = convertDate(str.mid(3, 8));
	  QString czas = str.mid(3, 8).insert(2, ":").insert(5, " - ").insert(10, ":");
	  QString text = parent->text(0);
	  int pos = text.indexOf(" | ");
	  if(pos < 1)
	    pos = text.size();
	  if(CzyPozycjaWolna(godziny.first, godziny.second, str[1].digitValue(), str[0].digitValue()))
	    {
	      odznaczWTablicy(godziny.first, godziny.second, str[1].digitValue(), true, str[0].digitValue());
	      wybraneZajecia.push_back(QPair<CheckBox *, QTreeWidgetItem *>(box, child));
	      int tydzien = 0;
	      if(str[0].digitValue() == 0)
		tydzien = 0;
	      else if (str[0].digitValue() == 1)
		tydzien = 2;
	      else
		tydzien = 5;
	      drawCourse(str[1].digitValue(), tydzien, godziny.first, godziny.second, child->text(1),
			 text.mid(0, pos), parent->background(0).color(), parent->foreground(0).color(), parent->text(9), czas);
	    }
	}
    }
  else
    {
      for(int k = 0; k < parent->childCount(); k++)
	{
	  auto child2 = static_cast<TreeWidgetItem *> (parent->child(k));
	  child2->setHidden(false);
	  child2->setSchowany(false);
	}
      for(QString str: data)
	{
	  QPair<int, int> godziny = convertDate(str.mid(3, 8));
	  odznaczWTablicy(godziny.first, godziny.second, str[1].digitValue(), false, str[0].digitValue());
	  wybraneZajecia.removeOne(QPair<CheckBox *, QTreeWidgetItem *> (box, child));
	  for(auto x: naSiatce)
	    {
	      if(x->kodGrupy() == child->text(1))
		{
		  naSiatce.removeOne(x);
		  delete x;
		  break;
		}
	    }
	}
    }
  ZablokujNakladajace();
  ZmienPotomkow();
  for(auto &a: wybraneZajecia)
    {
      a.first->setEnabled(true);	  		      
      a.second->setBackground(0, QBrush("red"));
    }
  filtry();
}

QList<QList<Kurs>> Window::zwrocListeNajmniejszych(QList<QString> kod)
{
  QList<QList<Kurs>> wolne;
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      wolne.push_back(QList<Kurs>());
      bool dalej = false;
      for(auto xyz: kod)
      	if(parent->text(9) == xyz)
      	  dalej = true;
      if(!dalej)
      	{
	  for(int j = 0; j < parent->childCount(); j++)
	    {
	      auto child = static_cast<TreeWidgetItem *> (parent->child(j));
	      CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	      if(box->isEnabled() && !child->schowany() && !child->isHidden())
		wolne[i].push_back(child->kurs());
	    }
	}
    }
  qSort(wolne.begin(), wolne.end(), [](const QList<Kurs> &l1, const QList<Kurs> &l2)
	{
	  return l1.size() < l2.size();
	});
  return wolne;
}

void Window::losujPlan()
{
  // odznaczZaznaczone();
  QList<QString> wybrane;
  QList<QList<Kurs>> wolne = zwrocListeNajmniejszych(wybrane);
  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());
  for(int i = 0; i < wolne.size(); i++)
    {
      if(wolne[i].size() < 1)
	continue;
      int random = qrand() % (wolne[i].size());
      zaznaczKurs(wolne[i][random].kodKursu(), wolne[i][random].kodGrupy());      
      wybrane.push_back(wolne[i][random].kodKursu());
      wolne = zwrocListeNajmniejszych(wybrane);
      i = 0;
    }
}

void Window::zaznaczKurs(QString kodKursu, QString kodGrupy)
{
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      if(parent->text(9) != kodKursu)
	continue;
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = static_cast<TreeWidgetItem *> (parent->child(j));
	  if(child->text(1) != kodGrupy)
	    continue;
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  box->setChecked(true);
	  return;
	}
    }
}
