/*  QCaAlarmInfo.h
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2009-2024 Australian Synchrotron
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

// Manage CA (and PVA) alarm and severity information

#ifndef QE_ALARM_INFO_H
#define QE_ALARM_INFO_H

#include <QEPvNameUri.h>
#include <QEFrameworkLibraryGlobal.h>

class QE_FRAMEWORK_LIBRARY_SHARED_EXPORT QCaAlarmInfo
{
public:
   typedef unsigned short Status;    // Alarm onfo status type.
   typedef unsigned short Severity;  // Alarm onfo severity type.

   // For backward compatibility.
#  define QCAALARMINFO_SEVERITY  QCaAlarmInfo::Severity

   explicit QCaAlarmInfo();
   QCaAlarmInfo( const QCaAlarmInfo& other );

   // General status and severity
   QCaAlarmInfo( const Status status,
                 const Severity severity );

   // PV update status and severity
   QCaAlarmInfo( const QEPvNameUri::Protocol protocol,
                 const QString& pvName,
                 const Status status,
                 const Severity severity,
                 const QString& message );

   virtual ~QCaAlarmInfo();

   QCaAlarmInfo& operator=(const QCaAlarmInfo& other);
   bool operator==(const QCaAlarmInfo& other) const;   // Return true if equal
   bool operator!=(const QCaAlarmInfo& other) const;   // Return true if not equal

   QString statusName() const;       // Return the name of the current alarm state
   QString severityName() const;     // Return the name of the current alarm severity
   QString messageText() const;      // Return alarm message - empty string for CA
   bool isInAlarm() const;           // Return true if there is an alarm
   bool isMinor() const;             // Return true if there is a minor alarm
   bool isMajor() const;             // Return true if there is a major alarm
   bool isInvalid() const;           // Return true if there is an invalid alarm
   // bool isOutOfService() const;      // Return true if the associated PV declared OOS.

   static Severity getInvalidSeverity();  // Return a severity that will not match any valid severity
   Severity getSeverity() const;      // Return the current severity
   Status   getStatus() const;        // Return the current status
   QString  getPvName() const;        // Return the record name
   QEPvNameUri::Protocol getProtocol() const;  // Return the protocol

private:
   QEPvNameUri::Protocol protocol;      // protocol - if known
   QString  pvName;      // pv/record name
   Status   status;      // Alarm state
   Severity severity;    // Alarm severity
   QString  message;     // Alarm message (PV Access only - otherwise empty string)
};

#endif // QE_ALARM_INFO_H
