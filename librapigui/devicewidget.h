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
#ifndef RAPIDEVICEWIDGET_H
#define RAPIDEVICEWIDGET_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include "dataled.h"
#include "device.h"

namespace Rapi
{

/**
 * Abstract base class for device widgets
 * @author Jens Wawerla
 */
class ADeviceWidget : public QGroupBox
{
  Q_OBJECT

  public:
    /** Default destructor */
    virtual ~ADeviceWidget();
    /** Update the data of the widget */
    virtual void updateData() = 0;

  protected slots:
    virtual void toggled(bool on );

  protected:
    /**
     * Default constructor
     * @param parent widget
     */
    ADeviceWidget ( QWidget* parent );
    /**
     * Updates the general information
     * @param device to get data from
     */
    void updateData(ADevice* device);

    /** Main layout component */
    QVBoxLayout* mMainLayout;
    /** Device enabled LED */
    CDataLed* mEnabledLed;
    /** General info box */
    QGroupBox* mGeneralInfoBox;
    /** General info box layout */
    QHBoxLayout* mGeneralInfoBoxLayout;
};

} // namespace

#endif
