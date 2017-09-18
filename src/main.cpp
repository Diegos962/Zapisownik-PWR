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
  if(width < 1250)
    width = 1150;
  else
    width = 1250;
  if(height < 800)
    height = 750;
  else
    height = 830;
  
  Window main;
  main.show();
  main.setMinimumSize(width, height);
  return a.exec();
}
