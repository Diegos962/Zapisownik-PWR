#include "../include/dropboxDialog.h"

dropboxDialog::dropboxDialog(Dropbox *db, const QString &path, QWidget *parent): QDialog(parent)
{
  dropbox = db;
  resize(500, 300);
  setModal(true);
  setWindowTitle("Zewnętrzna baza danych");
  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *layoutButtons = new QHBoxLayout();
  tree = new QTreeWidget(this);
  QPushButton *anuluj = new QPushButton("Anuluj", this);
  QPushButton *zapisz = new QPushButton("Pobierz i zamknij", this);
  
  layoutButtons->addStretch(100);
  layoutButtons->addWidget(zapisz);
  layoutButtons->addWidget(anuluj);
  layout->addWidget(tree);
  layout->addLayout(layoutButtons);

  tree->setColumnCount(2);
  tree->setHeaderLabels
    (QStringList() << "Ilość kursów" << "Data pobrania");

  tree->setSortingEnabled(true);
  tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  connect(anuluj, &QPushButton::clicked, this, &QDialog::reject);
  connect(zapisz, &QPushButton::clicked, this, &dropboxDialog::pobierz);

  dane(path);
  parentWidget()->setCursor(Qt::ArrowCursor);
}

void dropboxDialog::dane(const QString &path)
{
  setCursor(Qt::WaitCursor);
  tree->setEnabled(false);
  QList<QDropboxFileInfo> rekordy = dropbox->filterData(path);
  setCursor(Qt::ArrowCursor);
  tree->setEnabled(true);
  for(auto a: rekordy)
    {
      QTreeWidgetItem *tmp1 = new QTreeWidgetItem(tree);
      tmp1->setText(0, a.path());
      tmp1->setText(2, a.path());
      if(a.isDir())
	{
	  tmp1->setFirstColumnSpanned(true);
	  tmp1->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	  tmp1->setText(3, "1");
	  connect(tree, &QTreeWidget::itemClicked, this, &dropboxDialog::addChilds);
	}
    }
}

void dropboxDialog::addChilds(QTreeWidgetItem *parent, int)
{
  if(parent->childCount() > 0 || parent->text(0).endsWith(".txt"))
      return;
  setCursor(Qt::WaitCursor);
  tree->setEnabled(false);
  QList<QDropboxFileInfo> rekordy = dropbox->filterData(parent->text(2));
  setCursor(Qt::ArrowCursor);
  tree->setEnabled(true);
  for(auto a: rekordy)
    {
      QTreeWidgetItem *tmp1 = new QTreeWidgetItem(parent);
      tmp1->setText(0, a.path().remove(parent->text(2)).remove("/"));
      tmp1->setText(2, a.path());
      if(a.isDir())
	{
	  tmp1->setFirstColumnSpanned(true);
	  tmp1->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	  tmp1->setText(3, "1");
	}
      else
	{
	  int pos = tmp1->text(0).indexOf("|");
	  tmp1->setText(0, tmp1->text(0).left(pos));
	  tmp1->setText(1, a.modified().toTimeSpec(Qt::LocalTime).toString("dd.MM.yyyy hh.mm"));
	  tmp1->setText(3, "0");
	}
    }
  tree->expandItem(parent);
}

void dropboxDialog::pobierz()
{
  if(tree->currentItem() == nullptr || tree->currentItem()->text(3) == "1")
    return;
  setCursor(Qt::WaitCursor);
  tree->setEnabled(false);
  QByteArray data = dropbox->downloadFile("/dropbox" + tree->currentItem()->text(2));
  QTextStream stream(data);
  stream.setCodec("UTF-8");
  while(!stream.atEnd())
    {
      Kurs aaa;
      aaa.setKodKursu(stream.readLine());
      aaa.setKodGrupy(stream.readLine());
      aaa.setNazwa(stream.readLine());
      aaa.setForma(stream.readLine());
      aaa.setProwadzacy(stream.readLine().split("||", QString::SkipEmptyParts));
      aaa.setTermin(stream.readLine());
      aaa.setMiejsca(stream.readLine());
      aaa.setPotok(stream.readLine());
      listaK.push_back(aaa);
    }
  QDialog::accept();
}
