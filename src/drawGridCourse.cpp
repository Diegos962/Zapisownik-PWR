#include "../include/main_window.h"

void Window::drawGridV(int rows, int columns)
{
  for(int i = 0; i < rows*3+4; i+=3)
    {
      QFrame *linea1 = new QFrame();
      linea1->setFrameShape(QFrame::HLine);
      linea1->setFrameShadow(QFrame::Plain);
      linea1->setLineWidth(2);
      siatka->addWidget(linea1, i, 0, 1, columns+1);
    }
  for(int j = 0; j <= columns; j++)
    {
      QFrame *linea = new QFrame();
      linea->setFrameShadow(QFrame::Plain);
      linea->setFrameShape(QFrame::VLine);
      if((j+6)%12 == 0)
	linea->setLineWidth(3);
      if(j%3 == 0)
	siatka->addWidget(linea, 0, j, rows*3+1, 1);
    }
}

void Window::drawGridH(int rows, int columns)
{
  for(int j = 0; j < columns*3+1; j+=3)
    {
      QFrame *linea = new QFrame();
      linea->setFrameShadow(QFrame::Plain);
      linea->setFrameShape(QFrame::VLine);
      linea->setLineWidth(2);
      siatka->addWidget(linea, 0, j, rows, 1);
    }
  for(int i = 0; i < rows; i++)
    {
      QFrame *linea1 = new QFrame();
      linea1->setFrameShape(QFrame::HLine);
      linea1->setFrameShadow(QFrame::Plain);
      if((i+6)%12 == 0)
	linea1->setLineWidth(3);
      if(i%3 == 0)
	siatka->addWidget(linea1, i, 0, 2, columns*3+1);
    }
}

void Window::drawCourse(int row, int tydzien, int column, int length, QString grupa,
			QString text, QColor color, QColor fcolor, QString kKursu, QString czas)
{
  BlokWidget *blok = new BlokWidget(this);
  blok->setKodGrupy(grupa);
  blok->setKodKursu(kKursu);
  blok->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  blok->setWordWrap(true);
  czas += "\n" + text;
  blok->setStyleSheet(QString("background-color: %1;\n"
			      "border: 1px solid %1;\n"
			      "opacity: 0.85;\n"
			      "color: %2;").arg(color.name()).arg(fcolor.name()));
  int rowSpan = 0;
  if(tydzien == 5)
    {
      rowSpan = 4;
      tydzien = 0;
    }
  else
    rowSpan = 2;
  if(splitter->orientation() == Qt::Vertical)
    {
      siatka->addWidget(blok, row*3+tydzien, column, rowSpan, length/5 + 1);
      blok->setText(czas);
    }
  else
    {
      int br = czas.indexOf("\n");
      int pos = 1;
      while(pos > 0)
      	{
      	  pos = czas.indexOf(" ", pos+1);
      	  if(pos > br)
      	    czas.replace(pos, 1, "\n");
      	}
      siatka->addWidget(blok, column, row*3+tydzien, length/5 + 2, rowSpan);
      blok->setText(czas);
    }
  QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
  blok->setGraphicsEffect(eff);  
  QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
  a->setDuration(350);
  a->setStartValue(0);
  a->setEndValue(1);
  a->setEasingCurve(QEasingCurve::InBack);
  a->start(QPropertyAnimation::DeleteWhenStopped);
  
  naSiatce.push_back(blok);
}
