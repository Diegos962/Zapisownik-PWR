QT += core
  QT += gui
  QT += widgets
  QT += network
  QT += concurrent
  QT += xml
  
  TARGET = zapisownik
  CONFIG -= app_bundle
  TEMPLATE = app
  
  SOURCES = src/main.cpp \
            src/login_window.cpp \
            src/kursy_grupy_bloki.cpp \
            src/color_dialog.cpp \
            src/edycja_kursow.cpp \
            src/edycja_prowadzacych.cpp \
            src/treeWidgetItem.cpp \
            src/blokWidget.cpp \
            src/poboczneTerminy.cpp \
            src/filtry.cpp \
            src/color_double_button.cpp \
            src/wybrane_kolory.cpp \
            src/poboczneDopasowywanie.cpp \
            src/Polwro.cpp \
            src/AKZ.cpp \
            src/main_window.cpp \
            src/menu.cpp \
            src/wyborKursu.cpp \
            src/zapiszWczytaj.cpp \
            src/tree.cpp \
            src/drawGridCourse.cpp \
            src/dropbox.cpp \
            src/dropboxDialog.cpp
            
  HEADERS = include/login_window.h \
            include/kursy_grupy_bloki.h \
            include/main_window.h \
            include/color_dialog.h \
            include/edycja_kursow.h \
            include/edycja_prowadzacych.h \
            include/treeWidgetItem.h \
            include/blokWidget.h \
            include/poboczneTerminy.h \
            include/filtry.h \
            include/color_double_button.h \
            include/wybrane_kolory.h \
            include/poboczneDopasowywanie.h \
            include/dropbox.h \
            include/dropboxDialog.h

  OBJECTS_DIR = obj/
  MOC_DIR = moc/
  QMAKE_LFLAGS += -Wl,-rpath,"'$$ORIGIN'"
  LIBS += -L$$PWD/ -lQtDropbox -Wl,-R$$PWD/
