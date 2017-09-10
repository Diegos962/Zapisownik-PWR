#ifndef WYBRANE_KOLORY_H
#define WYBRANE_KOLORY_H
#include <QColor>
#include "color_dialog.h"

class PickColors;
class WybraneKolory
{
 public:
  QColor colorWyklady() const;
  QColor colorCwiczenia() const;
  QColor colorLaborki() const;
  QColor colorProjekty() const;
  QColor colorSeminaria() const;
  QColor colorInne() const;
  QColor fontColorWyklady() const;
  QColor fontColorCwiczenia() const;
  QColor fontColorLaborki() const;
  QColor fontColorProjekty() const;
  QColor fontColorSeminaria() const;
  QColor fontColorInne() const;
  void operator =(const PickColors *pick);
 private:
  QColor colorwyklady;
  QColor colorcwiczenia;
  QColor colorlaborki;
  QColor colorprojekty;
  QColor colorseminaria;
  QColor colorinne;
  QColor fontcolorWyklady;
  QColor fontcolorCwiczenia;
  QColor fontcolorLaborki;
  QColor fontcolorProjekty;
  QColor fontcolorSeminaria;
  QColor fontcolorInne;
};

#endif
