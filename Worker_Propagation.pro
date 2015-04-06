#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T19:34:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Worker_Propagation
TEMPLATE = app


SOURCES += main.cpp\
        fenetre.cpp \
    Networking.cpp \
    calcul.cpp \
    travail.cpp

HEADERS  += fenetre.h \
    Networking.h \
    calcul.h \
    travail.h

FORMS    += fenetre.ui
