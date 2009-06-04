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
#include "databar.h"
#include <QVBoxLayout>

namespace Rapi {

//-----------------------------------------------------------------------------
CDataBar::CDataBar(QWidget* parent, QString caption, int min, int max)
 : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout();

  mLabel = new QLabel(this);
  mLabel->setText(caption);
  mDisplay = new QProgressBar(this);
  mDisplay->setRange(min, max);

  layout->addWidget(mLabel);
  layout->addWidget(mDisplay);

  setLayout(layout);
}
//-----------------------------------------------------------------------------
CDataBar::~CDataBar()
{
}
//-----------------------------------------------------------------------------
void CDataBar::setRange(int min, int max)
{
  mDisplay->setRange(min, max);
}
//-----------------------------------------------------------------------------
void CDataBar::setCaption ( QString caption )
{
  mLabel->setText(caption);
}
//-----------------------------------------------------------------------------
void CDataBar::setGeometry ( int x, int y, int w, int h )
{
}
//-----------------------------------------------------------------------------
void CDataBar::setData ( int data )
{
  mDisplay->setValue(data);
}
//-----------------------------------------------------------------------------

} // namespace
