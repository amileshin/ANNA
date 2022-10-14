QT += core gui
QT += network
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithm.cpp \
    analyzer.cpp \
    config.cpp \
    connect.cpp \
    controller.cpp \
    generator.cpp \
    main.cpp \
    maingui.cpp

HEADERS += \
    SCPI.hpp \
    algorithm.hpp \
    analyzer.hpp \
    config.hpp \
    connect.hpp \
    controller.hpp \
    dm.hpp \
    generator.hpp \
    maingui.hpp

FORMS += \
    comconnect.ui \
    editor.ui \
    ipconnect.ui \
    maingui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
