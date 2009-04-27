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

#ifndef RAPILOOSESTAGELASER_H
#define RAPILOOSESTAGELASER_H

#include <stage.hh>
#include "stagelaser.h"

namespace Rapi
{

/**
 * Wrapper class for a stage laser model to make it work with ND
 * @author Jens Wawerla (jwawerla@sfu.ca)
 */
class CLooseStageLaser : public CStageLaser
{
  public:
    /**
     * Default constructor
     * @param stgLaser stage model of laser
     * @param devName name of device
     */
    CLooseStageLaser ( Stg::ModelLaser* stgLaser, std::string devName );
    /** Default destructor */
    virtual ~CLooseStageLaser();
    /**
     * Gets the underlying stage model
     * @return stage model
     */
    Stg::ModelLaser* getStageModel ();
};

} // namespace

#endif
