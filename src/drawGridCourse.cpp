#include "../include/main_window.h"

void Window::ustawGodzinyNaSiatceV()
{
  QLabel *godz1 = new QLabel("7:30", this);
  siatka->addWidget(godz1, 0, 0, 1, 0);
  QLabel *godz2 = new QLabel("9:00", this);
  siatka->addWidget(godz2, 0, 16, 1, -1);
  QLabel *godz3 = new QLabel("11:00", this);
  siatka->addWidget(godz3, 0, 40, 1, -1);
  QLabel *godz4 = new QLabel("13:00", this);
  siatka->addWidget(godz4, 0, 64, 1, -1);
  QLabel *godz5 = new QLabel("15:00", this);
  siatka->addWidget(godz5, 0, 88, 1, -1);
  QLabel *godz6 = new QLabel("17:00", this);
  siatka->addWidget(godz6, 0, 112, 1, -1);
  QLabel *godz7 = new QLabel("19:00", this);
  siatka->addWidget(godz7, 0, 136, 1, -1);
  QLabel *godz8 = new QLabel("21:00", this);
  siatka->addWidget(godz8, 0, 160, 1, -1);
}

void Window::ustawGodzinyNaSiatceH()
{
  QLabel *godz1 = new QLabel("7:30", this);
  siatka->addWidget(godz1, 0, 1, 5, 1);
  QLabel *godz2 = new QLabel("9:00", this);
  siatka->addWidget(godz2, 16, 1, 5, 1);
  QLabel *godz3 = new QLabel("11:00", this);
  siatka->addWidget(godz3, 40, 1, 5, 1);
  QLabel *godz4 = new QLabel("13:00", this);
  siatka->addWidget(godz4, 64, 1, 5, 1);
  QLabel *godz5 = new QLabel("15:00", this);
  siatka->addWidget(godz5, 88, 1, 5, 1);
  QLabel *godz6 = new QLabel("17:00", this);
  siatka->addWidget(godz6, 112, 1, 5, 1);
  QLabel *godz7 = new QLabel("19:00", this);
  siatka->addWidget(godz7, 136, 1, 5, 1);
  QLabel *godz8 = new QLabel("21:00", this);
  siatka->addWidget(godz8, 160, 1, 5, 1);
}

void Window::ustawDzienNaSiatce(QString dzien, int row, int col, int rowSpan, int colSpan)
{
  Day *day = new Day(dzien, this);
  day->setStyleSheet("QLabel {color: rgba(255, 244, 48, 50);}");
  QFont font = day->font();
  font.setPixelSize(34);
  day->setFont(font);
  siatka->addWidget(day, row, col, rowSpan, colSpan);
  QColor clr =  QWidget::palette().color(QWidget::backgroundRole());
  if(clr.red() > 230 && clr.green() > 230 && clr.blue() > 230)
    day->setStyleSheet("QLabel {color: rgba(49, 54, 59, 50);}");
  else
    day->setStyleSheet("QLabel {color: rgba(255, 244, 48, 50);}");
}

void Window::drawGridV(int rows, int columns)
{
  ustawGodzinyNaSiatceV();
  ustawDzienNaSiatce("PONIEDZIAŁEK", 2, 0, 4, 100);
  ustawDzienNaSiatce("WTOREK", 5, 0, 4, 100);
  ustawDzienNaSiatce("ŚRODA", 8, 0, 4, 100);
  ustawDzienNaSiatce("CZWARTEK", 11, 0, 4, 100);
  ustawDzienNaSiatce("PIĄTEK", 14, 0, 4, 100);  
  for(int i = 2; i < rows*3+6; i+=3)
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
	siatka->addWidget(linea, 2, j, rows*3+1, 1);
    }
}

void Window::drawGridH(int rows, int columns)
{
  ustawGodzinyNaSiatceH();
  ustawDzienNaSiatce("P\nO\nN\nI\nE\nD\nZ\nI\nA\nŁ\nE\nK", 1, 3, 174, 3);
  ustawDzienNaSiatce("W\nT\nO\nR\nE\nK", 1, 6, 174, 3);
  ustawDzienNaSiatce("Ś\nR\nO\nD\nA", 1, 9, 174, 3);
  ustawDzienNaSiatce("C\nZ\nW\nA\nR\nT\nE\nK", 1, 12, 174, 3);
  ustawDzienNaSiatce("P\nI\nĄ\nT\nE\nK", 1, 15, 174, 3);  
  
  for(int j = 2; j < columns*3+3; j+=3)
    {
      QFrame *linea = new QFrame();
      linea->setFrameShadow(QFrame::Plain);
      linea->setFrameShape(QFrame::VLine);
      linea->setLineWidth(2);
      siatka->addWidget(linea, 0, j, rows+3, 1);
    }
  for(int i = 0; i < rows+3; i++)
    {
      QFrame *linea1 = new QFrame();
      linea1->setFrameShape(QFrame::HLine);
      linea1->setFrameShadow(QFrame::Plain);
      if((i+6)%12 == 0)
	linea1->setLineWidth(3);
      if(i%3 == 0)
	siatka->addWidget(linea1, i, 2, 2, columns*3+1);
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
  if(tydzien == 0)
    czas += "     TN";
  else if (tydzien == 2)
    czas += "     TP";
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
      siatka->addWidget(blok, row*3+2+tydzien, column, rowSpan, length/5 + 1);
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
      siatka->addWidget(blok, column, row*3+2+tydzien, length/5 + 2, rowSpan);
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
  raiseHorizontalLines();
}

void Window::raiseHorizontalLines()
{
  if(splitter->orientation() == Qt::Vertical)
    for(int i = 2; i < 20; i+=3)
      for(int j = 0; j < 174; j++)
	siatka->itemAtPosition(i, j)->widget()->raise();
  else
    for(int i = 2; i < 18; i+=3)
      for(int j = 0; j < 174; j++)
	siatka->itemAtPosition(j, i)->widget()->raise();
}
