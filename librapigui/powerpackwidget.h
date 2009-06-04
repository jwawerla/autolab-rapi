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
#ifndef RAPIPOWERMANAGEMENTVIEW_H
#define RAPIPOWERMANAGEMENTVIEW_H

#include "devicewidget.h"
#include "powerpack.h"
#include "dataline.h"
#include "dataled.h"
#include "databar.h"
#include <QGroupBox>
#include <list>

namespace Rapi
{

/**
 * Visualization for the power management
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 01/2008
 */
class CPowerPackWidget : public ADeviceWidget
{
  public:
    /**
     * Default constructor
     * @param parent Qt's parent object
     * @param powerpack to visualize
     */
    CPowerPackWidget(APowerPack* powerpack, QWidget* parent = NULL );
    /** Default destructor */
    ~CPowerPackWidget();
    /** Update display */
    void updateData();

  private:
    /** Power pack device to visualize */
    APowerPack* mPowerPack;
    /** Battery level */
    CDataBar* mBatteryLevelBar;
    /** Battery capacity display */
    CDataLine* mBatCapacity;
    /** Maximal Battery capacity display */
    CDataLine* mMaxBatCapacity;
    /** Current drawn from battery display */
    CDataLine* mCurrent;
    /** Voltage of battery  */
    CDataLine* mVoltage;
    /** Battery temperature */
    CDataLine* mBatTemp;
    /** Charging Led */
    CDataLed* mChargingLed;
    /** Charging source */
    CDataLine* mChargingSource;
    /** Charging state */
    CDataLine* mChargingState;
    /** Total energy dissipated */
    CDataLine* mDissipated;
    /** Group box for battery data */
    QGroupBox* mBatteryBox;
    /** Group box for charger information */
    QGroupBox* mChargingBox;

};

} // namespace
#endif
