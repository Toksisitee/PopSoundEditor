QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common.cpp \
    Drums.cpp \
    Music.cpp \
    Sound.cpp \
    main.cpp \
    mainwindow.cpp \
    soundeditor.cpp

HEADERS += \
    Common.h \
    Drums.h \
    Editor.h \
    Music.h \
    Sound.h \
    Version.h \
    mainwindow.h \
    soundeditor.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = icon.ico
win32:RC_FILE = resources.rc

DISTFILES += \
    LICENSE \
    LICENSE-QT \
    PopSoundEditor.PNG \
    icon.png \
    resources.rc
