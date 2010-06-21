TEMPLATE = app

# Input
SOURCES += main.cpp \
    rayaairbot.cpp \
    qextserialport.cpp \
    posix_qextserialport.cpp \
    serial.cpp \
    tcp.cpp
QT -= gui
QT += network
HEADERS += rayaairbot.h \
    qextserialport_global.h \
    qextserialport.h \
    serial.h \
    tcp.h
