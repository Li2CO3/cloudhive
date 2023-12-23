QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#RC_ICONS = gameicon.ico

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += .
INCLUDEPATH += res/dll
#CONFIG += static
#INCLUDEPATH +=  += -L/D:/Qt/6.6.0/msvc2019_64/include
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib -lQt6Guid -lQt6Guid -lQt6Pdfd -lQt6Svgd -lQt6Widgetsd
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/generic -lqtuiotouchplugind
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/iconengines -lqsvgicond
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/imageformats -lqgifd -lqicnsd -lqicod -lqjpegd
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/imageformats -lqpdfd -lqsvgd -lqtgad -lqtiffd -lqwbmpd -lqwebpd
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/networkinformation -lqnetworklistmanagerd
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/platforms -lqwindowsd
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/styles -lqwindowsvistastyled
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib/tls -lqcertonybackendd -lqopensslbackendd -lqschannelbackendd
#LIBS += -L/D:/Qt/6.6.0/msvc2019_64/lib -lD3Dcompiler_47 -lopengl32sw -lQt6Cored


SOURCES += \
    game.cpp \
    mainwindow.cpp \
    monsters/catburglar.cpp \
    monsters/ironwall.cpp \
    monsters/mimicchest.cpp \
    monsters/monster.cpp \
    monsters/puppeteer.cpp \
    monsters/snowman.cpp \
    monsters/stall.cpp \
    monsters/twinhead.cpp \
    ui.cpp

HEADERS += \
    game.h \
    mainwindow.h \
    monsters/catburglar.h \
    monsters/ironwall.h \
    monsters/mimicchest.h \
    monsters/monster.h \
    monsters/puppeteer.h \
    monsters/snowman.h \
    monsters/stall.h \
    monsters/twinhead.h \
    ui.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

RESOURCES +=
