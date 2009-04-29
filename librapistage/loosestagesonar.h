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

#ifndef RAPILOOSESTAGESONAR_H
#define RAPILOOSESTAGESONAR_H

#include "stage.hh"
#include "stagesonar.h"

namespace Rapi
{

/**
 * This class provides a sonar simulated by stage
 *  @author Jens Wawerla <jwawerla@sfu.ca>
 */

class CLooseStageSonar : public CStageSonar
{
  // we are now friends with our robot
  friend class CLooseStageRobot;

  public:
    /**
     * Default constructor
     * @param stgModel stage model of sonar
     * @param devName name of device
     */
    CLooseStageSonar ( Stg::ModelRanger* stgModel, std::string devName );
    /** Default destructor */
    ~CLooseStageSonar();
    /**
     * Gets the underlying stage model
     * @return stage model
     */
    Stg::ModelRanger* getStageModel ();
};

} // namespace

#endif
