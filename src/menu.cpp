#include "../include/main_window.h"

void Window::ustawMenu()
{
  menu = new QMenuBar(this);
  QAction *LosujPlan = new QAction("Losuj plan", this);
  connect(LosujPlan, &QAction::triggered, this, &Window::losujPlan);
  
  QAction *oProgramie = new QAction("O programie", this);
  connect(oProgramie, &QAction::triggered, this, [=]
	  {
	    QMessageBox msg;
	    QString text = "<p style=\"text-align: center;\"><span style=\"font-size:20px;\"><strong>ZAPISOWNIK PWR 0.9</strong></span></p> <p>Program do szybkiego układania planu dla studentów Politechniki Wrocławskiej, współpracujący z serwisem <em>edukacja.cl.</em></p> <p style=\"text-align: center;\">Autor projektu: Dawid Zaraza</p> <p style=\"text-align: center;\">Strona projektu: <a href=\"https://github.com/Diegos962/Zapisownik-PWR\">GITHUB</a></p> <br> W programie mogą występować błędy i nie zwalnia on użytkownika z myślenia. Przed zapisami sprawdź, czy ułożony przez ciebie plan faktycznie istnieje w serwisie edukacja.cl. Wszelkie błędy i uwagi proszę zgłaszać na adres: diegozaraza@wp.pl<p style=\"text-align: center;\"><span style=\"font-size:14px;\"><strong>Licencja</strong></span></p> Niniejszy program jest wolnym oprogramowaniem; można go rozprowadzać dalej i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU, wydanej przez Fundację Wolnego Oprogramowania ‒ według wersji drugiej tej Licencji lub którejś z późniejszych wersji. Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. W celu uzyskania bliższych informacji należy zapoznać się z Powszechną Licencją Publiczną GNU. <br><p style=\"text-align: center;\">Copyright &copy; 2017 Dawid Zaraza</p>";
	    msg.about(this, "O programie", text);
	  });
  
  menu->addMenu(ustawMenuPlik());
  menu->addMenu(ustawMenuEdycja());
  menu->addMenu(ustawMenuWidok());
  menu->addAction(LosujPlan);
  menu->addAction(oProgramie);
}

QMenu *Window::ustawMenuPlik()
{

  QMenu *menuPlik = new QMenu("Plik", this);
  QMenu *bazaKursow = new QMenu("Baza kursów...", this);
  QAction *zapiszKursy = new QAction("Zapisz do pliku", this);
  QMenu *wczytajKursy = new QMenu("Wczytaj...", this);
  QAction *KursyZPliku = new QAction("Z pliku", this);
  QAction *KursyZEdukacji = new QAction("Z edukacji", this);
  QAction *KursyZAKZ = new QAction("Z AKZ", this);
  QMenu *bazaProawdzacych = new QMenu("Baza prowadzących...", this);
  QAction *zapiszProwadzacych = new QAction("Zapisz do pliku", this);
  QMenu *wczytajProwadzacych = new QMenu("Wczytaj...", this);
  QAction *ProwadzacyZPliku = new QAction("Z pliku", this);
  QAction *ProwadzacyZPolwro = new QAction("Z polwro", this);
  QMenu *ulozonePlany = new QMenu("Gotowe plany...", this);
  QAction *zapiszWybranyPlan = new QAction("Zapisz plan", this);
  QAction *wczytajWybranyPlan = new QAction("Wczytaj własny plan", this);
  QAction *wczytajWybranyPlanKolegi = new QAction("Wczytaj plan kolegi", this);
  
  bazaKursow->addMenu(wczytajKursy);
  bazaKursow->addAction(zapiszKursy);
  wczytajKursy->addAction(KursyZPliku);
  wczytajKursy->addAction(KursyZEdukacji);
  wczytajKursy->addAction(KursyZAKZ);
  bazaProawdzacych->addMenu(wczytajProwadzacych);
  bazaProawdzacych->addAction(zapiszProwadzacych);
  wczytajProwadzacych->addAction(ProwadzacyZPliku);
  wczytajProwadzacych->addAction(ProwadzacyZPolwro);
  ulozonePlany->addAction(zapiszWybranyPlan);
  ulozonePlany->addAction(wczytajWybranyPlan);
  ulozonePlany->addAction(wczytajWybranyPlanKolegi);

  connect(zapiszKursy, &QAction::triggered, this, &Window::ZapiszKursy);
  connect(zapiszProwadzacych, &QAction::triggered, this, &Window::ZapiszProwadzacych);
  connect(KursyZPliku, &QAction::triggered, this, &Window::wczytajKursyZPliku);
  connect(KursyZEdukacji, &QAction::triggered, this, &Window::wczytajKursyZEdukacji);
  connect(KursyZAKZ, &QAction::triggered, this, &Window::wczytajKursyZAKZ);
  connect(ProwadzacyZPliku, &QAction::triggered, this, &Window::wczytajProwadzacychZPliku);
  connect(ProwadzacyZPolwro, &QAction::triggered, this, &Window::wczytajProwadzacychZPolwro);
  connect(wczytajWybranyPlan, &QAction::triggered, this, &Window::wczytajPlan);
  connect(wczytajWybranyPlanKolegi, &QAction::triggered, this, &Window::wczytajPlanKolegi);
  connect(zapiszWybranyPlan, &QAction::triggered, this, &Window::zapiszPlan);

  menuPlik->addMenu(bazaKursow);
  menuPlik->addMenu(bazaProawdzacych);
  menuPlik->addMenu(ulozonePlany);

  return menuPlik;
}

QMenu *Window::ustawMenuEdycja()
{
  QMenu *menuEdycja = new QMenu("Edycja", this);
  
  QAction *edytujKursy = new QAction("Edytuj bazę kursów", this);
  QAction *edytujProwadzacych = new QAction("Edytuj prowadzących", this);
  QAction *dopasujPolwro = new QAction("Dopasuj prowadzących z polwro", this);
  QAction *OdznaczZaznaczone = new QAction("Odznacz zaznaczone", this);
  connect(OdznaczZaznaczone, &QAction::triggered, this, &Window::odznaczZaznaczone);
  connect(dopasujPolwro, &QAction::triggered, this, [=]
	  {
	    for(auto &a: przedmioty)
	      przypisz_prowadzacego(a);
	    update_tree();
	  });
  connect(edytujKursy, &QAction::triggered, this, [=]
	  {
	    EdycjaKursowDialog d(przedmioty, prow);
	    d.show();
	    int val = d.exec();
	    if(val)
	      {
		przedmioty = d.stare;
		kursy += d.nowe;
		Przedmioty();
		update_tree();
	      }
	  });
  connect(edytujProwadzacych, &QAction::triggered, this, [=]
	  {
	    EdycjaProwadzacychDialog d(przedmioty, prow);
	    d.show();
	    int val = d.exec();
	    if(val)
	      {
		przedmioty = d.kursy;
		update_tree();
	      }
	  });

  menuEdycja->addAction(edytujKursy);
  menuEdycja->addAction(edytujProwadzacych);
  menuEdycja->addAction(dopasujPolwro);
  menuEdycja->addAction(OdznaczZaznaczone);

  return menuEdycja;
}

QMenu *Window::ustawMenuWidok()
{
  QMenu *menuWidok = new QMenu("Widok", this);
  QAction *ZmienWidok = new QAction("Przełącz widok", this);
  QAction *ustawFiltry = new QAction("Filtrowanie...", this);
  QAction *zmienKolory = new QAction("Zmień kolory", this);
  QMenu *pokazInfo = new QMenu("Pokaż...", this);
  QAction *pokazKodGrupy = new QAction("Kod grupy", this);
  QAction *pokazProwadzacego = new QAction("Prowadzącego", this);
  QAction *pokazOceneProwadzacego = new QAction("Ocenę prowadzącego", this);
  QAction *pokazOPinieProwadzacego = new QAction("Ilość opinii prowadzącego", this);
  QAction *pokazTermin = new QAction("Termin", this);
  QAction *pokazMiejsca = new QAction("Ilość wolnych miejsc", this);
  QAction *pokazPotok = new QAction("Potok", this);
  pokazKodGrupy->setCheckable(true);
  pokazProwadzacego->setCheckable(true);
  pokazOceneProwadzacego->setCheckable(true);
  pokazOPinieProwadzacego->setCheckable(true);
  pokazTermin->setCheckable(true);
  pokazMiejsca->setCheckable(true);
  pokazPotok->setCheckable(true);
  pokazKodGrupy->setChecked(true);
  pokazProwadzacego->setChecked(true);
  pokazOceneProwadzacego->setChecked(true);
  pokazOPinieProwadzacego->setChecked(true);
  pokazTermin->setChecked(true);
  pokazMiejsca->setChecked(true);
  pokazPotok->setChecked(true);
  QObject::connect(ustawFiltry, &QAction::triggered, this , [=]
		   {
		     filtrowanie->show();
		     filtrowanie->exec();
		     filtry();
		   });
  QObject::connect(zmienKolory, &QAction::triggered, this , &Window::ZmienKolory);
  QObject::connect(pokazKodGrupy, &QAction::triggered, this, [=] {tree->setColumnHidden(1, !pokazKodGrupy->isChecked());});
  QObject::connect(pokazProwadzacego, &QAction::triggered, this, [=] {tree->setColumnHidden(2, !pokazProwadzacego->isChecked());});
  QObject::connect(pokazOceneProwadzacego, &QAction::triggered, this, [=] {tree->setColumnHidden(3, !pokazOceneProwadzacego->isChecked());});
  QObject::connect(pokazOPinieProwadzacego, &QAction::triggered, this, [=] {tree->setColumnHidden(4, !pokazOPinieProwadzacego->isChecked());});
  QObject::connect(pokazTermin, &QAction::triggered, this, [=] {tree->setColumnHidden(5, !pokazTermin->isChecked());});
  QObject::connect(pokazMiejsca, &QAction::triggered, this, [=] {tree->setColumnHidden(6, !pokazMiejsca->isChecked());});
  QObject::connect(pokazPotok, &QAction::triggered, this, [=] {tree->setColumnHidden(7, !pokazPotok->isChecked());});
  QObject::connect(ZmienWidok, &QAction::triggered, this, &Window::PrzelaczWidok);

  menuWidok->addAction(ustawFiltry);
  menuWidok->addAction(zmienKolory);
  menuWidok->addMenu(pokazInfo);
  pokazInfo->addAction(pokazKodGrupy);
  pokazInfo->addAction(pokazProwadzacego);
  pokazInfo->addAction(pokazOceneProwadzacego);
  pokazInfo->addAction(pokazOPinieProwadzacego);
  pokazInfo->addAction(pokazTermin);
  pokazInfo->addAction(pokazMiejsca);
  pokazInfo->addAction(pokazPotok);
  menuWidok->addAction(ZmienWidok);

  return menuWidok;
}
