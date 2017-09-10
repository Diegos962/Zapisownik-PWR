#include <QApplication>
#include "../include/main_window.h"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setStyle("Fusion");
  Window main;
  main.show();
  main.setMinimumSize(1250, 800);
  return a.exec();
}
