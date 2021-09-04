QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    disassembler.cpp \
    elfreader.cpp \
    emulator.cpp \
    filereader.cpp \
    main.cpp \
    mainwindow.cpp \
    memorymapmodel.cpp \
    registermapmodel.cpp

HEADERS += \
    disassembler.h \
    elfheader.h \
    elfreader.h \
    emulator.h \
    filereader.h \
    instruction.h \
    mainwindow.h \
    memorymapmodel.h \
    registermapmodel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    application.qrc
