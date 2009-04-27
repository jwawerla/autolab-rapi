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
#ifndef RAPILOOSESTAGEFIDUCIALFINDER_H
#define RAPILOOSESTAGEFIDUCIALFINDER_H

#include "stagefiducialfinder.h"
#include "stage.hh"

namespace Rapi
{

/**
 * Stage implementation of a fiducial finder
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CLooseStageFiducialFinder : public CStageFiducialFinder
{
  public:
    /**
     * Default constructor
     * @param stgMod stage model of a fiducial finder
     * @param devName name of device
     */
    CLooseStageFiducialFinder ( Stg::ModelFiducial* stgMod, std::string devName );
    /** Default destructor */
    ~CLooseStageFiducialFinder();
    /**
     * Gets the underlying stage model
     * @return stage model
     */
    Stg::ModelFiducial* getStageModel ();
};

} // namespace

#endif
