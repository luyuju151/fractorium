TEMPLATE = app
CONFIG += console
CONFIG += warn_off
CONFIG += precompile_header
CONFIG -= app_bundle
CONFIG -= qt
VERSION = 0.1.4.7

DESTDIR = $$(HOME)/Dev/fractorium/Bin

LIBS += -L/usr/lib -lOpenCL
LIBS += -L/usr/lib -lGL
LIBS += -L/usr/lib -ljpeg
LIBS += -L/usr/lib -lpng
LIBS += -L/usr/lib/x86_64-linux-gnu -lxml2
LIBS += -L$$DESTDIR -lEmber
LIBS += -L$$DESTDIR -lEmberCL

INCLUDEPATH += /usr/include/CL
INCLUDEPATH += /usr/include/GL
INCLUDEPATH += /usr/include/glm
INCLUDEPATH += /usr/include/tbb
INCLUDEPATH += /usr/include/libxml2
INCLUDEPATH += ../../../Source/Ember
INCLUDEPATH += ../../../Source/EmberCL
INCLUDEPATH += ../../../Source/EmberCommon

QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

QMAKE_CXXFLAGS += -march=k8
QMAKE_CXXFLAGS += -fPIC
QMAKE_CXXFLAGS += -fpermissive
QMAKE_CXXFLAGS += -fomit-frame-pointer
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wnon-virtual-dtor
QMAKE_CXXFLAGS += -Wshadow
QMAKE_CXXFLAGS += -Winit-self
QMAKE_CXXFLAGS += -Wredundant-decls
QMAKE_CXXFLAGS += -Wcast-align
QMAKE_CXXFLAGS += -Winline
QMAKE_CXXFLAGS += -Wunreachable-code
QMAKE_CXXFLAGS += -Wmissing-include-dirs
QMAKE_CXXFLAGS += -Wswitch-enum
QMAKE_CXXFLAGS += -Wswitch-default
QMAKE_CXXFLAGS += -Wmain
QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant
QMAKE_CXXFLAGS += -Wfatal-errors
QMAKE_CXXFLAGS += -Wall -fpermissive
QMAKE_CXXFLAGS += -Wold-style-cast
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-function
QMAKE_CXXFLAGS += -Wold-style-cast
QMAKE_CXXFLAGS += -D_M_X64
QMAKE_CXXFLAGS += -D_USRDLL 
QMAKE_CXXFLAGS += -DNDEBUG
QMAKE_CXXFLAGS += -D_CONSOLE

QMAKE_LFLAGS_RELEASE += -s

PRECOMPILED_HEADER = ../../../Source/EmberCommon/EmberCommonPch.h

SOURCES += \
    ../../../Source/EmberGenome/EmberGenome.cpp \
    ../../../Source/EmberCommon/EmberCommonPch.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../../../Source/EmberGenome/EmberGenome.h \
    ../../../Source/EmberCommon/EmberCommon.h \
    ../../../Source/EmberCommon/EmberCommonPch.h \
    ../../../Source/EmberCommon/EmberOptions.h \
    ../../../Source/EmberCommon/JpegUtils.h \
    ../../../Source/EmberCommon/SimpleGlob.h \
    ../../../Source/EmberCommon/SimpleOpt.h
