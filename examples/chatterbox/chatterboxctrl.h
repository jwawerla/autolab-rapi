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
#ifndef CHATTERBOXCTRL_H
#define CHATTERBOXCTRL_H

#include <RapiChatterbox>

using namespace Rapi;

/**
 * An example controller for chatterbox
 * @author Jens Wawerla
 */
class CChatterboxCtrl : public ARobotCtrl
{
  public:
    /**
     * Default constructor
     * @param robot this controller controls
     */
    CChatterboxCtrl ( ARobot* robot );
    /** Default destructor */
    ~CChatterboxCtrl();


  protected:
    /**
     * Update controller for the current time step
     * @param dt time since last upate [s]
     */
    void updateData(float dt);
    /** Obstacle avoidance routine */
    void obstacleAvoid();
    /** Drivetrain */
    ADrivetrain2dof* mDrivetrain;
    /** Infrared sesnors */
    ARangeFinder* mIr;
    /** Power pack */
    APowerPack* mPowerPack;
    /** Text display */
    ATextDisplay* mTextDisplay;
    /** Lights */
    ALights* mLights;
    /** Bumper */
    ABinarySensorArray* mBumper;
    /** Wheel drop */
    ABinarySensorArray* mWheelDrop;
    /** Low side driver */
    ASwitchArray* mLowSideDriver;
    /** Laser range finder */
    ARangeFinder* mLaser;
    /** Top fiducial */
    AFiducialFinder* mTopFiducial;
    /** Front fiducial */
    AFiducialFinder* mFrontFiducial;
    /** Photo sensor */
    AAnalogSensorArray* mPhoto;
    /** Create button */
    ABinarySensorArray* mButton;
    /** Some limit */
    CLimit mLimit;

};

#endif
