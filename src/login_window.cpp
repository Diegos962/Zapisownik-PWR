#include "../include/login_window.h"

LoginWindow::LoginWindow(Dropbox *d, QWidget *parent): QDialog(parent)
{
  setWindowTitle("Logowanie");
  setModal(true);
  db = d;
  progressBar = new QProgressBar(this);
  progressBarBot = new QProgressBar(this);
  formGridLayout = new QGridLayout(this);
  editUsername = new QLineEdit(this);
  editPassword = new QLineEdit(this);
  labelHead = new QLabel("Podaj dane do logowania", this);
  buttons = new QDialogButtonBox(this);
  mgr = new QNetworkAccessManager(this);
  mgr->setCookieJar(new QNetworkCookieJar(mgr));
  comboKierunki = new QComboBox(this);
  comboSemestr = new QComboBox(this);
  buttonMajor = new QPushButton("Wybierz", this);
  back = new QPushButton("Cofnij", this); 
  download = new QPushButton("Pobierz", this);
  jezyki = new QCheckBox("Języki Obce", this);
  wf = new QCheckBox("WF", this);
  inne = new QCheckBox("Inne", this);
  boxAKZ = new QTabWidget(this);
  tabOgolne = new QTabBar(boxAKZ);
  tabJezyki = new QTabBar(boxAKZ);
  tabWF = new QTabBar(boxAKZ);
  boxGridJezyki = new QGridLayout(tabJezyki);
  boxGridOgolne = new QGridLayout(tabOgolne);
  boxGridWF = new QGridLayout(tabWF);
  labelAngielski = new QLabel("J. Ang.", tabJezyki);
  labelNiemiecki = new QLabel("J. Niem.", tabJezyki);
  labelInneJezyki = new QLabel("Inne", tabJezyki);
  AngA = new QCheckBox("A", tabJezyki);
  AngB = new QCheckBox("B", tabJezyki);
  AngC = new QCheckBox("C", tabJezyki);
  NiemA = new QCheckBox("A", tabJezyki);
  NiemB = new QCheckBox("B", tabJezyki);
  NiemC = new QCheckBox("C", tabJezyki);
  InneA = new QCheckBox("A", tabJezyki);
  InneB = new QCheckBox("B", tabJezyki);
  InneC = new QCheckBox("C", tabJezyki);
  stacjonarne = new QCheckBox("Stacjonarne", tabOgolne);
  niestacjonarne = new QCheckBox("Niestacjonarne", tabOgolne);
  stopienI = new QCheckBox("I stopień", tabOgolne);
  stopienII = new QCheckBox("II stopień", tabOgolne);
  sekcja = new QCheckBox("Nie pokazuj kursów dla sekcji", tabWF);
  offset = new QSpinBox(this);

  formGridLayout->addWidget(labelHead, 0, 0, 1, 4, Qt::AlignCenter);
  formGridLayout->addWidget(editUsername, 1, 0, 1, 4);
  formGridLayout->addWidget(editPassword, 2, 0, 1, 4);
  formGridLayout->addWidget(buttons, 3, 1, 1, 3);
  formGridLayout->addWidget(offset, 3, 0);
  formGridLayout->addWidget(comboKierunki, 1, 0, 1, 4, Qt::AlignCenter);
  formGridLayout->addWidget(comboSemestr, 1, 0, 1, 4, Qt::AlignCenter);
  formGridLayout->addWidget(buttonMajor, 2, 0, 1, 4, Qt::AlignCenter);
  formGridLayout->addWidget(progressBar, 1, 0, 1, 4, Qt::AlignCenter);
  formGridLayout->addWidget(progressBarBot, 2, 0, 1, 4, Qt::AlignCenter);
  setLayout(formGridLayout);

  editPassword->setEchoMode(QLineEdit::Password);
  editPassword->setPlaceholderText("Hasło");
  editUsername->setPlaceholderText("Login");
  back->setVisible(false);

  buttons->addButton(QDialogButtonBox::Ok);
  buttons->addButton(QDialogButtonBox::Cancel);
  buttons->button(QDialogButtonBox::Ok)->setText("Zaloguj");
  buttons->button(QDialogButtonBox::Cancel)->setText("Anuluj");
  clear_window();
}

void LoginWindow::GUI_login()
{
  editUsername->setVisible(true);
  editPassword->setVisible(true);
  buttons->setVisible(true);
  comboKierunki->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  connect(buttons, &QDialogButtonBox::rejected,
  	  this, &QDialog::reject);
  connect(buttons, &QDialogButtonBox::accepted,
  	  this, &LoginWindow::AcceptLogin);
}

void LoginWindow::AcceptLogin()
{
  grey_out("LOGOWANIE...");
  QNetworkReply *reply = get_page("https://edukacja.pwr.wroc.pl/EdukacjaWeb/studia.do");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  
  clEduWebTOKEN = get_token("cl.edu.web.TOKEN\" value=\"", "\">", reply);
  QUrlQuery postData;
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("login", editUsername->text());
  postData.addQueryItem("password", editPassword->text());
  reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/logInUser.do");
  
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      buttons->setEnabled(true);
      return;
    }
  if(!if_logged(reply))
    {
      buttons->setEnabled(true);
      change_head_error("Błąd logowania/Złe dane");
      return;
    }
  clEduWebSESSIONTOKEN = get_token("clEduWebSESSIONTOKEN=", "&event", reply);
  clear_window();
  process();
}

void LoginWindow::process()
{
  QString zapisy = "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?clEduWebSESSIONTOKEN="
    + clEduWebSESSIONTOKEN + "&event=WyborSluchacza";

  QNetworkReply *reply = get_page(zapisy);
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);

  if(page.indexOf("Wybierz przedmiot kształcenia") != -1)
    GUI_wybor_kierunku(stream);
  else
    {
      PickSemester(stream);
      GUI_select_semester();
    }
}
void LoginWindow::GUI_wybor_kierunku(QTextStream &data)
{
  labelHead->setText("Wybierz kierunek kształcenia");

  ineSluId = parse_uczppsid(data, "ineSluId", "\"");

  for(auto i: ineSluId.keys())
    comboKierunki->addItem(i);
  
  ActiveWidget(comboKierunki);
  ActiveWidget(buttonMajor);
  connect(buttonMajor, &QPushButton::clicked,
	  this, &LoginWindow::SelectMajor);
}

void LoginWindow::SelectMajor()
{
  if(comboKierunki->currentIndex() == -1)
    return;
  grey_out("ŁĄCZENIE...");
  QUrlQuery postData;
  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("ineSluId", ineSluId.value(comboKierunki->currentText()));
  postData.addQueryItem("event_WyborSluchaczaSubmit", "Wybierz");
  QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);
  PickSemester(stream);
  clear_window();
  GUI_select_semester();
}

void LoginWindow::PickSemester(QTextStream &reply)
{
  QString line, semester = "", semester2 = "";
  bool start = false, stop = false;
  while(!reply.atEnd())
    {
      line = reply.readLine();
      if(line.indexOf("Semestry", 0) != -1)
        start = true;
      else if(line.indexOf("pagingPanel: start", 0) != -1)
	stop = true;
      else if( (line.indexOf("WYBRANA", 0) != -1) && start && !stop)
	semester += parse_table(reply, line, id_semesters);
      else if( (line.indexOf("BIALA", 0) != -1) && start && !stop)
	semester2 += parse_table(reply, line, id_semesters);
    }
  if(semester != "")
    semesters.append(semester);
  if(semester2 != "")
    semesters.append(semester2);
}

void LoginWindow::GUI_select_semester()
{
  labelHead->setText("Wybierz semestr");
  for(int i = 0; i < comboKierunki->count();)
    comboKierunki->removeItem(0);
  ActiveWidget(comboKierunki);
  ActiveWidget(buttonMajor);
  for(int i = 0; i < semesters.size(); i++)
    comboKierunki->addItem(semesters.at(i).simplified());

  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
	  this, &LoginWindow::SelectSemester);
}

void LoginWindow::SelectSemester()
{
  if(comboKierunki->currentIndex() == -1)
    return;
  FolderDane = comboKierunki->currentText();
  grey_out("ŁĄCZENIE...");
  QString zapisy = "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?clEduWebSESSIONTOKEN=" + clEduWebSESSIONTOKEN
    + "&event=wyborSemestruRow&rowId="+id_semesters[comboKierunki->currentIndex()]
    +"&positionIterator=SluchaczSemestrySzczegolyROViewIterator&href=";

  QNetworkReply *reply = get_page(zapisy);
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  PickZapisy(reply);
  clear_window();
  GUI_select_zapisy();
}

void LoginWindow::PickZapisy(QNetworkReply *reply)
{
  QString line, zapis;
  bool start = false;
  QByteArray page = reply->readAll();
  QTextStream stream(&page);
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if(line.indexOf("<!-- zapisy: poczatek -->", 0) != -1)
	start = true;
      else if( (line.indexOf("Wybierz wiersz", 0) != -1) && start)
	zapisy.append(parse_table(stream, line, id_zapisy).simplified());
    }
}

void LoginWindow::GUI_select_zapisy()
{
  labelHead->setText("Wybierz zapisy");
  for(int i = 0; i < comboKierunki->count();)
    comboKierunki->removeItem(0);
  ActiveWidget(comboKierunki);
  ActiveWidget(buttonMajor);
  for(int i = 0; i < zapisy.size(); i++)
    comboKierunki->addItem(zapisy.at(i));

  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
  	  this, &LoginWindow::SelectZapisy);
}

void LoginWindow::SelectZapisy()
{
  if(comboKierunki->currentIndex() == -1)
    return;
  FolderZapisy = comboKierunki->currentText();
  grey_out("ŁĄCZENIE");
  QUrlQuery postData;
  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("rowId", id_zapisy[comboKierunki->currentIndex()]);
  postData.addQueryItem("positionIterator", "ZapisyROViewIterator");
  postData.addQueryItem("event_ZapisyPrzegladanieGrup", "Przeglądanie grup");
  QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);

  if(page.indexOf("http://akz.pwr.wroc.pl", 0) != -1)
    {
      clear_window();
      back->setVisible(true);
      GUI_AKZ();
    }
  else if(page.indexOf("Parametry wyszukiwania", 0) == -1)
    {
      change_head_error("Brak dostępu do zapisów");
      QString fDane, folderPliku;
      fDane = folderDane();
      fDane.replace('/', '-');
      fDane.insert(9, '/');
      int pos = 0;
      if( (pos = fDane.indexOf("DWU")) != -1)
	fDane = fDane.left(pos+3);
      fDane.insert(fDane.indexOf("PO"), "/");
      folderPliku = "/kursy/" + fDane + "/" + folderZapisy().replace('/', '-') + "/";
      dropboxDialog d(db, folderPliku);
      d.show();
      int ret = d.exec();
      if(ret)
	{
	  lista_kursow = d.listaK;
	  QDialog::done(2);
	}
      else
	QDialog::reject();
      return;
    }
  else
    {
      clear_window();
      Wektor_Plan_wybor_GUI();
    }
}

void LoginWindow::Wektor_Plan_wybor_GUI()
{
  labelHead->setText("Wybierz kryterium filtrowania");
  for(int i = 0; i < comboKierunki->count();)
    comboKierunki->removeItem(0);
  comboKierunki->addItem("KRYTERIUM FILTROWANIA");
  comboKierunki->addItem("Z PLANU");
  comboKierunki->addItem("Z WEKTORA");
  comboKierunki->addItem("KURSY POWTÓRKOWE");
  comboKierunki->addItem("KURSY ZALEGŁE");
  ActiveWidget(comboKierunki);
  ActiveWidget(buttonMajor);
  
  buttonMajor->setVisible(true);
  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
	  this, &LoginWindow::select_Wektor_Plan);
}

QString LoginWindow::prase_value(QByteArray stream, const QString &name)
{
  int pos1 = stream.indexOf(name);
  int pos2 = stream.indexOf("value=\"", pos1) + 7;
  int pos3 = stream.indexOf("\"", pos2);
  return stream.mid(pos2, pos3-pos2);
}

QMap<QString, QString> LoginWindow::parse_uczppsid(QTextStream &stream, const QString &name, const QString &end_name)
{
  QString line;
  bool start = false, end = false;
  int pos1 = 0, pos2 = 0;
  QMap<QString, QString> map;
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if( line.indexOf(name) >= 0)
	{
	  start = true;
	  line = stream.readLine();
	}
      if( line.indexOf("</select>") >= 0 && start)
	end = true;
      if(start && !end)
	if( (pos1 = line.indexOf("option value=\"") + 14) >= 0)
	  if( (pos2 = line.indexOf(end_name, pos1)) >= 0)
	    map.insert(stream.readLine().simplified(), line.mid(pos1, pos2-pos1).simplified());
    }
  return map;
}

void LoginWindow::select_Wektor_Plan()
{
  QString Kryterium[4] = {"Z_PLANU", "Z_WEKTORA_ZAP", "KURSY_POWTORKOWE", "KURSY_ZALEGLE"};

  if(comboKierunki->currentIndex() < 1)
    return;
  grey_out("ŁĄCZENIE");
  QUrlQuery postData;
  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("KryteriumFiltrowania", Kryterium[comboKierunki->currentIndex()-1]);
  QNetworkReply *reply = post(postData,
			      "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?event=ZapiszFiltr&event=wyborKryterium&href=");
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);
  FolderKryterium = comboKierunki->currentText();
  if(comboKierunki->currentIndex() == 1)
    {
      pairUczWpp.first = prase_value(page, "uczWppNazwa");
      pairUczWpp.second = prase_value(page, "uczWppId");
      mapUczpps = parse_uczppsid(stream, "UczPpsId", "\"");
      stream.seek(0);
      mapSemestrId = parse_uczppsid(stream, "UczSppId", "\" selected");
      clear_window();
      Select_PlanStudiow_GUI();
    }
  else if(comboKierunki->currentIndex() == 2)
    {
      pairUczWpp.first = prase_value(page, "kfkPkSymbol");
      pairUczWpp.second = prase_value(page, "kfkPkId");
      mapSemestrId = parse_uczppsid(stream, "NrSemestru", "\"");
      clear_window();
      SelectSemestr_GUI(false);
    }
  else if(comboKierunki->currentIndex() == 3)
    {
      grey_out("ŁĄCZENIE");
      ZnajdzBlokiGrupyKursy(stream, "Kursy powtórkowe", false);
    }
  else if(comboKierunki->currentIndex() == 4)
    {
      grey_out("ŁĄCZENIE");
      ZnajdzBlokiGrupyKursy(stream, "Kursy zaległe", false);
    }
}

void LoginWindow::Select_PlanStudiow_GUI()
{
  labelHead->setText("Wybierz plan studiów");
  for(int i = 0; i < comboKierunki->count();)
    comboKierunki->removeItem(0);
  
  comboKierunki->addItem("PLAN STUDIÓW");
  for(auto i: mapUczpps.keys())
    comboKierunki->addItem(i);
  ActiveWidget(comboKierunki);
  ActiveWidget(buttonMajor);
  
  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
	  this, &LoginWindow::Select_PlanStudiow);  
}

void LoginWindow::Select_PlanStudiow()
{
  if(comboKierunki->currentIndex() < 1)
    return;
  FolderPlanStudiow = comboKierunki->currentText();
  grey_out("ŁĄCZENIE");
  QUrlQuery postData;
  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("KryteriumFiltrowania", "Z_PLANU");
  postData.addQueryItem("uczWppNazwa", pairUczWpp.first);
  postData.addQueryItem("uczWppNazwa", pairUczWpp.second);
  postData.addQueryItem("uczWppId", pairUczWpp.second);
  postData.addQueryItem("UczPpsId", mapUczpps.value(comboKierunki->currentText()));
  postData.addQueryItem("UczSppId", mapSemestrId.first());
  QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?event=ZapiszFiltr&event=wyborPlanu&href=");

  mapSemestrId.remove(mapSemestrId.first());
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);
  mapSemestrId = parse_uczppsid(stream, "UczSppId", "\"");
  clear_window();
  SelectSemestr_GUI(true);
}

void LoginWindow::SelectSemestr_GUI(const bool &nr)
{
  labelHead->setText("Wybierz semestr");
  comboSemestr->addItem("SEMESTR");
  for(auto i: mapSemestrId.keys())
    comboSemestr->addItem(i);
  comboSemestr->setVisible(true);
  ActiveWidget(buttonMajor);
  ActiveWidget(comboSemestr);
  
  buttonMajor->disconnect();
  if(nr)
    connect(buttonMajor, &QPushButton::clicked,
	  this, &LoginWindow::SelectSemestr_ZPLANU);
  else
    connect(buttonMajor, &QPushButton::clicked,
	    this, &LoginWindow::SelectSemestr_ZWEKTORA);
}

void LoginWindow::SelectSemestr_ZPLANU()
{
  if(comboSemestr->currentIndex() < 1)
    return;
  FolderSemestr = comboSemestr->currentText();
  grey_out("ŁĄCZENIE");
  QUrlQuery postData;
  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("KryteriumFiltrowania", "Z_PLANU");
  postData.addQueryItem("uczWppNazwa", pairUczWpp.first);
  postData.addQueryItem("uczWppNazwa", pairUczWpp.second);
  postData.addQueryItem("uczWppId", pairUczWpp.second);
  postData.addQueryItem("UczPpsId", mapUczpps.value(comboKierunki->currentText()));
  postData.addQueryItem("UczSppId", mapSemestrId.value(comboSemestr->currentText()));
  QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?event=ZapiszFiltr&event=wyborSemestru&href=");
  
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);
  ZnajdzBlokiGrupyKursy(stream, "Kursy/Bloki/Grupy", true);
}

void LoginWindow::SelectSemestr_ZWEKTORA()
{
  if(comboSemestr->currentIndex() < 1)
    return;
  FolderSemestr = comboSemestr->currentText();
  grey_out("ŁĄCZENIE");
  QUrlQuery postData;
  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
  postData.addQueryItem("KryteriumFiltrowania", "Z_WEKTORA_ZAP");
  postData.addQueryItem("kfkPkSymbol", pairUczWpp.first);
  postData.addQueryItem("kfkPkSymbol", pairUczWpp.second);
  postData.addQueryItem("kfkPkId", pairUczWpp.second);
  postData.addQueryItem("NrSemestru", mapSemestrId.value(comboSemestr->currentText()));
  QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?event=ZapiszFiltr&event=wyborNrSemestru&href=");
  
  if(error(reply))
    {
      change_head_error("Błąd połączenia");
      return;
    }
  QByteArray page = reply->readAll();
  QTextStream stream(&page);
  ZnajdzBlokiGrupyKursy(stream, "Kursy z wektora zapisowego", false);
}

bool LoginWindow::pytanieOWyborZrodla()
{
  QString folderPliku, nazwaPliku, fDane;
  fDane = folderDane();
  fDane.replace('/', '-');
  fDane.insert(9, '/');
  int pos = 0;
  if( (pos = fDane.indexOf("DWU")) != -1)
    fDane = fDane.left(pos+3);
  fDane.insert(fDane.indexOf("PO"), "/");
  folderPliku = "/kursy/" + fDane + "/" + folderZapisy().replace('/', '-') + "/" + folderKryterium().replace('/', '-')
    + "/" + folderPlanStudiow().replace('/', '-') + "/" + folderSemestr().replace('/', '-') + "/";
  QList<QDropboxFileInfo> terminyDB = db->filterData(folderPliku);
  if(terminyDB.size() > 0)
    {
      QDropboxFileInfo info = terminyDB.at(terminyDB.size()-1);
      QString nazwaPliku = info.root() + info.path();
      QString czasNowego = info.modified().toTimeSpec(Qt::LocalTime).toString("dd.MM.yyyy hh.mm");
      QString pytanie = (QString("Na serwerze znajduje się plik z dnia %2.\nMożesz go pobrać i odciążyć serwery edukacji!"))
        .arg(czasNowego);
      QMessageBox msg;
      msg.addButton(QMessageBox::No)->setText("Pobierz z edukacji");
      msg.addButton(QMessageBox::Yes)->setText("Pobierz z zew. serwera");
      msg.setText(pytanie);
      msg.setWindowTitle("Wybór źródła");
      int ret = msg.exec();
      if(ret == QMessageBox::Yes)
	{
	  QByteArray data = db->downloadFile(nazwaPliku);
	  if(data.size() == 0)
	    return false;

	  progressBar->setRange(0, 100);
	  progressBar->setValue(0);
	  QTextStream stream(data);
	  while(!stream.atEnd())
	    {
	      Kurs aaa;
	      aaa.setKodKursu(stream.readLine());
	      aaa.setKodGrupy(stream.readLine());
	      aaa.setNazwa(stream.readLine());
	      aaa.setForma(stream.readLine());
	      aaa.setProwadzacy(stream.readLine().split("||", QString::SkipEmptyParts));
	      aaa.setTermin(stream.readLine());
	      aaa.setMiejsca(stream.readLine());
	      aaa.setPotok(stream.readLine());
	      lista_kursow.push_back(aaa);
	    }
	  return true;
	}
    }
  return false;
}

void LoginWindow::ZnajdzBlokiGrupyKursy(QTextStream &stream, const QString &nazwa, const bool &nr)
{
  clear_window();
  progressBar->setVisible(true);
  progressBar->setRange(0, 100);
  progressBar->setValue(0);
  labelHead->setText("POBIERANIE");
  if(pytanieOWyborZrodla())
    {
      QDialog::done(2);
      return;
    }
  bool start = false, end = false;
  QString line;
  KursyGrupyBloki tmp;

  int strony = ilosc_stron(stream, "positionIterRangeStartKGB");
  stream.seek(0);
  if(strony == 0)
    strony = ilosc_stron(stream, "positionIterRangeStartKwGB");
  stream.seek(0);
  stream.device()->reset();
  stream.setCodec("UTF-8");

  for(int j = 0; j < strony*10; j+= 10)
    {
      if(!nr)
	{
	  QUrlQuery postData;
	  postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
	  postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
	  postData.addQueryItem("pagingIterName", "KursyWEKROViewIterator");
	  postData.addQueryItem("pagingRangeStart", QString::number(j));
	  postData.addQueryItem("event", "positionIterRangeStartKwGB");
	  QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?href=");
  
	  if(error(reply))
	    {
	      change_head_error("Błąd połączenia");
	      return;
	    }
	  QByteArray page = reply->readAll();
	  stream << page;
	  stream.seek(0);
	}
      while(!stream.atEnd())
	{
	  line = stream.readLine();
	  if(line.indexOf(nazwa) >= 0)
	    start = true;
	  if(   line.indexOf("Kursy/Grupy w bloku") >= 0
		|| line.indexOf("Kursy w grupie") >= 0
		|| line.indexOf("Termin, budynek, sala") >= 0)
	    end = true;
	  if(start && !end)
	    {
	      if(line.indexOf("Wybierz wiersz") >= 0)
		{
		  tmp = parseBloki(stream, line);
		  lista_zajec.push_back(tmp);
		}
	    }
	}
      start = false;
      end = false;
      stream.device()->reset();
    }
  rozdajBlokiGrupyKursy(nr);
}

void LoginWindow::zakonczonoPobieranie()
{
  QString koniec = "ZAKOŃCZONO POBIERANIE.\nPOBRANO "
    + QString::number(lista_kursow.size()) + " KURSÓW";
  labelHead->setText(koniec);
  progressBar->setVisible(false);

  ActiveWidget(buttonMajor);
  buttonMajor->setText("Zakończ");
  buttonMajor->disconnect();
  connect(buttonMajor, &QPushButton::clicked,
	  this, &QDialog::accept);
}

void LoginWindow::rozdajBlokiGrupyKursy(const bool &nr)
{
  int count = 0;
  if(nr)
    {
      for(auto a: lista_zajec)
	{
	  labelHead->setText("POBIERAM: " + a.nazwa());
	  QString link = "https://edukacja.pwr.wroc.pl" + a.link();
	  QNetworkReply *reply = get_page(link);
	  QByteArray page = reply->readAll();
	  QTextStream stream(&page);

	  if(a.rodzaj().indexOf("Blok") >= 0)
	    Bloki(stream);
	  else if(a.rodzaj().indexOf("Grupa") >= 0)
	    Grupy(stream, "GK0_");
	  else
	    Kursy(stream, "_PLN_", "K00_");
	  count+=100;
	  progressBar->setValue(count/lista_zajec.size());
	}
    }
  else
    {
      for(auto a: lista_zajec)
	{
	  labelHead->setText("POBIERAM: " + a.kod() + " - " + a.rodzaj().simplified().right(1));
	  QString link = "https://edukacja.pwr.wroc.pl" + a.link();
	  QNetworkReply *reply = get_page(link);
	  QByteArray page = reply->readAll();
	  QTextStream stream(&page);
	  Kursy(stream, "WEK", "");
	  count+=100;
	  progressBar->setValue(count/lista_zajec.size());
	}
    }
  zakonczonoPobieranie();
}
  

KursyGrupyBloki LoginWindow::parseBloki(QTextStream &stream, QString line)
{
  KursyGrupyBloki tmp;
  tmp.setLink(return_href(line.simplified()));
  line += stream.readLine();
  tmp.setRodzaj(usun_tagi(line.simplified()));
  for(int i = 0; i < 2;)
    {
      line = stream.readLine();
      if(line.indexOf("BIALA") >= 0 || line.indexOf("WYBRANA") >= 0)
	{
	  line = stream.readLine();
	  if(i == 0)
	    tmp.setKod(line.simplified());
	  else if(i == 1)
	    tmp.setNazwa(line.simplified());
	  i++;
	}
    }
  return tmp;
}

int LoginWindow::ilosc_stron(QTextStream &stream, const QString &name)
{
  QString line, liczba;
  bool start = false, end = false, ok;
  int max = 0;
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if(line.indexOf(name) >= 0)
	start = true;
      if(line.indexOf("</span>") >= 0 && start)
	end = true;
      if(start && !end)
	{
	  if(line.indexOf("pagingRangeStart") >= 0)
	    {
	      int pos1 = line.indexOf("value=\"") + 7;
	      int pos2 = line.indexOf("\"", pos1);
	      liczba = line.mid(pos1, pos2-pos1);
	      int number = liczba.toInt(&ok);
	      if(ok)
		max = number;
	    }
	}
    }
  return max;
}

void LoginWindow::Bloki(QTextStream &stream)
{
  bool start = false, end = false;
  QString line;
  QList<KursyGrupyBloki> GrupyWBloku;
  KursyGrupyBloki tmp;
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if(line.indexOf("Grupy w bloku") >= 0)
	start = true;
      if(line.indexOf("Kursy w grupie") >= 0 && start)
	end = true;
      if(start && !end)
	{
	  if(line.indexOf("Wybierz wiersz") >= 0)
	    {
	      tmp = parseBloki(stream, line);
	      GrupyWBloku.push_back(tmp);
	    }
	}
    }
  for(auto a: GrupyWBloku)
    {
      QString link = "https://edukacja.pwr.wroc.pl" + a.link();
      
      QNetworkReply *reply = get_page(link);
      QByteArray page = reply->readAll();
      QTextStream stream(&page);

      if(a.rodzaj().indexOf("Grupa") >= 0)
	Grupy(stream, "BGK_");
      else
	Kursy(stream, "_PLN_", "BK0_");
    }
}

void LoginWindow::Grupy(QTextStream &stream, const QString &kod)
{
  bool start = false, end = false;
  QString line;
  QList<QString> KursyWGrupie;
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if(line.indexOf("Kursy w grupie") >= 0)
	start = true;
      if(line.indexOf("Grupy zajęciowe") >= 0 && start)
	end = true;
      if(start && !end)
	{
	  if(line.indexOf("Wybierz wiersz") >= 0)
	    KursyWGrupie.push_back(return_href(line.simplified()));
	}
    }
  for(auto a: KursyWGrupie)
    {
      QString link = "https://edukacja.pwr.wroc.pl" + a;
      
      QNetworkReply *reply = get_page(link);
      QByteArray page = reply->readAll();
      QTextStream stream(&page);
      Kursy(stream, "_PLN_", kod);
    }
}

void LoginWindow::Kursy(QTextStream &streamx, const QString &plan_wektor,
			const QString &kod)
{
  int strony = ilosc_stron(streamx, "positionIterRangeStartGZK");

  streamx.seek(0);
  QString line;

  QString pagingIterName = "GrupyZajecioweKursu" + plan_wektor + kod + "ROViewIterator";

  for(int j = 0; j < strony*10; j+= 10)
    {
      QUrlQuery postData;
      postData.addQueryItem("clEduWebSESSIONTOKEN", clEduWebSESSIONTOKEN);
      postData.addQueryItem("cl.edu.web.TOKEN", clEduWebTOKEN);
      postData.addQueryItem("pagingIterName", pagingIterName);
      postData.addQueryItem("pagingRangeStart", QString::number(j));
      postData.addQueryItem("event", "positionIterRangeStartGZK");
      QNetworkReply *reply = post(postData, "https://edukacja.pwr.wroc.pl/EdukacjaWeb/zapisy.do?href=");
  
      if(error(reply))
	{
	  change_head_error("Błąd połączenia");
	  return;
	}
      QByteArray page = reply->readAll();
      QTextStream stream(&page);
      pobierz_kursy(stream);
    }
}

QString LoginWindow::parseClassBiala(QTextStream &stream, const QString &name)
{
  QString line, namex;
  while(line.indexOf(name) == -1)
    {
      line = stream.readLine();
      if(line.indexOf("Uwagi") >= 0)
	for(int l = 0; l < 3; l++)
	  line = stream.readLine();
      namex += line;
    }
  return namex;
}

void LoginWindow::parsujTermin(QTextStream &stream, Kurs &tmp)
{
  tmp.setTermin("");
  QStringList terminys = parseClassBiala(stream, "</table>").split("</td>", QString::SkipEmptyParts);
  for(auto &opk: terminys)
    {
      opk = usun_tagi(opk).simplified();
      if(opk.size() == 0)
	terminys.removeOne(opk);
    }
  QStringList nawiasy;
  for(auto opk: terminys)
    {
      QString jeden;
      terminy(opk, jeden);
      if(!sprawdzTerminy(jeden))
	jeden = "";
      else
	{
	  if(opk[2].digitValue() == 0)
	    tmp.setTermin(tmp.termin() + jeden);
	  else
	    nawiasy.push_back(jeden);
	}
    }
  if(tmp.termin().size() == 0)
    {
      int max = 0;
      QString max_count = "";
      for(auto opk: nawiasy)
	{
	  int l = nawiasy.count(opk);
	  if(l > max)
	    {
	      max = l;
	      max_count = opk;
	    }
	}
      tmp.setTermin(tmp.termin() + max_count);
    }
}

void LoginWindow::pobierz_kursy(QTextStream &stream)
{
  int i = 0;
  bool start = false;
  QString line;
  Kurs tmp;
  stream.setCodec("UTF-8");
  while(!stream.atEnd())
    {
      line = stream.readLine();
      if(line.indexOf("Termin, budynek, sala") >= 0)
	start = true;
      if(line.indexOf("Uwagi") >= 0)
	for(int l = 0; l < 3; l++)
	  line = stream.readLine();
      if(start)
	{
	  if(line.indexOf("BIALA") >= 0)
	    {
	      switch(i)
		{
		case 0:
		  tmp.setKodGrupy(usun_tagi(parseClassBiala(stream, "</td>")).simplified());
		  break;
		case 1:
		  tmp.setKodKursu(usun_tagi(parseClassBiala(stream, "</td>")).simplified());
		  break;
		case 2:
		  tmp.setNazwa(usun_tagi(parseClassBiala(stream, "</td>")).simplified());
		  break;
		case 3:
		  {
		    QStringList miejsca = usun_tagi(parseClassBiala(stream, "</td>")).simplified().split("/", QString::SkipEmptyParts);
		    if(miejsca.size() != 2)
		      tmp.setMiejsca("-");
		    else
		      {
			bool ok, ok1;
			int m1 = miejsca[0].toInt(&ok);
			int m2 = miejsca[1].toInt(&ok1);
			if(!ok || ! ok1)
			  tmp.setMiejsca("-");
			else
			  tmp.setMiejsca(QString::number(m2-m1));
		      }
		    break;
		  }
		case 4:
		  parseClassBiala(stream, "</td>");
		  break;
		case 5:
		  parseClassBiala(stream, "</td>");
		  break;
		case 6:
		  usun_tagi(parseClassBiala(stream, "</td>"));
		  break;
		case 7:
		  parseClassBiala(stream, "</table>");
		  break;
		case 8:
		  tmp.prowadzacy().push_back(usun_tagi(parseClassBiala(stream, "</td>")).simplified());
		  break;
		case 9:
		  tmp.setForma(usun_tagi(parseClassBiala(stream, "</td>")).simplified());
		  break;
		case 10:
		  parseClassBiala(stream, "</td>");
		  break;
		case 11:
		  parseClassBiala(stream, "</td>");
		  break;
		case 12:
		  parseClassBiala(stream, "</td>");
		  break;
		case 13:
		  tmp.setPotok(usun_tagi(parseClassBiala(stream, "</td>")).simplified());
		  break;
		case 14:
		  parsujTermin(stream, tmp);
		  break;
		}
	      i++;
	    }
	  if(i==15)
	    {
	      lista_kursow.push_back(tmp);
	      tmp.prowadzacy().clear();
	      i = 0;
	    }
	}
    }
}

QString LoginWindow::parse_table(QTextStream &reply, const QString &wiersz,
				 QList<QString> &list)
{
  QString name, line = wiersz, id="0";
  int pos1 = 0, pos2 = 0;
  while(line.indexOf("</td>", 0) == -1)
    {
      id = semester_rowID(line);
      if(id != "0")
	list.append(id);
      id = "0";
      while( (pos1 = line.indexOf("<", 0)) >= 0
  	     && (pos2 = line.indexOf(">", pos1)) >= 0)
  	line = line.remove(pos1, pos2-pos1+1);
      name += line;
      line = reply.readLine();
    }
  return name;
}

QString LoginWindow::semester_rowID(const QString &line)
{
  int pos1 = 0, pos2 = 0;
  pos1 = line.indexOf("rowId=", 0) +6;
  pos2 = line.indexOf("&position", pos1);
  if(pos1 >=0 && pos2 >=0)
    return line.mid(pos1, pos2-pos1);
  return "0";
}

QString LoginWindow::get_token(const QString &token,
			       const QString &end_token, QNetworkReply *reply)
{
  QString data;
  int pos = 0, pos1 = 0;
  while(reply->size() != 0)
    {
      data = reply->readLine();
      if( (pos = data.indexOf(token, 0)) != -1)
	break;
    }
  pos += token.size();
  pos1 = data.indexOf(end_token, pos);
  return data.mid(pos, pos1-pos);
}

QNetworkReply* LoginWindow::get_page(const QString &adres)
{
  QEventLoop wait;
  QNetworkRequest req(adres);
  connect(mgr, &QNetworkAccessManager::finished, &wait, &QEventLoop::quit);
  QNetworkReply *reply = mgr->get(req);
  wait.exec();
  return reply;
}

QNetworkReply* LoginWindow::post(const QUrlQuery &postData,
				 const QString &adres)
{
  QEventLoop wait;
  QNetworkRequest req(adres);
  req.setHeader(QNetworkRequest::ContentTypeHeader,
		"application/x-www-form-urlencoded");
  connect(mgr, &QNetworkAccessManager::finished,
	  &wait, &QEventLoop::quit);
  QNetworkReply *reply
    = mgr->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
  wait.exec();
  return reply;
}

void LoginWindow::change_head_error(const QString &error)
{
  labelHead->setText(error);
  labelHead->setStyleSheet("QLabel {color: red; font-size: 20px;}");
  buttons->setEnabled(true);
  editPassword->setEnabled(true);
  editUsername->setEnabled(true);
  editPassword->setText("");
  // editUsername->setText("");
}

void LoginWindow::grey_out(const QString &text)
{
  labelHead->setText(text);
  labelHead->setStyleSheet(QString("QLabel {color: %1; font-size: 18px;}").arg(palette().color(QPalette::WindowText).name()));
  buttons->setEnabled(false);
  editPassword->setEnabled(false);
  editUsername->setEnabled(false);
  comboKierunki->setEnabled(false);
  comboSemestr->setEnabled(false);
  buttonMajor->setEnabled(false);
}
bool LoginWindow::if_logged(QNetworkReply *reply)
{
  QString data;
  while(reply->size() != 0)
    {
      data = reply->readLine();
      if (data.indexOf("Wyloguj", 0) != -1)
	return true;
    }
  return false;
}


bool LoginWindow::error(QNetworkReply *reply)
{
  QNetworkReply::NetworkError err = reply->error();
  if(err != QNetworkReply::NoError)
    return true;
  return false;
}

void LoginWindow::clear_window()
{
  editPassword->setVisible(false);
  editUsername->setVisible(false);
  buttons->setVisible(false);
  comboKierunki->setVisible(false);
  comboSemestr->setVisible(false);
  buttonMajor->setVisible(false);
  back->setVisible(false);
  download->setVisible(false);
  jezyki->setVisible(false);
  wf->setVisible(false);
  inne->setVisible(false);
  jezyki->setChecked(false);
  wf->setChecked(false);
  inne->setChecked(false);

  offset->setVisible(false);
  progressBar->setVisible(false);
  progressBarBot->setVisible(false);
  boxAKZ->setVisible(false);
}

void LoginWindow::ActiveWidget(QWidget *w)
{
  w->setVisible(true);
  w->setEnabled(true);
}

QString LoginWindow::usun_tagi(QString line)
{
  int pos1 = 0, pos2 = 0;
  while(pos1 >= 0 && pos2 >= 0)
    {
      pos1 = line.indexOf("<");
      pos2 = line.indexOf(">", pos1) + 1;
      if(pos1 >=0 && pos2 >= 1)
	line.remove(pos1, pos2-pos1);
    }
  return line;
}

QString LoginWindow::return_href(const QString &line)
{
  int pos1 = line.indexOf("href=\"") + 6;
  int pos2 = line.indexOf("\"", pos1);
  return line.mid(pos1, pos2-pos1);
}

QString LoginWindow::folderDane() const {return FolderDane;}
QString LoginWindow::folderZapisy() const {return FolderZapisy;}
QString LoginWindow::folderKryterium() const {return FolderKryterium;}
QString LoginWindow::folderPlanStudiow() const {return FolderPlanStudiow;}
QString LoginWindow::folderSemestr() const {return FolderSemestr;}
