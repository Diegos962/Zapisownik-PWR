#ifndef COLOR_DOUBLE_BUTTON_H
#define COLOR_DOUBLE_BUTTON_H

#include <QPushButton>
#include <QColor>
#include <QMouseEvent>

class ColorDoubleButton: public QPushButton
{
  Q_OBJECT
 public:
  explicit ColorDoubleButton(const QString &text, QWidget *parent = 0);
  void setColor(const QColor &clr);
  void setFontColor(const QColor &clr);
  QColor color() const;
  QColor fontColor() const;
 private:
  QColor colorBg;
  QColor fontColorFg;
  private slots:
    void mousePressEvent(QMouseEvent *e) override;
 signals:
    void rightClicked();
    void leftClicked();
};


#endif
