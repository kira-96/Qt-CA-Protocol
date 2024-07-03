/*  QECommon.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2013-2023 Australian Synchrotron.
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
 *    Andrew Starritt
 *  Contact details:
 *    andrew.starritt@synchrotron.org.au
 *
 */

#include "QECommon.h"

#include <math.h>

#include <QDir>
#include <QtGlobal>
#include <QColor>
#include <QDebug>
#include <QFileInfo>
#include <QMetaEnum>
#include <QMetaObject>
#include <QPoint>
#include <QRegularExpression>
#include <QSize>
#include <QScreen>
#include <QEPlatform.h>

#define DEBUG qDebug () << "QECommon" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
QColor QEUtilities::fontColour (const QColor& backgroundColour)
{
   QColor result;
   int r, g, b, a;

   // Split colour into components.
   //
   backgroundColour.getRgb (&r, &g, &b, &a);

   // Counting the perceptive luminance - human eye favors green color...
   // Form weighted component wc. Weights based on algorithm:
   // http://stackoverflow.com/questions/1855884/determine-font-color-based-on-background-color
   //
   const int wc = ((299 * r) + (587 * g) + (114 * b)) / 1000;   // 299 + 587 + 114 = 1000

   // Dark or bright background colour ?
   //
   const bool isDark = (wc < 124);
   if (isDark) {
      result = QColor (255, 255, 255, 255);    // white font
   } else {
      result = QColor (0, 0, 0, 255);          // black font
   }

   return result;
}

//------------------------------------------------------------------------------
//
QString  QEUtilities::colourToStyle (const QColor& backgroundColour,
                                     const QColor& foregroundColour)
{
   QString result;
   int br, bg, bb, ba;
   int fr, fg, fb, fa;

   // Split colours into components and aggragate into a style sheet.
   //
   backgroundColour.getRgb (&br, &bg, &bb, &ba);
   foregroundColour.getRgb (&fr, &fg, &fb, &fa);

   result = QString::asprintf ("QWidget { background-color: rgba(%d, %d, %d, %d);"
                               " color: rgba(%d, %d, %d, %d); }",
                               br, bg, bb, ba, fr, fg, fb, fa );
   return result;
}

//------------------------------------------------------------------------------
//
QString QEUtilities::colourToStyle (const QColor& backgroundColour)
{
   // Choose high contrast font/foreground colour.
   //
   QColor foregroundColour = QEUtilities::fontColour (backgroundColour);
   return QEUtilities::colourToStyle (backgroundColour, foregroundColour);
}

//------------------------------------------------------------------------------
//
QString QEUtilities::offBackgroundStyle ()
{
   // Standard background on Linux is #d6d2d0. Add #0a0a0a
   // Windows may be a bit different.
   //
   return QEUtilities::colourToStyle (QColor (0xe0dcda));
}

//------------------------------------------------------------------------------
//
QColor QEUtilities::darkColour (const QColor& lightColour)
{
   QColor result;
   int h, s, l, a;

   lightColour.getHsl (&h, &s, &l, &a);

   // 33.3% lightness
   //
   l = (2*l) / 3;

   result.setHsl (h, s, l, a);
   return result;
}

//------------------------------------------------------------------------------
//
QColor QEUtilities::blandColour (const QColor& vibrantColour)
{
   QColor result;
   int r, g, b, a;

   vibrantColour.getRgb (&r, &g, &b, &a);

   // Move 87.5% of the way towards light gray - #c8c8c8.
   //
   r = (7*200 + r) / 8;
   g = (7*200 + g) / 8;
   b = (7*200 + b) / 8;

   result.setRgb (r, g, b, a);
   return result;
}

//------------------------------------------------------------------------------
//
int QEUtilities::roundToInt (const double x, bool* ok)
{
   const double dimin = (double) INT_MIN;
   const double dimax = (double) INT_MAX;

   int result;
   bool okay;

   if (x < dimin) {
      // Too small
      //
      result = INT_MIN;
      okay = false;

   } else if (x > dimax) {
      // Tooo big
      //
      result = INT_MAX;
      okay = false;

   } else {
      // In range - conversion possible.
      //
      if (x >= 0.0) {
         // Conversion to int truncates towards 0, must treat positive and
         // negative numbers differently.
         //
         result = (int)(x + 0.5);
      } else {
         result = (int)(x - 0.5);
      }
      okay = true;
   }

   if (ok) *ok = okay;
   return result;
}

//------------------------------------------------------------------------------
//
QString QEUtilities::intervalToString (const double interval,
                                       const int precision,
                                       const bool showDays)
{
   QString result;
   double seconds;
   QString sign;
   QString image;
   QString fraction;
   int days;
   int hours;
   int mins;
   int secs;
   int nanoSecs;

   if (interval >= 0.0) {
      seconds = +interval;
      sign = "";
   } else {
      seconds = -interval;
      sign = "-";
   }

#define EXTRACT(item, spi) { item = int (floor (seconds / spi)); seconds = seconds - (spi * item); }

   EXTRACT (days, 86400.0);
   EXTRACT (hours, 3600.0);
   EXTRACT (mins, 60.0);
   EXTRACT (secs, 1.0);
   EXTRACT (nanoSecs, 1.0E-9);

#undef EXTRACT

   if (showDays || (days != 0)) {
      image = QString::asprintf ("%d %02d:%02d:%02d", days, hours, mins, secs);
   } else {
      image = QString::asprintf ("%02d:%02d:%02d", hours, mins, secs);
   }

   if (precision > 0) {
      // Limit precision to 9.
      //
      fraction = QString::asprintf (".%09d", nanoSecs);
      fraction.truncate (MIN (9, precision) + 1);
   } else {
      fraction = "";
   }

   result = sign.append (image).append (fraction);
   return result;
}

//------------------------------------------------------------------------------
//
int QEUtilities::getTimeZoneOffset (const QDateTime& atTime)
{
   QDateTime local = atTime.toLocalTime ();

   // This changes the time, e.g. from "2013-02-24 11:37:19 EST" to "2013-02-24 11:37:19 UTC" which
   // has same numbers but is not the same time in an absolute sense (with apologies to Einstein).
   //
   local.setTimeSpec (Qt::UTC);

   // The "same" time in different time zones is a different time.
   // Use that difference to determine the local time offset from UTC.
   //
   return atTime.secsTo (local);
}

//------------------------------------------------------------------------------
//
QString QEUtilities::getTimeZoneTLA (const Qt::TimeSpec timeSpec, const QDateTime & atTime)
{
   QString result;

   switch (timeSpec) {
      case Qt::UTC:
         result = "UTC";
         break;

      case Qt::LocalTime:
#ifdef _XOPEN_SOURCE
#include <time.h>
         {
            // Get offset and convert to hours
            //
            int actualOffset;
            actualOffset = QEUtilities::getTimeZoneOffset (atTime);

            // Ensure zone information initialised.
            //
            tzset ();

            // timezone is seconds West of GMT, whereas actualOffset is seconds East,
            // hence the negation in the equality test.
            //
            if ((actualOffset == -timezone) || (daylight == 0) ) {
               result = tzname [0];
            } else {
               // offsets not equal and daylight available - use it.
               //
               result = tzname [1];
            }
         }
#else
         // Not sure what Windows has to offer (yet).
         //
         result = "???";
#endif
         break;

      case Qt::OffsetFromUTC:    // Don't know what to do here,
      default:                   // or here - both unexpected;
         result = "ERR";
         break;
   }
   return result;
}

//------------------------------------------------------------------------------
//
QString QEUtilities::getTimeZoneTLA (const QDateTime & atTime)
{
   return QEUtilities::getTimeZoneTLA (atTime.timeSpec(), atTime);
}

//------------------------------------------------------------------------------
//
QString QEUtilities::enumToString (const QObject& object,
                                   const QString& enumTypeName,
                                   const int enumValue)
{
   const QMetaObject mo = *object.metaObject();

   return enumToString(mo, enumTypeName, enumValue);
}

QString QEUtilities::enumToString(const QMetaObject& mo,
                                  const QString& enumTypeName,
                                  const int enumValue)
{
   QString result;

   for (int e = 0; e < mo.enumeratorCount(); e++) {
      QMetaEnum metaEnum = mo.enumerator(e);
      if (metaEnum.isValid () && metaEnum.name () == enumTypeName) {
         // found it.
         //
         result = metaEnum.valueToKey (enumValue);
         break;
      }
   }

   return result;
}

//------------------------------------------------------------------------------
//
int QEUtilities::stringToEnum (const QObject& object,
                               const QString& enumTypeName,
                               const QString& enumImage,
                               bool* ok)
{
   const QMetaObject mo = *object.metaObject();

   return stringToEnum (mo, enumTypeName, enumImage, ok);
}

//------------------------------------------------------------------------------
//
int QEUtilities::stringToEnum (const QMetaObject& mo,
                               const QString& enumTypeName,
                               const QString& enumImage,
                               bool* ok)
{
   int result = -1;
   bool okay = false;

   for (int e = 0; e < mo.enumeratorCount(); e++) {
      QMetaEnum metaEnum = mo.enumerator(e);
      if (metaEnum.isValid () && metaEnum.name () == enumTypeName) {
         // found it.
         //
         result = metaEnum.keyToValue (enumImage.trimmed ().toLatin1 ().data ());
         if (result != (-1)) {
            // This is a good value.
            okay = true;
         } else {
            okay = false;  // hypothosize not okay
            for (int i = 0; i < metaEnum.keyCount (); i++) {
               if (result == metaEnum.value (i)) {
                  // This was a valid value after all.
                  //
                  okay = true;
                  break;
               }
            }
         }
         break;
      }
   }

   if (ok) *ok = okay;
   return result;
}

//------------------------------------------------------------------------------
//
QStringList QEUtilities::split (const QString& s)
{
   QStringList result;
   QRegularExpression re ("\\s+", QRegularExpression::NoPatternOption);
   result = s.split (re, QESkipEmptyParts);
   return result;
}

//------------------------------------------------------------------------------
//
QStringList QEUtilities::variantToStringList (const QVariant& v)
{
   QStringList result;
   QStringList vlist;

   result.clear ();
   vlist = v.toStringList ();  // Do variant split
   for (int v = 0; v < vlist.count(); v++) {
      const QString s = vlist.value (v);
      const QStringList slist = QEUtilities::split (s); // Do white space split
      result.append (slist);
   }

   return result;
}

//------------------------------------------------------------------------------
// static
//
QString QEUtilities::dirName (const QString& pathName)
{
   QFileInfo fileInfo (pathName);
   QDir dir = fileInfo.dir ();
   QString result = dir.path ();
   return result;
}

// end
