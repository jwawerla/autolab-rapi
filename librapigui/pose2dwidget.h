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
#ifndef RAPIPOSE2DWIDGET_H
#define RAPIPOSE2DWIDGET_H

#include <qgroupbox.h>
#include "pose2d.h"
#include "dataline.h"

namespace Rapi
{

/**
 * Displays a pose
 * @author Jens Wawerla
 */
class CPose2dWidget : public QGroupBox
{
  public:
    /**
     * Default constructor
     * @param velocity
     * @param parent
     */
    CPose2dWidget ( QWidget* parent );
    /** Default destructor */
    ~CPose2dWidget();
    /**
     * Updates the data to be displayed
     * @param pose
     */
    void updateData( CPose2d pose);

  protected:
    CDataLine* mXLine;
    CDataLine* mYLine;
    CDataLine* mYawLine;

};

} // namespace

#endif
