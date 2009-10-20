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
#ifndef RAPILOCATIONWIDGET_H
#define RAPILOCATIONWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QWheelEvent>
#include "utilities.h"
#include "pose2d.h"
#include "velocity2d.h"
#include "drivetrain2dof.h"
#include "devicewidget.h"


class View : public QGraphicsView
{
  public:
    View( QGraphicsScene* scene, QWidget * widget )
      : QGraphicsView( scene, widget ) {}
  protected:
    static const float factor = 1.2;
    void wheelEvent( QWheelEvent * event )
    {
      float numSteps = event->delta() / ( 8.0 * 15.0 );
      float zoom = pow( factor, numSteps );

      if( event->orientation() == Qt::Vertical )
        scale( zoom, zoom );
    }
};

namespace Rapi
{
/**
 * Plots a robot position and velocity
 * @author Ash Charles
 */
class CLocationWidget : public ADeviceWidget
{
  public:
    /**
     * Default constructor
     * @param velocity
     * @param parent
     */
    CLocationWidget ( ADrivetrain2dof* drivetrain, QWidget* parent );
    /** Default destructor */
    ~CLocationWidget();
    /**
     * Updates the data to be displayed
     * @param pose
     */
    void updateData( void );

  protected:
    /** Creates a painter path to quickly draw a robot */
    QPainterPath createRobot( void );

  private:
    /** Drivetrain we visualize */
    ADrivetrain2dof* mDrivetrain;
    /** a view of the scene (canvas) */
    View* mView;
    /** the canvas/world into which we put graphical items */
    QGraphicsScene* mScene;
    /** the robot graphics item */
    QGraphicsPathItem* mRobot;
    //QGraphicsPixmapItem* mRobot;
    /** vector of line segments showing where the robot has been **/
    QVector<QGraphicsLineItem*> mTrail;
    /** robot location in previous time step */
    QPointF mPrevPoint;
    /** pen colour for drawing trail */
    QColor mColor;
    /** Resolution in X */
    double mRes;
};

} // namespace

#endif
