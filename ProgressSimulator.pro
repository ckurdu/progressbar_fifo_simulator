QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        ../../sunrise/progresswriterobject.cpp \
        ../../sunrise/progresswriterworker.cpp \
        ../../sunrise/factory.cpp \
        ../../sunrise/general.cpp \
        ../../sunrise/linuxnamedpipe.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

    HEADERS += \
../../sunrise/factory.h \
../../sunrise/interfacenamedpipe.h \
../../sunrise/linuxnamedpipe.h \
../../sunrise/progresswriterobject.h \
../../sunrise/progresswriterworker.h

INCLUDEPATH += ../../sunrise
