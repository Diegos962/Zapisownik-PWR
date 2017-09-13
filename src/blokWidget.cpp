#include "../include/blokWidget.h"

BlokWidget::BlokWidget(QWidget *parent): QLabel(parent)
{
  resizeTimer = new QTimer(this);
  resizeTimer->setInterval(100);
  resizeTimer->setSingleShot(true);
  connect(resizeTimer, &QTimer::timeout, this, &BlokWidget::resizeFont);
}

void BlokWidget::resizeEvent(QResizeEvent* event)
{
  QLabel::resizeEvent(event);
  resizeTimer->start();
}

void BlokWidget::resizeFont()
{
  QFont basefont = font();
  const QRect baseRect = contentsRect();
  const QString baseText = text();
  int fontSizeGuess = 5;
  int zapas = 10;
  QSplitter *splt = static_cast<QSplitter *> (parent()->parent());
  if(splt->orientation() == Qt::Vertical)
    {
      fontSizeGuess = 10;
      zapas = 45;
    }
  for(;; ++fontSizeGuess)
    {
      QFont testFont(basefont);
      testFont.setPixelSize(fontSizeGuess);
      const QRect fontRect = QFontMetrics(testFont).boundingRect(rect(), Qt::TextWordWrap, baseText);
      if(fontRect.height() + 15 > baseRect.height() || fontRect.width() + zapas > baseRect.width())
	break;
    }
  for(; fontSizeGuess > 1; --fontSizeGuess)
    {
      QFont testFont(basefont);
      testFont.setPixelSize(fontSizeGuess);
      const QRect fontRect = QFontMetrics(testFont).boundingRect(rect(), Qt::TextWordWrap, baseText);
      if(fontRect.height() <= baseRect.height() && fontRect.width() <= baseRect.width())
	break;
    }
  basefont.setPixelSize(fontSizeGuess);
  setFont(basefont);
}

void BlokWidget::setKodGrupy(const QString kg)
{
  kod_grupy = kg;
}

void BlokWidget::setKodKursu(const QString kk)
{
  kod_kursu = kk;
}

QString BlokWidget::kodGrupy() const 
{
  return kod_grupy;
}

QString BlokWidget::kodKursu() const
{
  return kod_kursu;
}


void BlokWidget::enterEvent(QEvent *)
{
  duzy = new QLabel(parentWidget()->parentWidget()->parentWidget());
  duzy->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  duzy->setWordWrap(true);
  duzy->setScaledContents(true);
  duzy->setStyleSheet(styleSheet());

  duzy->setText(text());
  duzy->show();
  const QRect fontRect = QFontMetrics(duzy->font()).boundingRect(rect(), Qt::TextWordWrap, text());
  duzy->setGeometry(0, 0, 250, fontRect.height());
}

void BlokWidget::leaveEvent(QEvent *)
{
  delete duzy;
}

Day::Day(QString text, QWidget *parent): QLabel(text, parent)
{}

void Day::enterEvent(QEvent *)
{
  lower();
}
