#include "../include/login_window.h"

void LoginWindow::PolwroGUI()
{
  labelHead->setText("Logowanie do polwro.pl");
  labelHead->setStyleSheet("QLabel {color: black; font-size: 18px;}");
  labelHead->setVisible(true);
  buttons->setEnabled(true);
  buttons->setVisible(true);
  editPassword->setEnabled(true);
  editPassword->setVisible(true);
  editUsername->setEnabled(true);
  editUsername->setVisible(true);
  editPassword->setText("");
  editUsername->setText("");
  
  offset->setVisible(true);
  offset->setRange(0, 500);
  offset->setSingleStep(20);
  offset->setSuffix(" ms");

  buttons->disconnect();
  connect(buttons, &QDialogButtonBox::rejected,
  	  this, &QDialog::reject);
  connect(buttons, &QDialogButtonBox::accepted,
  	  this, &LoginWindow::PolwroLogin);
}

void LoginWindow::PolwroLogin()
{
  labelHead->setVisible(true);
  offset->setVisible(false);
  grey_out("ŁĄCZENIE...");
  QUrlQuery postData;
  postData.addQueryItem("username", editUsername->text());
  postData.addQueryItem("password", editPassword->text());
  postData.addQueryItem("login", "Zaloguj");
  QNetworkReply *reply = post(postData, "http://polwro.pl/login.php");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      buttons->setEnabled(true);
      return;
    }
  reply = get_page("http://polwro.pl/index.php");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      buttons->setEnabled(true);
      return;
    }
  if(!if_logged(reply))
    {
      buttons->setEnabled(true);
      offset->setVisible(true);
      change_head_error("Błąd logowania/Złe dane");
      return;
    }
  PolwroPobieranie();
}

void LoginWindow::PolwroPobieranie()
{
  QString typ[9] = {"6", "7", "8", "9", "10", "11", "12", "25", "42"};
  QString nazwy[9] = {"Matematycy", "Fizycy", "Chemicy", "Elektronicy",
		      "Językowcy", "Humaniści", "Sportowcy", "Informatycy", "Inni"};
  progressBar->setVisible(true);
  progressBar->setValue(0);
  progressBarBot->setVisible(true);
  progressBarBot->setValue(0);
  progressBarBot->setRange(0, 100);
  editUsername->setVisible(false);
  editPassword->setVisible(false);
  labelHead->setText("POBIERANIE...");
  QNetworkReply *reply;
  
  for(int i = 0; i < 9; i++)
    {
      labelHead->setText("POBIERAM: " + nazwy[i]);
      progressBarBot->setValue(0);
      QString link = "http://polwro.pl/viewforum.php?f=" + typ[i] +"&topicdays=0&start=0";
      reply = get_page(link);
      QByteArray page = reply->readAll();
      QTextStream stream(&page);
      int strony = PolwroStrony(stream);
      for(int j = 0; j <= strony; j+=50)
	{
	  QString link = "http://polwro.pl/viewforum.php?f=" + typ[i] +"&topicdays=0&start=" + QString::number(j);
	  reply = get_page(link);
	  QByteArray page = reply->readAll();
	  QTextStream stream(&page);
	  stream.setCodec("ISO 8859-2");
	  PolwroProwadzacy(stream);
	  QThread::msleep(350 + offset->value());
	  progressBarBot->setValue((j+50)*100/(strony+50));
	}
      QThread::msleep(830 + offset->value());
      progressBar->setValue((i+1)*100/9);
    }
  qSort(lista_prowadzacych.begin(), lista_prowadzacych.end(), [](const Prowadzacy &s1, const Prowadzacy &s2) {return s1.nazwa() < s2.nazwa();});
  for(int i = 0; i < lista_prowadzacych.size()-1; i++)
    if(lista_prowadzacych[i].nazwa() == lista_prowadzacych[i+1].nazwa())
      lista_prowadzacych[i].setNazwa("2 " + lista_prowadzacych[i].nazwa());
  labelHead->setText("ZAKOŃCZONO POBIERANIE\nPOBRANO " + QString::number(lista_prowadzacych.size()) + " PROWADZĄCYCH");
  progressBarBot->setVisible(false);
  buttons->setVisible(false);
  ActiveWidget(buttonMajor);
  buttonMajor->setText("Zakończ");
  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
  	  this, &QDialog::accept);
}

int LoginWindow::PolwroStrony(QTextStream &stream)
{
  QString line;
  int pos1 = 0, pos2 = 0;
  int strona = 0;
  bool tematy = false;
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if(line.indexOf("<div class=\"vtimp\">Tematy") >= 0)
	tematy = true;
      while( (pos1 = line.indexOf("topicdays=0&amp;start=", pos2)) >= 0 )
	{
	  pos2 = line.indexOf("\"", pos1+22);
	  if(pos2 >= 0)
	    strona = qMax(strona, line.mid(pos1+22, pos2-pos1-22).toInt());
	}
    }
  if(!tematy)
    return -1;
  return strona;
}

void LoginWindow::PolwroProwadzacy(QTextStream &stream)
{
  bool start = false, end = false;
  QString line;
  Prowadzacy tmp;
  while(!stream.atEnd())
    {
      if(line.indexOf("<div class=\"vtimp\">Tematy") >= 0)
	start = true;
      line = stream.readLine();
      if(line.indexOf("<ul class=\"vfigntop\">") >= 0)
	end = true;
      if(start && !end)
	{
	  tmp.setLink(return_href(line));
	  line = usun_tagi(line);
	  int pos = line.indexOf(" ");
	  tmp.setOcena(line.left(pos));
	  tmp.setNazwa(line.right(line.size() - pos - 1));
	  line = stream.readLine();
	  pos = line.indexOf("Opinii: ") + 8;
	  tmp.setIloscOpinii(line.mid(pos, line.indexOf("<", pos) - pos));
	  lista_prowadzacych.push_back(tmp);
	}
    }
}

