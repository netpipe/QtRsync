QT       += core gui multimedia widgets

OBJECTS_DIR = ./build/obj
MOC_DIR = ./build/moc

TEMPLATE = app

SOURCES += src/main.cpp \
 src/mainwindow.cpp \
 src/rsyncwrapper.cpp \
 src/rsyncwidget.cpp

CONFIG += warn_on \
	  thread \
          qt
#TARGET = ./bin/qtrsync

HEADERS += src/mainwindow.h \
src/rsyncwrapper.h \
 src/rsyncwidget.h

desktop.path = /usr/share/applications/
desktop.files = ./other/*.desktop

target.path = /usr/bin

CONFIG += warn_on \
          qt \
          thread \
	release

RESOURCES += icons.qrc
#INSTALLS = target desktop

