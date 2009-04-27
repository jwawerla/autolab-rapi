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

#ifndef RAPILOOSESTAGEROBOT_H
#define RAPILOOSESTAGEROBOT_H

#include "stagerobot.h"
#include "stage.hh"
#include "loosestagelaser.h"
#include "loosestagesonar.h"
#include "loosestagedrivetrain2dof.h"
#include "loosestagepowerpack.h"
#include "loosestagefiducialfinder.h"
#include "loosestagetextdisplay.h"
#include "loosestagelights.h"

namespace Rapi
{

/**
 * This robot gets devices for a robot simulated in Stage. The driver
 * classes are of a LOOSE standard, meaning they are not 100% compatible
 * with other RAPI drivers. Code writting using these drivers might
 * not be portable to a different robot.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CLooseStageRobot : public CStageRobot
{
  public:
    /** Default constructor */
    CLooseStageRobot ( Stg::Model* mod );
    /** Default destructor */
    ~CLooseStageRobot();
    /**
     * Ininitializes the robot
     * @return 1 if successfull, 0 otherwise
     */
    virtual int init();
    /**
     * Gets a device with a given device index
     * @param devName name of device
     * @return device
     * @return 1 if successfull, 0 otherwise
     */
    virtual int findDevice ( ARangeFinder* &device, std::string devName );
    virtual int findDevice ( ADrivetrain2dof* &device, std::string devName );
    virtual int findDevice ( APowerPack* &device, std::string devName );
    virtual int findDevice ( AFiducialFinder* &device, std::string devName );
    virtual int findDevice ( ALights* &device, std::string devName );
    virtual int findDevice ( ATextDisplay* &device, std::string devName );
    /**
     * Gets a device with a given device index, using the LOOSE standard
     * @param devName name of device
     * @return device
     * @return 1 if successfull, 0 otherwise
     */
    virtual int findDevice ( CLooseStageLaser* &device, std::string devName );
    virtual int findDevice ( CLooseStageSonar* &device, std::string devName );
    virtual int findDevice ( CLooseStageDrivetrain2dof* &device, std::string devName );
    virtual int findDevice ( CLooseStagePowerPack* &device, std::string devName );
    virtual int findDevice ( CLooseStageFiducialFinder* &device, std::string devName );
    virtual int findDevice ( CLooseStageLights* &device, std::string devName );
    virtual int findDevice ( CLooseStageTextDisplay* &device, std::string devName );

  protected:
    /**
     * Searches the list of previously found devices and returns a match or
     * NULL if no device was filed under the given device name
     * @param devName to search
     * @return device or NULL
     */
    ADevice* findDeviceByName ( std::string devName );

  private:
    /** Main Stage model */
    Stg::Model*mStageModel;
    /** List of all devices generated */
    std::list<ADevice*> mDeviceList;
};

} // namespace

#endif
