QT   += core gui sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = requeteSql
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS += widget.h

FORMS   += widget.ui
