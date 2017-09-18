#include <QApplication>
#include <QDesktopWidget>
#include "../include/main_window.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setStyle("Fusion");
  QRect rec = a.desktop()->screenGeometry();
  int height = rec.height();
  int width = rec.width();
  if(width < 1300)
    width = width - 100;
  else
    width = 1250;
  if(height < 900)
    height = height - 100;
  else
    height = 830;
  
  Window main;
  main.show();
  main.setMinimumSize(width, height);
  return a.exec();
}
