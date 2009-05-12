/*****************************************************************************
 * Project: Forever
 * File:    dataled.cpp
 * Author:  Jens Wawerla (jwawerla_at_sfu.ca)
 *
 *****************************************************************************
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ******************************************************************************
 * $Log: dataled.cpp,v $
 * Revision 1.1  2009-03-23 06:24:22  jwawerla
 * RobotVis works now for data, maps still need more work
 *
 * Revision 1.1  2008/12/04 02:01:03  jwawerla
 * *** empty log message ***
 *
 * Revision 1.2  2007/11/05 21:54:51  jwawerla
 * Version 0.3 works now in windows, need to test erase memory with updated
 * firmware.
 *
 * Revision 1.1  2007/11/02 19:32:45  jwawerla
 * SensorLink v0.3 ready for testing
 *
 * Revision 1.1  2007/11/01 02:44:40  jwawerla
 * SensorLink is the new application with an new communication concept
 *
 * Revision 1.2  2007/08/22 03:16:58  jwawerla
 * Downloading memory seems to work, but it is slow about 1.16 s per dataset
 *
 * Revision 1.1  2007/07/19 05:38:23  jwawerla
 * *** empty log message ***
 *
 * Revision 1.1  2006/07/21 02:49:09  jwawerla
 * first time in cvs
 *
 *
 *****************************************************************************/

#include "dataled.h"
#include <QHBoxLayout>

#include "./images/ledgreenoff.xpm"
#include "./images/ledgreenon.xpm"
#include "./images/ledredon.xpm"
#include "./images/ledredoff.xpm"

const int LED_SIZE = 10;
//-----------------------------------------------------------------------------
CDataLed::CDataLed(QWidget* parent, QString caption)
    : QWidget(parent)
{

  QHBoxLayout* layout = new QHBoxLayout();

  mLabel = new QLabel(this);
  //m_label->setGeometry(20, 1, 110, 19);
  mLabel->setText(caption);
  mDisplay = new QLabel(this);
  //m_display->setGeometry(1, 1, LED_SIZE, LED_SIZE);
  mDisplay->setMaximumSize(LED_SIZE, LED_SIZE);
  mDisplay->setPixmap(QPixmap(ledgreenoff_xpm) );

  layout->addWidget(mDisplay);
  layout->addWidget(mLabel);

  setLayout(layout);

  mData = GREEN_OFF;
}
//-----------------------------------------------------------------------------
CDataLed::~CDataLed()
{
}
//-----------------------------------------------------------------------------
void CDataLed::setCaption(QString caption)
{
  mLabel->setText(caption);
}
//-----------------------------------------------------------------------------
void CDataLed::setGeometry(int x, int y, int w, int h)
{
  QWidget::setGeometry(x, y, w, h);
  mLabel->setGeometry(20, 1, w-2-LED_SIZE, 19);
  mDisplay->setGeometry(1, 5, LED_SIZE, LED_SIZE);
}
//-----------------------------------------------------------------------------
void CDataLed::setData(tLedState data)
{
  if (data != mData) {
    mData = data;

    switch( data) {
    case GREEN_ON:
      mDisplay->setPixmap(QPixmap(ledgreenon_xpm) );
      break;
    case GREEN_OFF:
      mDisplay->setPixmap(QPixmap(ledgreenoff_xpm) );
      break;
    case RED_ON:
      mDisplay->setPixmap(QPixmap(ledredon_xpm) );
      break;
    case RED_OFF:
      mDisplay->setPixmap(QPixmap(ledredoff_xpm) );
      break;
    }
  }
}
//-----------------------------------------------------------------------------


