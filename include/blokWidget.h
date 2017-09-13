#ifndef BLOKWIDGET_H
#define BLOKWIDGET_H
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include <QResizeEvent>
#include <QSplitter>

class BlokWidget: public QLabel
{
  Q_OBJECT
 public:
  explicit BlokWidget(QWidget *parent = 0);
  void setKodGrupy(const QString kg);
  void setKodKursu(const QString kk);
  QString kodGrupy() const;
  QString kodKursu() const; 
  private slots:
    void resizeFont();
 private:
    QTimer *resizeTimer;
    QString kod_grupy;
    QString kod_kursu;
    QLabel *duzy;
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

class Day: public QLabel
{
  Q_OBJECT
 public:
  explicit Day(QString text, QWidget *parent = 0);
  void enterEvent(QEvent *);
};

#endif
