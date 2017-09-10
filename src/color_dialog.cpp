#include "../include/color_dialog.h"

PickColors::PickColors(QWidget *parent): QDialog(parent)
{
  setModal(true);
  setWindowTitle("Ustaw kolory");
  QGridLayout *grid = new QGridLayout(this);
  wyklady = new ColorDoubleButton("Wykłady", this);
  cwiczenia = new ColorDoubleButton("Ćwiczenia", this);
  laborki = new ColorDoubleButton("Zajęcia laboratoryjne", this);
  projekty = new ColorDoubleButton("Projekt", this);
  seminaria = new ColorDoubleButton("Seminarium", this);
  inne = new ColorDoubleButton("Inne", this);
  KoloryJSOS = new ColorDoubleButton("Kolory JSOSa", this);
  KoloryPlanPWR = new ColorDoubleButton("Kolory z plan-pwr", this);
  buttons = new QDialogButtonBox(this);

  buttons->addButton(QDialogButtonBox::Cancel);
  buttons->addButton(QDialogButtonBox::Ok);
  buttons->button(QDialogButtonBox::Ok)->setText("Zastosuj");
  buttons->button(QDialogButtonBox::Cancel)->setText("Anuluj");

  grid->addWidget(wyklady, 0, 0);
  grid->addWidget(cwiczenia, 1, 0);
  grid->addWidget(laborki, 2, 0);
  grid->addWidget(projekty, 3, 0);
  grid->addWidget(seminaria, 4, 0);
  grid->addWidget(inne, 5, 0);
  grid->addWidget(KoloryJSOS, 2, 1);
  grid->addWidget(KoloryPlanPWR, 3, 1);
  grid->addWidget(buttons, 6, 0, 1, 2);
  
  connect(KoloryJSOS, &ColorDoubleButton::leftClicked, this, &PickColors::JSOS_clicked);
  connect(KoloryPlanPWR, &ColorDoubleButton::leftClicked, this, &PickColors::PWR_clicked);
  connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  connect(wyklady, &ColorDoubleButton::leftClicked, this, &PickColors::getColor);
  connect(wyklady, &ColorDoubleButton::rightClicked, this, &PickColors::getFontColor);
  connect(cwiczenia, &ColorDoubleButton::leftClicked, this, &PickColors::getColor);
  connect(cwiczenia, &ColorDoubleButton::rightClicked, this, &PickColors::getFontColor);
  connect(laborki, &ColorDoubleButton::leftClicked, this, &PickColors::getColor);
  connect(laborki, &ColorDoubleButton::rightClicked, this, &PickColors::getFontColor);
  connect(projekty, &ColorDoubleButton::leftClicked, this, &PickColors::getColor);
  connect(projekty, &ColorDoubleButton::rightClicked, this, &PickColors::getFontColor);
  connect(seminaria, &ColorDoubleButton::leftClicked, this, &PickColors::getColor);
  connect(seminaria, &ColorDoubleButton::rightClicked, this, &PickColors::getFontColor);
  connect(inne, &ColorDoubleButton::leftClicked, this, &PickColors::getColor);
  connect(inne, &ColorDoubleButton::rightClicked, this, &PickColors::getFontColor);
  KoloryJSOS->leftClicked();
  setLayout(grid);
}

void PickColors::getColor()
{
  QColor tmp = QColorDialog::getColor(colorWyklady(), this);
  if(tmp.isValid())
    {
      ColorDoubleButton *obj = static_cast<ColorDoubleButton *> (sender());
      obj->setColor(tmp);
	updateColors();
    }
}

void PickColors::getFontColor()
{
  QColor tmp = QColorDialog::getColor(fontColorWyklady(), this);
  if(tmp.isValid())
    {
      ColorDoubleButton *obj = static_cast<ColorDoubleButton *> (sender());
      obj->setFontColor(tmp);
	updateColors();
    }
}

void PickColors::JSOS_clicked() const
{
  wyklady->setColor(QColor(0, 166, 90));
  cwiczenia->setColor(QColor(243, 156, 18));
  laborki->setColor(QColor(60, 141, 188));
  projekty->setColor(QColor(0, 192, 239));
  seminaria->setColor(QColor(69, 182, 176));
  inne->setColor(QColor(0, 0, 0));
  wyklady->setFontColor(QColor(255, 255, 255));
  cwiczenia->setFontColor(QColor(255, 255, 255));
  laborki->setFontColor(QColor(255, 255, 255));
  projekty->setFontColor(QColor(255, 255, 255));
  seminaria->setFontColor(QColor(255, 255, 255));
  inne->setFontColor(QColor(255, 255, 255));
  updateColors();
}

void PickColors::PWR_clicked() const
{
  wyklady->setColor(QColor(243, 182, 183));
  cwiczenia->setColor(QColor(174, 237, 145));
  laborki->setColor(QColor(165, 196, 247));
  projekty->setColor(QColor(239, 188, 240));
  seminaria->setColor(QColor(245, 220, 192));
  inne->setColor(QColor(204, 193, 236));
  wyklady->setFontColor(QColor(230, 48, 33));
  cwiczenia->setFontColor(QColor(71, 174, 3));
  laborki->setFontColor(QColor(28, 100, 220));
  projekty->setFontColor(QColor(187, 58, 189));
  seminaria->setFontColor(QColor(243, 142, 0));
  inne->setFontColor(QColor(92, 58, 178));
  updateColors();
}

void PickColors::updateColors() const
{
  updateButton(wyklady);
  updateButton(cwiczenia);
  updateButton(laborki);
  updateButton(projekty);
  updateButton(seminaria);
  updateButton(inne);
}

void PickColors::updateButton(ColorDoubleButton *bt) const
{
  bt->setStyleSheet(QString("background-color: %1; color: %2;").arg(bt->color().name()).arg(bt->fontColor().name()));
}

QColor PickColors::colorWyklady() const {return wyklady->color();}
QColor PickColors::colorCwiczenia() const {return cwiczenia->color();}
QColor PickColors::colorLaborki() const {return laborki->color();}
QColor PickColors::colorProjekty() const {return projekty->color();}
QColor PickColors::colorSeminaria() const {return seminaria->color();}
QColor PickColors::colorInne() const {return inne->color();}
QColor PickColors::fontColorWyklady() const {return wyklady->fontColor();}
QColor PickColors::fontColorCwiczenia() const {return cwiczenia->fontColor();}
QColor PickColors::fontColorLaborki() const {return laborki->fontColor();}
QColor PickColors::fontColorProjekty() const {return projekty->fontColor();}
QColor PickColors::fontColorSeminaria() const {return seminaria->fontColor();}
QColor PickColors::fontColorInne() const {return inne->fontColor();}
void PickColors::setColorWyklady(const QColor &clr) {wyklady->setColor(clr);}
void PickColors::setColorCwiczenia(const QColor &clr) {cwiczenia->setColor(clr);}
void PickColors::setColorLaborki(const QColor &clr) {laborki->setColor(clr);}
void PickColors::setColorProjekty(const QColor &clr) {projekty->setColor(clr);}
void PickColors::setColorSeminaria(const QColor &clr) {seminaria->setColor(clr);}
void PickColors::setColorInne(const QColor &clr) {inne->setColor(clr);}
void PickColors::setFontColorWyklady(const QColor &clr)
{wyklady->setFontColor(clr);}
void PickColors::setFontColorCwiczenia(const QColor &clr)
{cwiczenia->setFontColor(clr);}
void PickColors::setFontColorLaborki(const QColor &clr)
{laborki->setFontColor(clr);}
void PickColors::setFontColorProjekty(const QColor &clr)
{projekty->setFontColor(clr);}
void PickColors::setFontColorSeminaria(const QColor &clr)
{seminaria->setFontColor(clr);}
void PickColors::setFontColorInne(const QColor &clr)
{inne->setFontColor(clr);}
