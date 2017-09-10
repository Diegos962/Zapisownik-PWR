#include "../include/treeWidgetItem.h"

TreeWidgetItem::TreeWidgetItem(QTreeWidget *parent): QTreeWidgetItem(parent)
{
  Schowany = false;
  Termin = "";
}

TreeWidgetItem::TreeWidgetItem(TreeWidgetItem *parent): QTreeWidgetItem(parent)
{
  Schowany = false;
  Termin = "";
}

bool TreeWidgetItem::operator < (const QTreeWidgetItem &other) const
{
  int column = treeWidget()->sortColumn();
  auto xxx = const_cast<TreeWidgetItem &>(static_cast<const TreeWidgetItem &>(other));
  if(column == 5)
    return kodTermin() < xxx.kodTermin();
  else
    {
      bool ok, ok1;
      int w = text(column).toInt(&ok);
      int q = other.text(column).toInt(&ok1);
      if(ok && ok1)
	return w < q;
      else
	return text(column).toLower() < other.text(column).toLower();
    }
}

void TreeWidgetItem::setKodTermin(const QString &kt)
{
  Termin = kt;
}

void TreeWidgetItem::setSchowany(const bool &s)
{
  Schowany = s;
}

void TreeWidgetItem::setKurs(const Kurs &kr)
{
  k = kr;
}

QString TreeWidgetItem::kodTermin() const
{
  return Termin;
}

bool TreeWidgetItem::schowany() const
{
  return Schowany;
}

Kurs TreeWidgetItem::kurs() const
{
  return k;
}

