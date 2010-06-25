DESTDIR = build
CONFIG += qt \
    gui
QT += network

# event driven device enumeration on windows requires the gui module
OBJECTS_DIR = tmp
MOC_DIR = tmp
DEPENDDIR = .
INCLUDEDIR = .
HEADERS = quadrotorjoystick.h \
    data_plot.h \
    data_plot2.h \
    tcp.h
SOURCES = quadrotorjoystick.cpp \
    data_plot.cpp \
    data_plot2.cpp \
    tcp.cpp
FORMS += mainwindow.ui
LIBS += -lqwt-qt4
macx:LIBS += -framework \
    IOKit \
    -framework \
    CoreFoundation

HEADERS += qjoystick.h
SOURCES += qjoystick.cpp \
    main.cpp
unix:!macx:LIBS += -lSDL \
    -L/usr/lib
macx { 
    LIBS += -framework \
        SDL \
        -framework \
        Cocoa \
        -lode
    SOURCES += src/SDLMain.m
    HEADERS += src/SDLMain.h
}
