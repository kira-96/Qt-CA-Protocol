QT += core

TEMPLATE = lib
DEFINES += QE_FRAMEWORK_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 修改目标架构和目录
EPICS_HOST_ARCH = linux-x86_64
EPICS_BASE = /home/loongson/base-7.0.8
ACAI = /home/loongson/acai

# Support PV Access
QE_PVACCESS_SUPPORT = YES

# Check EPICS dependancies
#
_EPICS_BASE = $${EPICS_BASE}
isEmpty( _EPICS_BASE ) {
    error( "EPICS_BASE must be defined. Ensure EPICS is installed and EPICS_BASE environment variable is defined." )
}

_EPICS_HOST_ARCH = $${EPICS_HOST_ARCH}
isEmpty( _EPICS_HOST_ARCH ) {
    error( "EPICS_HOST_ARCH must be defined. Ensure EPICS is installed and EPICS_HOST_ARCH environment variable is defined." )
}

_ACAI = $${ACAI}
isEmpty( _ACAI ) {
    message ( "ACAI must be defined. Ensure ACAI is installed and the ACAI environment variable is defined," )
    error ( "  It is available from: https://github.com/andrewstarritt/acai.git" )
}

# Define _MINGW if using a MinGW compiler
#
equals( _EPICS_HOST_ARCH, "win32-x86-mingw" ) {
    message("MINGW compiler in use, defining _MINGW=TRUE")
    DEFINES += _MINGW=TRUE
}
equals( _EPICS_HOST_ARCH, "windows-x64-mingw" ) {
    message("MINGW compiler in use, defining _MINGW=TRUE")
    DEFINES += _MINGW=TRUE
}

include(protocol/protocol.pri)
include(data/data.pri)

HEADERS += \
    QEFrameworkLibraryGlobal.h \
    QECommon.h \
    QEEnums.h \
    QEPlatform.h \
    QERecordFieldName.h \
    QEThreadSafeQueue.h \
    UserMessage.h

SOURCES += \
    QECommon.cpp \
    QEPlatform.cpp \
    QERecordFieldName.cpp \
    UserMessage.cpp

OTHER_FILES += \
    LICENSE \
    README.md

INCLUDEPATH += \
    $${EPICS_BASE}/include \
    $${EPICS_BASE}/include/compiler/gcc \
    $${EPICS_BASE}/include/pv

win32 {
INCLUDEPATH += $${EPICS_BASE}/include/os/WIN32
}
linux {
INCLUDEPATH += $${EPICS_BASE}/include/os/Linux
}

# Set runtime path for shared libraries
#
unix: QMAKE_LFLAGS += -Wl,-rpath,$$(EPICS_BASE)/lib/$$(EPICS_HOST_ARCH)

LIBS += -L$${EPICS_BASE}/lib/$${EPICS_HOST_ARCH} -lca -lCom

#===========================================================
# Set up ACAI
#
INCLUDEPATH += $${ACAI}/include
LIBS += -L$${ACAI}/lib/$${EPICS_HOST_ARCH} -lacai

#===========================================================
# Set up PV Access
# For headless build QE_PVACCESS_SUPPORT is set in qeframeworkSup Makefile if using EPICS 7 or later.
#
_PVACCESS_SUPPORT = $${QE_PVACCESS_SUPPORT}
equals(_PVACCESS_SUPPORT, "YES") {
    # This currently assumes EPICS 7. Maybe we could allow EPICS 4 builds as well.
    #
    message( "QE_PVACCESS_SUPPORT is defined. The QE framework library will be built for both CA and PVA. ")

    # Let the code 'know' to include PV Access related stuff.
    #
    DEFINES += QE_PVACCESS_SUPPORT

    # If you are using EPICS 4, modify the following to reference  EPICS 4 libraries.
    #
    LIBS += -L$${EPICS_BASE}/lib/$${EPICS_HOST_ARCH} -lpvData  -lpvAccess -lnt
} else {
    message( "QE_PVACCESS_SUPPORT is not defined. The QE framework library will not include PV Access support." )
    message( "If you want to build with PV Access support, set environment variable QE_PVACCESS_SUPPORT=YES" )
}

TARGET = QEProtocol

# Place all intermediate generated files in architecture specific locations
#
MOC_DIR        = O.$${EPICS_HOST_ARCH}/moc
OBJECTS_DIR    = O.$${EPICS_HOST_ARCH}/obj
UI_DIR         = O.$${EPICS_HOST_ARCH}/ui
RCC_DIR        = O.$${EPICS_HOST_ARCH}/rcc
MAKEFILE       = Makefile.$${EPICS_HOST_ARCH}

VERSION = 4.0.1
CONFIG += skip_target_version_ext
RC_LANG = 0x0004
QMAKE_TARGET_COMPANY = "SINAP"
QMAKE_TARGET_DESCRIPTION = "Qt EPICS Protocol Client Lib"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2024 SINAP. All rights reserved."
QMAKE_TARGET_PRODUCT = "EPICS Protocol Lib"

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
