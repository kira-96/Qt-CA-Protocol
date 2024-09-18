QT += core gui

TEMPLATE = lib
DEFINES += QE_FRAMEWORK_LIBRARY
DEFINES += QE_PVACCESS_SUPPORT

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 修改目标架构和目录
EPICS_HOST_ARCH = linux-x86_64
EPICS_BASE = /home/loongson/base-7.0.8
ACAI = /home/loongson/acai

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

INCLUDEPATH += $${ACAI}/include
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

LIBS += -L$$ACAI/lib/$$EPICS_HOST_ARCH -lacai
LIBS += -L$$EPICS_BASE/lib/$$EPICS_HOST_ARCH -lca -lCom -lnt -lpvAccess -lpvAccessCA -lpvData -lpvaClient

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
