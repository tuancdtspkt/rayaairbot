DESTDIR = build
CONFIG += qt \
    gui

# event driven device enumeration on windows requires the gui module
OBJECTS_DIR = tmp
MOC_DIR = tmp
DEPENDDIR = .
INCLUDEDIR = .
HEADERS = qextserialport.h \
    qextserialport_global.h \
    quadrotorjoystick.h \
    data_plot.h \
    data_plot2.h
SOURCES = qextserialport.cpp \
    quadrotorjoystick.cpp \
    data_plot.cpp \
    data_plot2.cpp
FORMS += mainwindow.ui
LIBS += -lqwt-qt4
unix:SOURCES += posix_qextserialport.cpp
macx:LIBS += -framework \
    IOKit \
    -framework \
    CoreFoundation
win32 { 
    SOURCES += win_qextserialport.cpp
    DEFINES += WINVER=0x0501 # needed for mingw to pull in appropriate dbt business...probably a better way to do this
    LIBS += -lsetupapi
}
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
