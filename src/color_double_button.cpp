#include "../include/color_double_button.h"

ColorDoubleButton::ColorDoubleButton(const QString &text, QWidget *parent):
  QPushButton(text, parent)
{}

void ColorDoubleButton::mousePressEvent(QMouseEvent *e)
{
  if(e->button() == Qt::RightButton)
    emit rightClicked();
  else if(e->button() == Qt::LeftButton)
    emit leftClicked();
}

void ColorDoubleButton::setColor(const QColor &clr)
{
  colorBg = clr;
}

void ColorDoubleButton::setFontColor(const QColor &clr)
{
  fontColorFg = clr;
}

QColor ColorDoubleButton::color() const
{
  return colorBg;
}

QColor ColorDoubleButton::fontColor() const
{
  return fontColorFg;
}
