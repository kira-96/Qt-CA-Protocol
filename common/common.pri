# common.pri
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
    $$PWD\QEFrameworkLibraryGlobal.h \
    $$PWD\QEFrameworkVersion.h \
    $$PWD\QECommon.h \
    $$PWD\QEEnums.h \
    $$PWD\QEPlatform.h \
    $$PWD\QERecordFieldName.h \
    $$PWD\QEThreadSafeQueue.h \
    $$PWD\UserMessage.h

SOURCES += \
    $$PWD\QEFrameworkVersion.cpp \
    $$PWD\QECommon.cpp \
    $$PWD\QEPlatform.cpp \
    $$PWD\QERecordFieldName.cpp \
    $$PWD\UserMessage.cpp

#end
