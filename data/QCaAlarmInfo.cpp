/*  QCaAlarmInfo.cpp
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
 *  Original author: Andrew Rhyder
 *  Maintained by:   Andrew Starritt
 *
 *  Contact details: andrews@ansto.gov.au
 *  800 Blackburn Road, Clayton, Victoria 3168, Australia.
 */

// Alarm info manager

#include "QCaAlarmInfo.h"
#include <QDebug>
#include <alarm.h>
#include <acai_client_types.h>

#define DEBUG  qDebug () << "QCaAlarmInfo" << __LINE__ << __FUNCTION__ << "  "

// Quazi OOS severity (set to one more than 3, i.e. one more than INVALID)
//
// static const QCaAlarmInfo::Severity OOS_ALARM = 4;

//------------------------------------------------------------------------------
// Default standard color names.
// These string lists are index by alarm severity and/or out of service.
//
// const QStringList defaultStyleColorNames = QStringList ()
//       << "#e0eee0"                // pale green
//       << "#ffff00"                // yellow
//       << "#ff8080"                // pale red
//       << "#ffffff"                // white
//       << "#80c0ff";               // pale blue

// const QStringList defaultColorNames = QStringList ()
//       << "#00ff00"                // green
//       << "#ffff00"                // yellow
//       << "#ff0000"                // red
//       << "#ffffff"                // white
//       << "#0080ff";               // blue

//------------------------------------------------------------------------------
// Construct an empty instance.
// By default there is no alarm present.
//
QCaAlarmInfo::QCaAlarmInfo ()
{
   this->protocol = QEPvNameUri::undefined;
   this->pvName = "";
   this->status = NO_ALARM;
   this->severity = NO_ALARM;
   this->message = "";
}

//------------------------------------------------------------------------------
// Copy construct.
//
QCaAlarmInfo::QCaAlarmInfo( const QCaAlarmInfo& other )
{
   this->protocol = other.protocol;
   this->pvName = other.pvName;
   this->status = other.status;
   this->severity = other.severity;
   this->message = other.message;
}


//------------------------------------------------------------------------------
// Construct an instance given an alarm state and severity
//
QCaAlarmInfo::QCaAlarmInfo (const Status statusIn,
                            const Severity severityIn)
{
   this->protocol = QEPvNameUri::undefined;
   this->pvName = "";
   this->status = statusIn;
   this->severity = severityIn;
   this->message = "";
}

//------------------------------------------------------------------------------
// Construct an instance given an alarm state and severity together with
// pvName and mesage (PVA only).
//
QCaAlarmInfo::QCaAlarmInfo (const QEPvNameUri::Protocol protocolIn,
                            const QString& pvNameIn,
                            const Status statusIn,
                            const Severity severityIn,
                            const QString & messageIn)
{
   this->protocol = protocolIn;
   this->pvName = pvNameIn;
   this->status = statusIn;
   this->severity = severityIn;
   this->message = messageIn;
}

//------------------------------------------------------------------------------
// Deconstruct - place holder
//
QCaAlarmInfo::~QCaAlarmInfo () { }

//------------------------------------------------------------------------------
// Assignment function.
QCaAlarmInfo& QCaAlarmInfo::operator=(const QCaAlarmInfo& other)
{
   this->protocol = other.protocol;
   this->pvName = other.pvName;
   this->status = other.status;
   this->severity = other.severity;
   this->message = other.message;
   return *this;
}

//------------------------------------------------------------------------------
// Equality function.
// We only check severity and status, we exclude message and name.
//
bool QCaAlarmInfo::operator== (const QCaAlarmInfo& other) const
{
   return ((this->status == other.status) && (this->severity == other.severity));
}

//------------------------------------------------------------------------------
// InEquality function - defined in terms of == to ensure consistancy.
//
bool QCaAlarmInfo::operator!= (const QCaAlarmInfo& other) const
{
   return !(*this == other);
}

//------------------------------------------------------------------------------
// Return a string identifying the alarm state
//
QString QCaAlarmInfo::statusName () const
{
   // TODO - PVA status strings are different.
   //
   ACAI::ClientAlarmCondition condition = ACAI::ClientAlarmCondition (this->status);
   return QString::fromStdString (ACAI::alarmStatusImage (condition));
}

//------------------------------------------------------------------------------
// Return a string identifying the alarm severity
//
QString QCaAlarmInfo::severityName () const
{
   QString result;

   if ((this->severity & 0x0f00) == 0x0f00) {
      // Do CA archiver severity specials.
      //
      // QEArchiveInterface::archiveAlarmSeverity sevr =
      //       QEArchiveInterface::archiveAlarmSeverity (this->severity);
      // result = QEArchiveInterface::alarmSeverityName (sevr);
   } else {
      ACAI::ClientAlarmSeverity sevr = ACAI::ClientAlarmSeverity (this->severity);
      result = QString::fromStdString (ACAI::alarmSeverityImage (sevr));
   }

   return result;
}

//------------------------------------------------------------------------------
// Return alarm message - empty string for CA
//
QString QCaAlarmInfo::messageText () const
{
   return this->message;
}

//------------------------------------------------------------------------------
// Return true if there is an alarm
//
bool QCaAlarmInfo::isInAlarm () const
{
   return (this->severity != NO_ALARM);
}

//------------------------------------------------------------------------------
// Return true if there is a minor alarm
//
bool QCaAlarmInfo::isMinor () const
{
   return (this->severity == MINOR_ALARM);
}

//------------------------------------------------------------------------------
// Return true if there is a major alarm
//
bool QCaAlarmInfo::isMajor () const
{
   return (this->severity == MAJOR_ALARM);
}

//------------------------------------------------------------------------------
// Return true if there is an invalid alarm
//
bool QCaAlarmInfo::isInvalid () const
{
   return (this->severity == INVALID_ALARM);
}

//------------------------------------------------------------------------------
// Return a severity that will not match any valid severity (static)
// Not to be confused with the invalid state.
//
QCaAlarmInfo::Severity QCaAlarmInfo::getInvalidSeverity ()
{
   return Severity (ACAI::CLIENT_ALARM_NSEV);
}

//------------------------------------------------------------------------------
// Return the severity
//
QCaAlarmInfo::Severity QCaAlarmInfo::getSeverity () const
{
   return this->severity;
}

//------------------------------------------------------------------------------
// Return the status
//
QCaAlarmInfo::Status QCaAlarmInfo::getStatus () const
{
   return this->status;
}

//------------------------------------------------------------------------------
// Return the record name
//
QString QCaAlarmInfo::getPvName() const
{
    return this->pvName;
}

//------------------------------------------------------------------------------
// Return the protocol
//
QEPvNameUri::Protocol QCaAlarmInfo::getProtocol() const
{
    return this->protocol;
}

// end
