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
#ifndef RAPIVELOCITY2DWIDGET_H
#define RAPIVELOCITY2DWIDGET_H

#include <qgroupbox.h>
#include "velocity2d.h"
#include "dataline.h"

namespace Rapi
{

/**
 * Displays velocities
 * @author Jens Wawerla
 */
class CVelocity2dWidget : public QGroupBox
{
  public:
    /**
     * Default constructor
     * @param velocity
     * @param parent
     */
    CVelocity2dWidget ( QWidget* parent );
    /** Default destructor */
    ~CVelocity2dWidget();
    /**
     * Sets data to be displayed
     * @param velocity
     */
    void setData( CVelocity2d velocity);

  protected:
    CDataLine* mVXLine;
    CDataLine* mVYLine;
    CDataLine* mYawDotLine;

};

} // namespace

#endif
