#ifndef COLOR_DIALOG_H
#define COLOR_DIALOG_H

#include <QDialog>
#include <QColor>
#include <QColorDialog>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QMouseEvent>
#include "color_double_button.h"
#include "wybrane_kolory.h"

class WybraneKolory;
class PickColors: public QDialog
{
  Q_OBJECT
 public:
    explicit PickColors(QWidget *parent = 0);
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
    void setColorWyklady(const QColor &clr);
    void setColorCwiczenia(const QColor &clr);
    void setColorLaborki(const QColor &clr);
    void setColorProjekty(const QColor &clr);
    void setColorSeminaria(const QColor &clr);
    void setColorInne(const QColor &clr);
    void setFontColorWyklady(const QColor &clr);
    void setFontColorCwiczenia(const QColor &clr);
    void setFontColorLaborki(const QColor &clr);
    void setFontColorProjekty(const QColor &clr);
    void setFontColorSeminaria(const QColor &clr);
    void setFontColorInne(const QColor &clr);
    void updateColors() const;
 private:
    ColorDoubleButton *wyklady;
    ColorDoubleButton *cwiczenia;
    ColorDoubleButton *laborki;
    ColorDoubleButton *projekty;
    ColorDoubleButton *seminaria;
    ColorDoubleButton *inne;
    ColorDoubleButton *KoloryJSOS;
    ColorDoubleButton *KoloryPlanPWR;
    QDialogButtonBox *buttons;
    void updateButton(ColorDoubleButton *bt) const;
    private slots:
      void JSOS_clicked() const;
      void PWR_clicked() const;
      void getColor();
      void getFontColor();
};


#endif
