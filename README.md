# Zapisownik

Program do układania planu dla studentów Politechniki Wrocławskiej przez serwis Edukacja.cl, oparty na bibliotece QT5, dzięki czemu jest wieloplatformowy.

## Kompilacja ze źródeł

### Wymagania
* Kompilator C++ z obsługą c++11 (clang++, g++ lub MinGW dla Windowsa),
* Qt 5.5+ (zalecana najnowsza wersja 5.x) wraz z qmake (do pobrania na Windowsie z http://www.qt.io/download-open-source/ lub z repozytoriów na linuksie)
* OpenSSL (do pobrania na Windowsie z http://slproweb.com/products/Win32OpenSSL.html lub z repozytoriów na linuksie)

#### Pobranie paczki

Pobierz dane bezpośrednio ze strony lub poprzez klienta.

$ git clone --recursive https://github.com/Diegos962/Zapisownik.git

#### Kompilacja

W celu kompilacji programu wykonaj następujące dwa polecenia w głównym katalogu:

$ qmake

To polecenie utworzy plik Makefile.

$ make (lub mingw32-make na Windowsie)

Możemy również pobrać QtCreatora, wczytać projekt i program nas wyręczy.

#### Uruchamianie

Na linuksie od razu możemy uruchomić program, lecz na Windowsie konieczne jest skopiowanie plików z głównego katalogu OpenSSLa do katalogu z programem (biblioteki libeay32.dll oraz ssleay32.dll).

## Gotowe paczki
Przygotowane zostały również gotowe paczki, dla systemów Linux i Windows, które nie wymagają żadnych działań i program jest gotowy do pracy zaraz po pobraniu. Wystarczy tylko uruchomić plik 'zapisownik'.
https://github.com/Diegos962/Zapisownik/releases

## Uwagi

* Program nie pilnuje potoków i użytkownik powinien sam o to zadbać, by się one zgadzały.
* W oknie logowania do serwisu polwro.pl dostępny jest "licznik" milisekund. Został on dodany, ponieważ serwis odrzuca zbyt częste połączenia i w celu uzyskania wszystkich danych, trzeba robić opóźnienia między przjeściami do kolejnych podstron. Program ma ustawione pewne opóźnienie, które dla autora wydało się optymalne, lecz w razie problemów należy je zwiększyć o jakieś 20-50 ms. Ilość prowadzących na dzień 07.09.2017 to 3005.
* Przy dodawaniu własnych kursów kod kursu powinien być zgodny z typem zajęć (Kod wykładu powinien kończyć się na 'W' itd.). Termin zajęć należy wprowadzać dokładnie w takiej postaci: pn 00:00-00:00 (inne dni to: wt, sr, cz, pt). Możliwe są również połączenia pn/TN ... Czas zajęć powinien się mieścić w przedziale [07:30; 22:00].
* Losowanie planu w pewnych przypadkach może nie wybrać kursu dla wszystkich przedmiotów.
