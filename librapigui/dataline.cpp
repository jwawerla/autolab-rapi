/*****************************************************************************
 * Project: Forever
 * File:    dataline.cpp
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
 * $Log: dataline.cpp,v $
 * Revision 1.1  2009-03-23 06:24:22  jwawerla
 * RobotVis works now for data, maps still need more work
 *
 *
 *****************************************************************************/

#include "dataline.h"
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
CDataLine::CDataLine ( QWidget* parent, QString caption )
    : QWidget ( parent )
{
  QVBoxLayout* layout = new QVBoxLayout();

  mFgColor = false;
  mLabel = new QLabel ( this );
  //m_label->setGeometry(1,1,70,19);
  layout->addWidget ( mLabel );
  mLabel->setText ( caption );

  mLineEdit = new QLineEdit ( this );
  //m_lineEdit->setGeometry(1,20,70,17);
  layout->addWidget ( mLineEdit );
  mLineEdit->setText ( "Na" );
  mLineEdit->setReadOnly ( true );
  mLineEdit->setAlignment ( Qt::AlignRight );
  mFloatPrecision = 1;

  setMaximumHeight ( 70 );

  setLayout ( layout );
}
//-----------------------------------------------------------------------------
CDataLine::~CDataLine()
{
}
//-----------------------------------------------------------------------------
void CDataLine::setColorEnable(bool on)
{
  mFgColor = on;
  setAutoFillBackground( mFgColor );
}
//-----------------------------------------------------------------------------
void CDataLine::setAlignment ( t_alignment align )
{
  switch ( align ) {
    case LEFT:
      mLineEdit->setAlignment ( Qt::AlignLeft );
      break;
    case RIGHT:
      mLineEdit->setAlignment ( Qt::AlignRight );
      break;
    case CENTER:
      mLineEdit->setAlignment ( Qt::AlignHCenter );
      break;
  } // switch
}
//-----------------------------------------------------------------------------
void CDataLine::setCaption ( QString caption )
{
  mLabel->setText ( caption );
}
//-----------------------------------------------------------------------------
void CDataLine::setGeometry ( int x, int y, int w, int h )
{
  QWidget::setGeometry ( x, y, w, h );
  mLabel->setGeometry ( 1, 1, w - 2, 19 );
  mLineEdit->setGeometry ( 1, 20, w - 2, 17 );
}
//-----------------------------------------------------------------------------
void CDataLine::setFloatPrecision ( unsigned int p )
{
  mFloatPrecision = p;
}
//-----------------------------------------------------------------------------
void CDataLine::setData ( double data )
{
  QString dataStr;

  if ( mFgColor ) {
    if ( data < 0.0 )
      mPalette.setColor ( QPalette::Background, Qt::red );
    else
      mPalette.setColor ( QPalette::Background, Qt::green );
  }
  mLineEdit->setPalette ( mPalette );

  dataStr = dataStr.setNum ( data, 'f', mFloatPrecision );
  mLineEdit->setText ( dataStr );
}
//-----------------------------------------------------------------------------
void CDataLine::setData ( QString data )
{
  mLineEdit->setText ( data );
}
//-----------------------------------------------------------------------------
void CDataLine::setData ( int data )
{
  QString dataStr;

  if ( mFgColor ) {
    if ( data < 0 )
      mPalette.setColor ( QPalette::Background, Qt::red );
    else
      mPalette.setColor ( QPalette::Background, Qt::green );
  }
  mLineEdit->setPalette ( mPalette );

  dataStr = dataStr.setNum ( data );
  mLineEdit->setText ( dataStr );
}
//-----------------------------------------------------------------------------
void CDataLine::setData ( unsigned int data )
{
  QString dataStr;

  dataStr = dataStr.setNum ( data );
  mLineEdit->setText ( dataStr );
}
//-----------------------------------------------------------------------------
