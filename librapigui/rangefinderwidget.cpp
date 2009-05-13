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
#include "rangefinderwidget.h"
#include "assert.h"
#include <QPaintEvent>

namespace Rapi
{

//---------------------------------------------------------------------------
CRangeFinderWidget::CRangeFinderWidget ( ARangeFinder* rangeFinder,
    QWidget* parent ) : ADeviceWidget ( parent )
{
  assert(rangeFinder);
  setTitle ( "Rangefinder" );

  mPolygonItem = NULL;
  mRobotPolygonItem = NULL;
  mRangeFinder = rangeFinder;

  // create a scene and add it to the widget
  mGraphicsScene = new QGraphicsScene ( this );
  mGraphicsScene->setSceneRect ( -100, -100, 200, 200 ); // x, y, w, h

  mGraphicsView = new QGraphicsView ( mGraphicsScene, this );
  mGraphicsView->setMinimumSize(202,202);
  mGraphicsView->setDisabled(true);
  mMainLayout->addWidget ( mGraphicsView );

  mNumPolygon = 0;
}
//---------------------------------------------------------------------------
CRangeFinderWidget::~CRangeFinderWidget()
{
  // nothing to do
}
//---------------------------------------------------------------------------
void CRangeFinderWidget::createPolygons ( int n )
{
  QPolygonF polygon;
  float robotLength = 7.5;
  float robotWidth  = 5.0;

  if ( mRangeFinder->getBeamConeAngle() == 0 ) {
    // we are okay, we only need one polygon for drawing laser data
    if ( mNumPolygon == 1 ) {
      return;
    }

    // clear old stuff if there is any
    if ( mPolygonItem != NULL )
      delete[] mPolygonItem;

    // now create one polygon for the whole range
    mPolygonItem = new QGraphicsPolygonItem*[1];
    mPolygonItem[0] = mGraphicsScene->addPolygon ( polygon,
                      QPen ( QColor ( 0, 0, 255 ), 1, Qt::SolidLine ),
                      QBrush ( QColor ( 0, 0, 255 ), Qt::Dense6Pattern ) );
    mNumPolygon = 1;
  }
  else {

    // clear old stuff if there is any
    if ( mPolygonItem != NULL )
      delete[] mPolygonItem;

    // now create one polygon per reading
    mNumPolygon = n;
    mPolygonItem = new QGraphicsPolygonItem*[mNumPolygon];

    for ( int i = 0; i < n; i++ ) {
      mPolygonItem[i] = mGraphicsScene->addPolygon ( polygon,
                        QPen ( QColor ( 0, 255, 0 ), 1, Qt::SolidLine ),
                        QBrush ( QColor ( 0, 255, 0 ), Qt::Dense6Pattern ) );
    }
  }

  // add robot
  if (mRobotPolygonItem)
    delete mRobotPolygonItem;

  mRobotPolygonItem = new QGraphicsPolygonItem();
  mRobotPolygonItem = mGraphicsScene->addPolygon ( polygon,
                        QPen ( QColor ( 255, 0, 0 ), 1, Qt::SolidLine ) );

  polygon = mRobotPolygonItem->polygon();
  polygon.clear();

  polygon << QPointF(  0.0,  0.0);
  polygon << QPointF(  robotLength,  robotWidth);
  polygon << QPointF( -robotLength,  robotWidth);
  polygon << QPointF( -robotLength, -robotWidth);
  polygon << QPointF(  robotLength, -robotWidth);
  polygon << QPointF(  robotLength,  robotWidth);
  polygon << QPointF(  0.0,  0.0);
  polygon << QPointF(  robotLength, -robotWidth);
  mRobotPolygonItem->setPolygon(polygon);

}
//---------------------------------------------------------------------------
void CRangeFinderWidget::updateData()
{
  QPolygonF polygon;
  float halfConeAngle;
  int sX, sY, x, y; // [pixel]
  float r;          // [pixel]
  float pixelPerMeter;

  // check if device still exists
  if ( mRangeFinder == NULL)
    return;

  // call update of base class
  ADeviceWidget::updateData ( mRangeFinder );

  // should we display stuff or are we turned off
  if ( isChecked() == false ) {
    mGraphicsView->setHidden(true);
    return;
  }
  mGraphicsView->setHidden(false);

  // if the number of samples is zero, there is nothing to draw and we can
  // quit right here
  if ( mRangeFinder->getNumSamples() == 0 )
    return;

  // check if the beam configuration has changened and if, so reallocate
  // memory
  if ( mRangeFinder->getNumSamples() != mNumPolygon ) {
    createPolygons ( mRangeFinder->getNumSamples() );
  }

  // resolution
  pixelPerMeter = (mGraphicsScene->height() * 0.5) / mRangeFinder->getMaxRange();

  // is it a laser (beam cone angle = 0) or not ?
  if ( mRangeFinder->getBeamConeAngle() == 0 ) {
    // insert start location of range reading
    sX = ( int ) ( mRangeFinder->mRelativeBeamPose[0].mX * pixelPerMeter );
    sY = ( int ) ( mRangeFinder->mRelativeBeamPose[0].mY * pixelPerMeter );

    polygon = mPolygonItem[0]->polygon();
    polygon.clear();

    polygon << QPointF ( sX, sY );
    for ( unsigned int i = 0; i < mRangeFinder->getNumSamples(); i++ ) {
      // insert range reading
      r = mRangeFinder->mRangeData[i].range * pixelPerMeter;
      x = ( int ) ( sX + r * cos ( mRangeFinder->mRelativeBeamPose[i].mYaw ) );
      y = ( int ) ( sY + r * sin ( mRangeFinder->mRelativeBeamPose[i].mYaw ) );

      polygon << QPointF ( x, -y );
    }

    // insert end point
    polygon << QPointF ( sX, sY );
    mPolygonItem[0]->setPolygon ( polygon );
  }
  else {
    halfConeAngle = mRangeFinder->getBeamConeAngle() / 2.0f;

    for ( unsigned int i = 0; i < mRangeFinder->getNumSamples(); i++ ) {
      // insert start location of range reading
      sX = ( int ) ( mRangeFinder->mRelativeBeamPose[i].mX * pixelPerMeter );
      sY = ( int ) ( mRangeFinder->mRelativeBeamPose[i].mY * pixelPerMeter );
      polygon = mPolygonItem[i]->polygon();
      polygon.clear();
      polygon << QPointF ( sX, sY );
      // insert range reading
      r = mRangeFinder->mRangeData[i].range * pixelPerMeter;

      x = ( int ) ( sX + r * cos ( mRangeFinder->mRelativeBeamPose[i].mYaw -
                                   halfConeAngle ) );
      y = ( int ) ( sY + r * sin ( mRangeFinder->mRelativeBeamPose[i].mYaw -
                                   halfConeAngle ) );
      polygon << QPointF ( x, y );
      x = ( int ) ( sX + r * cos ( mRangeFinder->mRelativeBeamPose[i].mYaw +
                                   halfConeAngle ) );
      y = ( int ) ( sY + r * sin ( mRangeFinder->mRelativeBeamPose[i].mYaw +
                                   halfConeAngle ) );
      polygon << QPointF ( x, -y );
      polygon << QPointF ( sX, sY );
      mPolygonItem[i]->setPolygon ( polygon );
    }
  }
}

//---------------------------------------------------------------------------

} // namespace
