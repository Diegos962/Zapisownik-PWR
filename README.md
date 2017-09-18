# Zapisownik

Program do układania planu dla studentów Politechniki Wrocławskiej przez serwis Edukacja.cl, oparty na bibliotece QT5.

## Kompilacja ze źródeł

### Wymagania
* Kompilator C++ z obsługą c++11 (clang++, g++ lub MinGW dla Windowsa),
* Qt 5.5+ (zalecana najnowsza wersja 5.x) wraz z qmake (do pobrania na Windowsie z http://www.qt.io/download-open-source/ lub z repozytoriów na linuksie)
* OpenSSL (do pobrania na Windowsie z http://slproweb.com/products/Win32OpenSSL.html lub z repozytoriów na linuksie)
* libpng16 (tylko na linuksie - dostępna w repozytoriach)
* QtDropbox(https://github.com/lycis/QtDropbox)

#### Pobranie paczki

Pobierz dane bezpośrednio ze strony lub poprzez klienta.

$ git clone --recursive https://github.com/Diegos962/Zapisownik.git

#### Kompilacja

Najpierw należy skompilować bibliotekę QtDropbox i plik wyjściowy (.dll lub .so) umieścić w katalogu głównym. Kompilacja biblioteki jak i programu przebiega w ten sam sposób.

W celu kompilacji wykonaj następujące dwa polecenia w głównym katalogu:

$ qmake

To polecenie utworzy plik Makefile.

$ make (lub mingw32-make na Windowsie)

Możemy również pobrać QtCreatora, wczytać projekt i nacisnąć zieloną strzałkę (release) - program nas wyręczy.

#### Uruchamianie

Na linuksie od razu możemy uruchomić program, lecz na Windowsie konieczne jest skopiowanie plików z głównego katalogu OpenSSLa do katalogu z programem (biblioteki libeay32.dll oraz ssleay32.dll). Należy się również upewnić, że w głównym folderze jest biblioteka QtDropbox i folder icons.

## Gotowe paczki
Przygotowane zostały również gotowe paczki, dla systemów Linux i Windows, które nie wymagają żadnych działań i program jest gotowy do pracy zaraz po pobraniu. Wystarczy tylko uruchomić plik 'zapisownik'.
https://github.com/Diegos962/Zapisownik/releases

Uwaga! Na linuksie konieczne może być doinstalowanie wspomnianej biblioteki 'libpng16' lub innych drobiazgów, dostępnych w repozytoriach.

## Uwagi

* W oknie logowania do serwisu polwro.pl dostępny jest "licznik" milisekund. Został on dodany, ponieważ serwis odrzuca zbyt częste połączenia i w celu uzyskania wszystkich danych, trzeba robić opóźnienia między przjeściami do kolejnych podstron. Program ma ustawione pewne opóźnienie, które dla autora wydało się optymalne, lecz w razie problemów należy je zwiększyć o jakieś 20-50 ms. Ilość prowadzących na dzień 07.09.2017 to 3005.
* Przy dodawaniu własnych kursów kod kursu powinien być zgodny z typem zajęć (Kod wykładu powinien kończyć się na 'W' itd.). Termin zajęć należy wprowadzać dokładnie w takiej postaci: pn 00:00-00:00 (inne dni to: wt, sr, cz, pt). Możliwe są również połączenia pn/TN ... Czas zajęć powinien się mieścić w przedziale [07:00; 22:00].
* Losowanie planu w pewnych przypadkach może nie wybrać kursu dla wszystkich przedmiotów.
* Zalecane jest pobieranie danych z zewnętrznego serwera, a nie z edukacji czy polwro, by owe strony nie zostały przeciążone. (Zwłaszcza polwro, ponieważ opinie o prowadzących się zbytnio nie zmieniają i wystarczy jedna aktualizacja na semestr).
* Program nie zwalnia z myślenia i wybrany plan należy potwierdzić w systemie edukacja.cl.
* Program po pobraniu danych z edukacji lub polwro wysyła je na zewnętrzny serwer. Korzystając z programu, zgadzasz się na to. Żadne dane osobiste nie są pobierane ani przechowywane, jedynie dane o kursach.

## Przykładowe użycie
* Po uruchmieniu programu wchodzimy w Plik->Baza kursów->Wczytaj->Z edukacji.
* Opcjonalnie możemy pobrać bazę prowadzących (Plik->Baza prowadzących->Wczytaj->Z Polwro).
* Teraz wybieramy kursy poprzez zaznaczenie kwadracika. Możemy ustawić filtry lub zmienić kolory na bardziej przyjazne. Jeśli nie mamy weny do układania, możemy wylosować cały plan lub tylko dolosować resztę kursów do już wybranych - zalecane użycie: po wybraniu kursów, zapisujemy je tymczasowo, dolosowywujemy i jeśli plan nam nie odpowiada, wczytujemy plan tymczasowy i ponownie dolosowywujemy.
* Zapisujemy wybrany plan: Plik->Gotowe plany->Zapisz plan. Tak zapisany plan możemy wczytać do programu, jak i zawiera on kody wybranych grup, które możemy wykorzystać do procesu zapisywania.
