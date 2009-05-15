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
#ifndef RAPICONSOLWIDGET_H
#define RAPICONSOLWIDGET_H

#include "devicewidget.h"
#include <QTextEdit>
#include <string>

namespace Rapi {

/**
 * Console that displays the rprintf information from a robot controller
 * @author Jens Wawerla
 */
class CConsoleWidget : public ADeviceWidget
{
  public:
    /**
     * Default constructor
     * @param parent
     */
    CConsoleWidget(std::string* str, QWidget* parent);
    /** Default destructor */
    ~CConsoleWidget();
    /** Update the display */
    void updateData();

  protected:
    /** Text display */
    QTextEdit* mTextEdit;
    std::string* mRPrintfString;
};

} // namespace

#endif
