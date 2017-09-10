#include "../include/wybrane_kolory.h"

void WybraneKolory::operator = (const PickColors *pick)
{
  colorwyklady = pick->colorWyklady();
  colorcwiczenia = pick->colorCwiczenia();
  colorlaborki = pick->colorLaborki();
  colorprojekty = pick->colorProjekty();
  colorseminaria = pick->colorSeminaria();
  colorinne = pick->colorInne();
  fontcolorWyklady = pick->fontColorWyklady();
  fontcolorCwiczenia = pick->fontColorCwiczenia();
  fontcolorLaborki = pick->fontColorLaborki();
  fontcolorProjekty = pick->fontColorProjekty();
  fontcolorSeminaria = pick->fontColorSeminaria();
  fontcolorInne = pick->fontColorInne();
}

QColor WybraneKolory::colorWyklady() const
{
  return colorwyklady;
}

QColor WybraneKolory::colorCwiczenia() const
{
  return colorcwiczenia;
}

QColor WybraneKolory::colorLaborki() const
{
  return colorlaborki;
}

QColor WybraneKolory::colorProjekty() const
{
  return colorprojekty;
}

QColor WybraneKolory::colorSeminaria() const
{
  return colorseminaria;
}

QColor WybraneKolory::colorInne() const
{
  return colorinne;
}

QColor WybraneKolory::fontColorWyklady() const
{
  return fontcolorWyklady;
}

QColor WybraneKolory::fontColorCwiczenia() const
{
  return fontcolorCwiczenia;
}

QColor WybraneKolory::fontColorLaborki() const
{
  return fontcolorLaborki;
}

QColor WybraneKolory::fontColorProjekty() const
{
  return fontcolorProjekty;
}

QColor WybraneKolory::fontColorSeminaria() const
{
  return fontcolorSeminaria;
}

QColor WybraneKolory::fontColorInne() const
{
  return fontcolorInne;
}
