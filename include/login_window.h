#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H
#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QComboBox>
#include <QList>
#include <QMap>
#include <QProgressBar>
#include <QPair>
#include <QTabWidget>
#include <QThread>
#include <QSpinBox>
#include <QEventLoop>
#include <QMessageBox>
#include "poboczneTerminy.h"
#include "kursy_grupy_bloki.h"

class LoginWindow: public QDialog
{
  Q_OBJECT
 public:
  explicit LoginWindow(QWidget *parent = 0);
  /************EDUKACJA*********/
 private:
  QNetworkAccessManager *mgr;
  QList<QString> semesters;
  QList<QString> id_semesters;
  QList<QString> zapisy;
  QPair<QString, QString> pairUczWpp;
  QMap<QString, QString> mapUczpps;
  QMap<QString, QString> mapSemestrId;
  QMap<QString, QString> ineSluId;
  QList<QString> id_zapisy;
  QList<KursyGrupyBloki> lista_zajec;
  QString clEduWebSESSIONTOKEN;
  QString clEduWebTOKEN;
  QString FolderDane;
  QString FolderZapisy;
  QString FolderKryterium;
  QString FolderPlanStudiow;
  QString FolderSemestr;

  QString get_token(const QString &token,
		    const QString &end_token, QNetworkReply *reply);
  void process();
  void GUI_wybor_kierunku(QTextStream &data);
  void GUI_select_semester();
  void PickSemester(QTextStream &reply);
  QMap<QString, QString> parse_uczppsid(QTextStream &stream,
					const QString &name,
					const QString &end_name);
  void PickZapisy(QNetworkReply *reply);
  void GUI_select_zapisy();
  void Wektor_Plan_wybor_GUI();
  void Select_PlanStudiow_GUI();
  void SelectSemestr_GUI(const bool &nr);
  void ZnajdzBlokiGrupyKursy(QTextStream &stream,
			     const QString &nazwa, const bool &nr);
  KursyGrupyBloki parseBloki(QTextStream &stream, QString line);
  int ilosc_stron(QTextStream &stream, const QString &name);
  void Kursy(QTextStream &streamx, const QString &plan_wektor,
	     const QString &kod);
  void Grupy(QTextStream &stream, const QString &kod);
  void Bloki(QTextStream &stream);
  void pobierz_kursy(QTextStream &stream);
  QString parseClassBiala(QTextStream &stream, const QString &name);
  QString semester_rowID(const QString &line);
  QString prase_value(QByteArray stream, const QString &name);
  QString parse_table(QTextStream &reply, const QString &wiersz,
		      QList<QString> &list);
  void rozdajBlokiGrupyKursy(const bool &nr);
  void parsujTermin(QTextStream &stream, Kurs &tmp);
  bool pytanieOWyborZrodla();
  void zakonczonoPobieranie();
  
  private slots:
    void AcceptLogin();
    void SelectMajor();
    void SelectSemester();
    void SelectZapisy();
    void select_Wektor_Plan();
    void Select_PlanStudiow();
    void SelectSemestr_ZPLANU();
    void SelectSemestr_ZWEKTORA();
 public:
    void GUI_login();
    QString folderDane() const;
    QString folderZapisy() const;
    QString folderKryterium() const;
    QString folderPlanStudiow() const;
    QString folderSemestr() const;
    /*****************************/
 
    /************AKZ**************/
 private:
    QCheckBox *jezyki, *wf, *inne;
    QPushButton *back, *download;
    QTabWidget *boxAKZ;
    QWidget *tabOgolne, *tabJezyki, *tabWF;
    QGridLayout *boxGridJezyki, *boxGridOgolne, *boxGridWF;
    QLabel *labelAngielski, *labelNiemiecki, *labelInneJezyki;
    QCheckBox *AngA, *AngB, *AngC, *NiemA, *NiemB, *NiemC,
      *InneA, *InneB, *InneC, *stacjonarne, *niestacjonarne,
      *stopienI, *stopienII, *sekcja;
    void filtrujAKZ(QList<Kurs> &tmp);
    void filtrujJezyki(const Kurs &i);
    private slots:
      void AKZ_download();
 public:
      void GUI_AKZ();
      /*****************************/

      /************POLWRO***********/
 private:
      int PolwroStrony(QTextStream &stream);
      void PolwroProwadzacy(QTextStream &stream);
      void PolwroPobieranie();
      void PolwroPolwro();
      QSpinBox *offset;
     
      private slots:
	void PolwroLogin();
	
 public:
	void PolwroGUI();
	QList<Prowadzacy> lista_prowadzacych;
	/*****************************/

	/************WSPOLNE***********/
	QList<Kurs> lista_kursow;
 private:
	QLabel *labelHead;
	QGridLayout *formGridLayout;
	QProgressBar *progressBar;
	QProgressBar *progressBarBot;
	QLineEdit *editUsername, *editPassword;                
	QDialogButtonBox *buttons;                              
	QComboBox *comboKierunki;
	QPushButton *buttonMajor;
	QComboBox *comboSemestr;
	
	QNetworkReply* post(const QUrlQuery &postData, const QString &adres);
	QNetworkReply* get_page(const QString &adres);
	bool error(QNetworkReply *reply);
	void change_head_error(const QString &error);
	void grey_out(const QString &text);
	void clear_window();
	bool if_logged(QNetworkReply *reply);
	void ActiveWidget(QWidget *w);
	QString return_href(const QString &line);
	QString usun_tagi(QString line);
};
 
#endif
