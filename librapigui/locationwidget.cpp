/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Ash Charles(jac27@sfu.ca)                                 *
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
#include "locationwidget.h"

const int HISTORY_LENGTH = 360;

namespace Rapi
{

//-----------------------------------------------------------------------------
CLocationWidget::CLocationWidget ( ADrivetrain2dof* drivetrain,
                                   QWidget* parent )
    : ADeviceWidget ( parent ), mColor( "Red" )
{
  mDrivetrain = drivetrain;
  setTitle( "Location" );
  mScene = new QGraphicsScene( this );
  mView = new View( mScene, this );
  mView->setDragMode( QGraphicsView::ScrollHandDrag );
  mView->scale( 1.0, -1.0 ); // Coord orgin: Rapi = bottom left, Qt = top left
  // let's use a 2[m] wide arena to start
  mRes = mView->width() / 2.0;


  // add stuff to the scene
  CPose2d pose = mDrivetrain->getOdometry()->getPose();
  mPrevPoint = QPointF( mRes*pose.mX, mRes*pose.mY );
  mRobot = mScene->addPath( createRobot(), QPen( QColor( "Red" ), 2 ) );
  //QPixmap icon( "/home/jac27/robot.png" );
  //int width = 40;
  //mRobot = mScene->addPixmap( icon.scaledToWidth( width ) );
  //mRobot->setOffset( 0, -width/2 ); // centre pixmap
  mRobot->setPos( mPrevPoint );

  mMainLayout->addWidget( mView );
  mView->show();
}
//-----------------------------------------------------------------------------
CLocationWidget::~CLocationWidget()
{
  if( mView )
    delete mView;
  if( mScene )
    delete mScene;
}
//------------------------------------------------------------------------------
QPainterPath CLocationWidget::createRobot( void )
{
  QPainterPath robot;
  float robotLength = 10.0;
  float robotWidth = 5.0;
  robot.moveTo( robotWidth, -robotLength );
  robot.lineTo( robotWidth, robotLength );
  robot.lineTo( -robotWidth, robotLength );
  robot.lineTo( -robotWidth, -robotLength );
  robot.lineTo( robotWidth, -robotLength );
  robot.lineTo( 0, 0 );
  robot.lineTo( robotWidth, robotLength );
  return robot;
}
//-----------------------------------------------------------------------------
void CLocationWidget::updateData ( void )
{
  if ( isChecked() )
    mView->setHidden( false );
  else
    mView->setHidden( true );

  CVelocity2d velocity = mDrivetrain->getVelocity();
  CPose2d pose = mDrivetrain->getOdometry()->getPose();
  QPointF currPoint( mRes*pose.mX, mRes*pose.mY );
  // move robot
  mRobot->resetTransform();
  mRobot->setPos( currPoint );
  mRobot->rotate( R2D( pose.mYaw ) );
  // draw new trail segment
  int h, s, v;
  mColor.getHsv( &h, &s, &v );
  mColor.setHsv( (h+1) % 360, s, v );
  mTrail.push_back( mScene->addLine( QLineF(mPrevPoint, currPoint),
                      QPen( mColor, 2 ) ) );
  if( mTrail.size() > HISTORY_LENGTH ) {
    mScene->removeItem( mTrail.front() );
    mTrail.pop_front();
  }
  // update
  ADeviceWidget::updateData ( mDrivetrain );
  mView->update();
  mPrevPoint = currPoint;
}
//-----------------------------------------------------------------------------
} // namespace
