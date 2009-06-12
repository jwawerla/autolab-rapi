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
#include "cbwallsensor.h"
#include "utilities.h"
#include "cbfiducialfinder.h"
#include <assert.h>
#include <math.h>

namespace Rapi {

//-----------------------------------------------------------------------------
CCBFiducialFinder::CCBFiducialFinder(CCBDriver* driver, std::string devName)
 : AFiducialFinder(devName)
{
	assert(driver);
	mCBDriver = driver;
	mNumReadings = 1;

	mFiducialData = new tFiducialData();
	mFiducialData->range = -1;
	mFiducialData->bearing = 0;
	mFiducialData->id = 255;

	mMinRange = 0.0;
	mMaxRange = 4.0;
	mFov = 2*M_PI;

	setEnabled ( true );
}
//-----------------------------------------------------------------------------
CCBFiducialFinder::~CCBFiducialFinder()
{
	if (mFiducialData)
		delete mFiducialData;
	mFiducialData = NULL;
}
//-----------------------------------------------------------------------------
void CCBFiducialFinder::updateData()
{
	mFiducialData->id = mCBDriver->mCreateSensorPackage.ir;
}
//-----------------------------------------------------------------------------
void CCBFiducialFinder::setEnabled(bool enable)
{
	mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
int CCBFiducialFinder::init()
{
	return 1; // success
}
//-----------------------------------------------------------------------------
void CCBFiducialFinder::
setFiducialSignal(int id)
{
	if (id && ~255) {
		fprintf(stderr, "Invalid fiducial ID %d; must be in [0..255].\n", id);
		return;
	}

	mOwnFiducialId = id;
}
//-----------------------------------------------------------------------------
void CCBFiducialFinder::
print(void) const
{
	printf("CBFiducialFinder: reading %u, I am %u.  ",
		mFiducialData->id, mOwnFiducialId);
    
	/* Remote control bytes */
	switch(mFiducialData->id) {
	case CBFIDUCIAL_LEFT: printf("LEFT"); break;
	case CBFIDUCIAL_FORWARD: printf("FORWARD"); break;
	case CBFIDUCIAL_RIGHT: printf("RIGHT"); break;
	case CBFIDUCIAL_SPOT: printf("SPOT"); break;
	case CBFIDUCIAL_MAX: printf("MAX"); break;
	case CBFIDUCIAL_SMALL: printf("SMALL"); break;
	case CBFIDUCIAL_MEDIUM: printf("MEDIUM"); break;
	case CBFIDUCIAL_LARGE_CLEAN: printf("LARGE_CLEAN"); break;
	case CBFIDUCIAL_PAUSE: printf("PAUSE"); break;
	case CBFIDUCIAL_POWER: printf("POWER"); break;
	case CBFIDUCIAL_ARC_FORWARD_LEFT: printf("ARC_FORWARD_LEFT"); break;
	case CBFIDUCIAL_ARC_FORWARD_RIGHT: printf("ARC_FORWARD_RIGHT"); break;
	case CBFIDUCIAL_DRIVE_STOP: printf("DRIVE_STOP"); break;

	/* Scheduling remote bytes */
	case CBFIDUCIAL_SEND_ALL: printf("SEND_ALL"); break;
	case CBFIDUCIAL_SEEK_DOCK: printf("SEEK_DOCK"); break;

	/* Home base bytes */
	case CBFIDUCIAL_RESERVED: printf("RESERVED"); break;
	case CBFIDUCIAL_RED_BUOY: printf("RED_BUOY"); break;
	case CBFIDUCIAL_GREEN_BUOY: printf("GREEN_BUOY"); break;
	case CBFIDUCIAL_FORCE_FIELD: printf("FORCE_FIELD"); break;
	case CBFIDUCIAL_RED_AND_GREEN_BUOYS: printf("RED_AND_GREEN_BUOYS"); break;
	case CBFIDUCIAL_RED_AND_FORCE_FIELD: printf("RED_AND_FORCE_FIELD"); break;
	case CBFIDUCIAL_GREEN_AND_FORCE_FIELD: printf("GREEN_AND_FORCE_FIELD"); break;
	case CBFIDUCIAL_RED_GREEN_AND_FORCE_FIELD: printf("RED_GREEN_AND_FORCE_FIELD"); break;

	/* something else */
	default: printf("(unknown)\n"); break;
	}

	putchar('\n');
}
//-----------------------------------------------------------------------------

} // namespace
