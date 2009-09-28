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

#ifndef RAPIDEVICETYPES_H
#define RAPIDEVICETYPES_H

namespace Rapi
{
/**
 * Definition of device types
 */
typedef enum  {RAPI_RANGEFINDER,
               RAPI_DRIVETRAIN2DOF,
               RAPI_LOCALIZER,
               RAPI_POWERPACK,
               RAPI_FIDUCIALFINDER,
               RAPI_LIGHTS,
               RAPI_TEXTDISPLAY,
               RAPI_BINARYSENSORARRAY,
               RAPI_SWITCHARRAY,
               RAPI_ANALOGSENSORARRAY,
               RAPI_BLOBFINDER
              } tRapiDeviceType;

/**
 * Mode for writing files
 */
typedef enum {OVERWRITE, APPEND} tWriteMode;

} // namespace
#endif
