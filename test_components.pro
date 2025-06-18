QT += core testlib sql

CONFIG += c++17

# Test specific configurations
CONFIG += testcase
CONFIG -= app_bundle
CONFIG -= uic

# Include paths
INCLUDEPATH += .
INCLUDEPATH += ..

# Define a macro to skip UI-related code
DEFINES += TEST_MODE

# Create a separate build directory
DESTDIR = $$PWD/build

SOURCES += \
    test_components.cpp

HEADERS += \
    database.h \
    HandlePB.h \
    History_LinkedList.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 