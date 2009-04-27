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
#ifndef RAPILOOSESTAGELIGHTS_H
#define RAPILOOSESTAGELIGHTS_H

#include "stagelights.h"
#include "stage.hh"

namespace Rapi
{

/**
 * Stage implementation for lights controller
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CLooseStageLights : public CStageLights
{
  public:
    /**
     * Default constructor
     * @param stgModel stage model for blinking lights
     * @param devName name of device
     */
    CLooseStageLights ( Stg::ModelBlinkenlight* stgModel, std::string devName );
    /** Default destructor */
    ~CLooseStageLights();
    /**
     * Gets the underlying stage model
     * @return stage model
     */
    Stg::ModelBlinkenlight* getStageModel ();

};

} // namespace

#endif
