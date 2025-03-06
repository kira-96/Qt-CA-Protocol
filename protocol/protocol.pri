# protocol.pri
#
# The EPICS QT Framework is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The EPICS QT Framework is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
#
# Author:
#   Yang Zhenghan
# Contact details:
#   yangzhenghan@sinap.ac.cn
#

INCLUDEPATH += $$PWD

HEADERS += \
        $$PWD/QEPvaCheck.h \
        $$PWD/QEBaseClient.h \
        $$PWD/QECaClient.h \
        $$PWD/QENullClient.h \
        $$PWD/QEPvNameUri.h \
        $$PWD/QEVectorVariants.h

SOURCES += \
        $$PWD/QEBaseClient.cpp \
        $$PWD/QECaClient.cpp \
        $$PWD/QENullClient.cpp \
        $$PWD/QEPvNameUri.cpp \
        $$PWD/QEVectorVariants.cpp

contains(DEFINES, QE_PVACCESS_SUPPORT) {
HEADERS += \
        $$PWD/QENTNDArrayData.h \
        $$PWD/QENTTableData.h \
        $$PWD/QEOpaqueData.h \
        $$PWD/QEPvaData.h \
        $$PWD/QEPvaClient.h

SOURCES += \
        $$PWD/QENTNDArrayData.cpp \
        $$PWD/QENTTableData.cpp \
        $$PWD/QEOpaqueData.cpp \
        $$PWD/QEPvaData.cpp \
        $$PWD/QEPvaClient.cpp
}

# end
