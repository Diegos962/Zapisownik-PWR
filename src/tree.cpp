#include "../include/main_window.h"

TreeWidgetItem *findItem(QTreeWidget *tree, QString text, int column)
{
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = static_cast<TreeWidgetItem *>(tree->topLevelItem(i));
      if(parent->text(column) == text)
	return parent;
      for(int j = 0; j < parent->childCount(); j++)
	{
	  auto child = static_cast<TreeWidgetItem *>(parent->child(j));
	  if(child->text(column) == text)
	    return child;
	}
    }
  return nullptr;
}

TreeWidgetItem* Window::add_parent(Kurs a)
{
  TreeWidgetItem *treeItem = new TreeWidgetItem(tree);
  treeItem->setText(0, a.nazwa() + " | " + a.forma());
  treeItem->setText(9, a.kodKursu());
  treeItem->setFirstColumnSpanned(true);
  treeItem->setTextAlignment (0, Qt::AlignCenter);
  return treeItem;
}

void Window::add_child(TreeWidgetItem *treeItem, Kurs a)
{
  QLabel *prowadzacy = new QLabel(this);
  QString data;
  int daneSize = a.dane().size() - 1;
  for(int i = 0; i < a.prowadzacy().size(); i++)
    {
      if(daneSize >= i)
	{
	  if(a.dane()[i].link().size() != 0)
	    {
	      prowadzacy->setTextFormat(Qt::RichText);
	      prowadzacy->setTextInteractionFlags(Qt::TextBrowserInteraction);
	      prowadzacy->setOpenExternalLinks(true);
	      data += "       " + QString("<a href=\"http://polwro.pl/%1\">%2</a>").arg(a.dane()[i].link()).arg(a.prowadzacy()[i]);
	    }
	  else
	    data += "       " + a.prowadzacy()[i];
	}
      else
	data += "       " + a.prowadzacy()[i];
    }
  prowadzacy->setText(data);
  
  QString oceny;
  QString opinie;
  TreeWidgetItem *child = new TreeWidgetItem(treeItem);
  CheckBox *checkBox = new CheckBox(this);
  checkBox->treeItem = child;
  child->setKurs(a);
  tree->setItemWidget(child, 0, checkBox);  	  
  child->setText(1, a.kodGrupy());
  tree->setItemWidget(child, 2, prowadzacy);
  for(int i = 0; i < a.dane().size(); i++)
    if(i == 0)
      {
	oceny = a.dane()[0].ocena();
	opinie = a.dane()[0].iloscOpinii();
      }
    else
      {
	oceny += " | " + a.dane()[i].ocena();
	opinie += " | " + a.dane()[i].iloscOpinii();
      }
  child->setText(3, oceny);
  child->setText(4, opinie);
  child->setKodTermin(a.termin());
  child->setText(5, DecodeTermin(a.termin()));
  child->setText(6, a.miejsca());
  child->setText(7, a.potok());
  connect(checkBox, &CheckBox::stateChanged, this, &Window::Zaznaczono);
  
}

void Window::add_branch(int ilosc)
{
  int size = 0;
  for(int i = 0; i < przedmioty.size(); i++)
    for(int j = 0; j < przedmioty[i].size(); j++)
      size++;
  QProgressDialog progress("Ładowanie kursów", "Anuluj", 0, size - ilosc, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(0);
  progress.setCancelButton(0);
  for(int i = 0; i < (size - ilosc); i++)
    {
      progress.setValue(i+1);
      for(auto b: przedmioty)
	for(auto a: b)
	  {
	    auto x = findItem(tree, a.kodGrupy(), 1);
	    if(x == nullptr)
	      {
		auto y = findItem(tree, a.kodKursu(), 9);
		if(y == nullptr)
		  {
		    add_parent(a);
		    i--;
		  }
		else
		  {
		    add_child(y, a);
		    break;
		  }
	      }
	  }
    }
}

void Window::update_tree()
{
  bool p = false;
  int ilosc = 0;
  for(int i = 0; i < tree->topLevelItemCount(); i++)
    {
      auto parent = tree->topLevelItem(i);
      for(int j = 0; j < parent->childCount(); j++)
	{
	  ilosc++;
	  auto child = static_cast<TreeWidgetItem *>(parent->child(j));
	  p = false;
	  for(auto b: przedmioty)
	    for(auto a: b)
	      {
		if(a.kodKursu() != parent->text(9))
		  break;
		if(a.kodGrupy() == child->text(1))
		  {
		    QLabel *prowadzacy = new QLabel(this);
		    QString data;

		    int daneSize = a.dane().size() - 1;
		    for(int i = 0; i < a.prowadzacy().size(); i++)
		      {
			if(daneSize >= i)
			  {
			    if(a.dane()[i].link().size() != 0)
			      {
				prowadzacy->setTextFormat(Qt::RichText);
				prowadzacy->setTextInteractionFlags(Qt::TextBrowserInteraction);
				prowadzacy->setOpenExternalLinks(true);
				data += ' ' + QString("<a href=\"http://polwro.pl/%1\">%2</a>").arg(a.dane()[i].link()).arg(a.prowadzacy()[i]);
			      }
			    else
			      data += ' ' + a.prowadzacy()[i];
			  }
			else
			  data += ' ' + a.prowadzacy()[i];
		      }
		    prowadzacy->setText(data);

		    QString oceny;
		    QString opinie;
		    tree->setItemWidget(child, 2, prowadzacy);
		    for(int i = 0; i < a.dane().size(); i++)
		      if(i == 0)
			{
			  oceny = a.dane()[0].ocena();
			  opinie = a.dane()[0].iloscOpinii();
			}
		      else
			{
			  oceny += " | " + a.dane()[i].ocena();
			  opinie += " | " + a.dane()[i].iloscOpinii();
			}
		    (child)->setText(3, oceny);
		    (child)->setText(4, opinie);
		    child->setKodTermin(a.termin());
		    child->setText(5, DecodeTermin(a.termin()));
		    (child)->setText(6, a.miejsca());
		    p = true;
		    break;
		  }
	      }
	  if(!p)
	    {
	      ilosc--;
	      parent->removeChild(child);
	      if(tree->topLevelItem(i)->childCount() == 0)
	  	{
	  	  delete parent;
	  	  i--;
	  	  break;
	  	}
	      j--;
	    }
	}
    }
  for(int i = 0; i < przedmioty.size(); i++)
    if(przedmioty.at(i).size() == 0)
      przedmioty.removeAt(i--);
  
  add_branch(ilosc);
  UpdateColors();
}

void Window::build_tree()
{
  int size = 0;
  int count = 0;
  for(int i = 0; i < przedmioty.size(); i++)
    for(int j = 0; j < przedmioty[i].size(); j++)
      size++;
  QProgressDialog progress("Ładowanie kursów", "Anuluj", 0, size, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(0);
  progress.setCancelButton(0);
  for(auto a: przedmioty)
    {
      progress.setValue(count);
      TreeWidgetItem *treeItem;
      if(a.size() != 0)
  	treeItem = add_parent(a[0]);
      else
  	continue;
      for(auto b: a)
	{
	  add_child(treeItem, b);
	  count++;
	}
    }
  UpdateColors();
  tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
