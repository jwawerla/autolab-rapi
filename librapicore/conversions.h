/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

#ifndef RAPICONVERSIONS_H
#define RAPICONVERSIONS_H

namespace Rapi
{

/** Converts from joules to watt hours */
inline double JOULES_TO_WATTHOURS ( double joules )
{
  return joules / 3600.0;
}

/** Converts from hours to seconds */
inline double HOURS_TO_SECONDS ( double h )
{
  return h * 3600.0;
}

/** Converts from secounds to hours */
inline double SECONDS_TO_HOURS ( double s )
{
  return s / 3600.0;
}

/** Converts from joules and a voltage to ampere hours */
inline double JOULES_TO_AMPHOURS ( double joules, double volts )
{
  return joules / ( 3600.0 * volts );
}

} // namespace

#endif
