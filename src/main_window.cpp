#include "../include/main_window.h"

Window::Window(QWidget *parent): QMainWindow(parent)
{
  QWidget *widget = new QWidget;
  setCentralWidget(widget);
  setWindowTitle("Zapisownik PWR");
  pick = new PickColors(this);
  kolory = pick;
  filtrowanie = new FiltryDialog(this);
  ustawMenu();
  
  for(auto &i: tablicaCzasu)
    for(auto &j: i)
      for(auto &k: j)
	k = false;
  
  tree = new QTreeWidget(this);
  tree->setColumnCount(8);
  tree->setHeaderLabels(QStringList() << "Zaznacz" << "Kod grupy" << "Prowadzący" << "Ocena" << "Ilość opinii" << "Termin" << "Miejsca" << "Potok");
  tree->setSelectionMode(QAbstractItemView::NoSelection);
  tree->setSortingEnabled(true);
  tree->setRootIsDecorated(false);
  tree->setColumnWidth(2, 250);
  tree->setColumnWidth(5, 150);
  connect(tree, &QTreeWidget::itemClicked, this, &Window::TreeHeaderClicked);
  
  siatka = new QGridLayout();
  siatka->setContentsMargins(0, 0, 0, 0);
  drawGridV(5, 14.5*12);

  splitter = new QSplitter;
  splitter->setHandleWidth(3);
  splitter->setStyleSheet("QSplitter::handle{background: black;}");
  QWidget *top = new QWidget;
  QBoxLayout *topLayout = new QHBoxLayout;
  topLayout->addWidget(tree);
  top->setLayout(topLayout);
  splitter->addWidget(top);
  
  QWidget *bot = new QWidget;
  bot->setLayout(siatka);
  splitter->addWidget(bot);

  splitter->setOrientation(Qt::Vertical);
  
  QVBoxLayout *layout = new QVBoxLayout();
  
  layout->setMenuBar(menu);
  layout->addWidget(splitter);
  widget->setLayout(layout);
}

Window::~Window()
{
  while(QWidget *w = findChild<QWidget*>())
    delete w;
}

void Window::PrzelaczWidok()
{
  while(siatka->count() > 0)
    delete siatka->takeAt(0)->widget();
  naSiatce.clear();
  if(splitter->orientation() == Qt::Vertical)
    {
      splitter->setOrientation(Qt::Horizontal);
      drawGridH(14.5*12, 5);
    }
  else
    {
      drawGridV(5, 14.5*12);
      splitter->setOrientation(Qt::Vertical);
    }
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
  	{
  	  auto child = static_cast<TreeWidgetItem *> (parent->child(j));
  	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
  	  if(box->isChecked())
  	    {
  	      box->setChecked(false);
  	      box->setChecked(true);
  	    }
  	}
    }
}

void Window::odznaczZaznaczone()
{
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = static_cast<TreeWidgetItem *> (parent->child(j));
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  if(box->isChecked())
	    box->setChecked(false);
	}
    }
}

void Window::filtry()
{
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = static_cast<TreeWidgetItem *> (parent->child(j));
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  if(child->schowany() || box->isChecked())
	    continue;
	  if(filtrowanie->ukryjZajete())
	    {
	      CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	      if(child->background(0).color().name() == "#ff0000" && !box->isChecked())
		{
		  child->setHidden(true);
		  continue;
		}
	    }
	  else
	    {
	      if(child->background(0).color().name() == "#000000")
		child->setHidden(false);
	    }
	  if(parent->text(9).right(1) == "W")
	    {
	      if(filtrowanie->ukryjPelneW() && (child->text(6).toFloat()) <= 0)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	  else if(parent->text(9).right(1) == "C")
	    {
	      if(filtrowanie->ukryjPelneC() && (child->text(6).toFloat()) <= 0)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	  else if(parent->text(9).right(1) == "L")
	    {
	      if(filtrowanie->ukryjPelneL() && (child->text(6).toFloat()) <= 0)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	  else if(parent->text(9).right(1) == "P")
	    {
	      if(filtrowanie->ukryjPelneP() && (child->text(6).toFloat()) <= 0)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	  else if(parent->text(9).right(1) == "S")
	    {
	      if(filtrowanie->ukryjPelneS() && (child->text(6).toFloat()) <= 0)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	  else if(parent->text(9).right(1) == "I")
	    {
	      if(filtrowanie->ukryjPelneI() && (child->text(6).toFloat()) <= 0)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	  if(filtrowanie->ukryjOcena())
	    {
	      QStringList oceny = child->text(3).split("|", QString::SkipEmptyParts);
	      float srednia = 0;
	      for(QString xya: oceny)
		srednia += xya.toFloat();
	      if(srednia != 0 && srednia < filtrowanie->ukryjOcenaWartosc())
		{
		  child->setHidden(true);
		  continue;
		}
	    }
	  else
	    child->setHidden(false);
	  if(filtrowanie->ukryjTerminy().size() != 0)
	    {
	      bool wylacz = false;
	      for(auto a: filtrowanie->ukryjTerminy())
		if(porownajGodziny(child->kodTermin(), a))
		  {
		    wylacz = true;
		    break;
		  }
	      if(wylacz)
		{
		  child->setHidden(true);
		  continue;
		}
	      else
		child->setHidden(false);
	    }
	}
    }
}
void Window::reload()
{
  for(auto x: naSiatce)
    delete x;
  for(auto &i: tablicaCzasu)
    for(auto &j: i)
      for(auto &k: j)
	k = false;
  naSiatce.clear();
  wybraneZajecia.clear();
  tree->clear();
  Przedmioty();  
  build_tree();
}

void Window::TreeHeaderClicked(QTreeWidgetItem *parent)
{
  int count = 0;
  if(!parent->isExpanded())
    tree->expandItem(parent);
  else
    {
      for(int i = 0; i < parent->childCount(); i++)
	{
	  auto child = parent->child(i);
	  CheckBox *box = static_cast<CheckBox *> (tree->itemWidget(child, 0));
	  if(box->isChecked())
	    {
	      count++;
	      for(int k = 0; k < parent->childCount(); k++)
		{
		  auto child2 = static_cast<TreeWidgetItem *> (parent->child(k));
		  if(child2->isHidden() && child2->schowany())
		    {
		      child2->setHidden(false);
		      child2->setSchowany(false);
		    }
		  else if(child2 != child)
		    {
		      child2->setHidden(true);
		      child2->setSchowany(true);
		    }
		}
	      break;
	    }
	}
      if(count == 0)
	{
	  tree->collapseItem(parent);
	  for(int k = 0; k < parent->childCount(); k++)
	    {
	      auto child2 = static_cast<TreeWidgetItem *> (parent->child(k));
	      child2->setHidden(false);
	      child2->setSchowany(false);
	    }
	}
    }
  filtry();
}

void Window::Przedmioty()
{
  qSort(kursy.begin(), kursy.end(), [](const Kurs &s1, const Kurs &s2)
	{return s1.kodKursu() < s2.kodKursu();});
  bool found;
  for(int i = 0; i < kursy.size();)
    {
      found = false;
      Kurs kurs = kursy[0];
      for(auto &j: przedmioty)
	if(j.size() != 0)
	  {
	    if(kurs.kodKursu() != j[0].kodKursu())
	      continue;
	    else
	      {
		do
		  {
		    j.push_back(kurs);
		    kursy.removeAt(0);
		    if( kursy.size() > 0)
		      kurs = kursy[0];
		    else
		      break;	      
		  }
		while(kurs.kodKursu() == j[0].kodKursu());
		found = true;
		break;
	      }
	  }
      if(!found)
	{
	  QList<Kurs> tmp;
	  tmp.push_back(kurs);
	  kursy.removeAt(0);
	  przedmioty.push_back(tmp);
	}
    }
}

void Window::przypisz_prowadzacego(QList<Kurs> &lista)
{
  QProgressDialog dialog;
  dialog.setLabelText("Dopasowywanie prowadzących");
  dialog.setCancelButton(0);
  QFutureWatcher<void> futureWatcher;
  connect(&futureWatcher, &QFutureWatcher<void>::progressRangeChanged, &dialog, &QProgressDialog::setRange);
  connect(&futureWatcher, &QFutureWatcher<void>::progressValueChanged, &dialog, &QProgressDialog::setValue);
  futureWatcher.setFuture(QtConcurrent::map(lista, [=] (Kurs &data) {dopasuj(data, prow);}));
  dialog.exec();
  futureWatcher.waitForFinished();
}

void Window::rozdajKolory(QString l, QColor &clr, QColor &fclr)
{
  if(l == "W")
    {
      clr = kolory.colorWyklady();
      fclr = kolory.fontColorWyklady();
    }
  else if(l == "C")
    {
      clr = kolory.colorCwiczenia();
      fclr = kolory.fontColorCwiczenia();
    }
  else if(l == "L")
    {
      clr = kolory.colorLaborki();
      fclr = kolory.fontColorLaborki();
    }
  else if(l == "P")
    {
      clr = kolory.colorProjekty();
      fclr = kolory.fontColorProjekty();
    }
  else if(l == "S")
    {
      clr = kolory.colorSeminaria();
      fclr = kolory.fontColorSeminaria();
    }
  else if(l == "I")
    {
      clr = kolory.colorInne();
      fclr = kolory.fontColorInne();
    }
}

void Window::UpdateColors()
{
  QColor clr;
  QColor fclr;
  for(int i = 0; i < tree->topLevelItemCount(); ++i)
    {
      QTreeWidgetItem *treeItem = tree->topLevelItem(i);
      QString l = treeItem->text(9).right(1);
      rozdajKolory(l, clr, fclr);
      treeItem->setBackground(0, clr);
      treeItem->setForeground(0, fclr);
    }
  for(auto x: naSiatce)
    {
      QString l = x->kodKursu().right(1);
      rozdajKolory(l, clr, fclr);
      x->setStyleSheet(QString("background-color: %1;\n"
			       "border: 1px solid %1;\n"
			       "opacity: 0.85;\n"
			       "color: %2;").arg(clr.name()).arg(fclr.name()));
    }
}

void Window::przepiszKolory(PickColors *p)
{
  p->setColorWyklady(kolory.colorWyklady());
  p->setColorCwiczenia(kolory.colorCwiczenia());
  p->setColorLaborki(kolory.colorLaborki());
  p->setColorProjekty(kolory.colorProjekty());
  p->setColorSeminaria(kolory.colorSeminaria());
  p->setColorInne(kolory.colorInne());
  p->setFontColorWyklady(kolory.fontColorWyklady());
  p->setFontColorCwiczenia(kolory.fontColorCwiczenia());
  p->setFontColorLaborki(kolory.fontColorLaborki());
  p->setFontColorProjekty(kolory.fontColorProjekty());
  p->setFontColorSeminaria(kolory.fontColorSeminaria());
  p->setFontColorInne(kolory.fontColorInne());
  p->updateColors();
}

void Window::ZmienKolory()
{
  przepiszKolory(pick);
  pick->show();
  int value = pick->exec();
  if(value)
    {
      kolory = pick;
      UpdateColors();
    }
}
