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
#ifndef RAPIFIDUCIALFINDERWIDGET_H
#define RAPIFIDUCIALFINDERWIDGET_H

#include "devicewidget.h"
#include "fiducialfinder.h"
#include "dataline.h"
#include <QTableWidget>

namespace Rapi
{

/**
 * Widget to visualize fiducial data
 * @author Jens Wawerla
 */
class CFiducialFinderWidget : public ADeviceWidget
{
  Q_OBJECT

  public:
    /**
     * Default constructor
     * @param fiducial to display
     * @param parent of class
     */
    CFiducialFinderWidget ( AFiducialFinder* fiducial, QWidget* parent = NULL );
    /** Default destructor */
    ~CFiducialFinderWidget();
    /** Update the data of the display */
    void updateData ();

  protected slots:
    virtual void toggled(bool on );

  protected:
    /** Fiducial finder */
    AFiducialFinder* mFiducialFinder;
    /** Table to display fiducial data */
    QTableWidget* mTableWidget;
    /** Field of view  */
    CDataLine* mFov;
    /** Minimum Range */
    CDataLine* mMinRange;
    /** Maximum Range */
    CDataLine* mMaxRange;
    /** Own fiducial id */
    CDataLine* mOwnFiducialId;

};

} // namespace

#endif
