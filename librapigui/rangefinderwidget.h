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
#ifndef RAPIRANGEFINDERVIEW_H
#define RAPIRANGEFINDERVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QHBoxLayout>
#include "devicewidget.h"
#include "rangefinder.h"
#include "math.h"

namespace Rapi
{

/**
 * This class provides a view for range finders, that is it visualizes the
 * range finders data
 * @author Jens Wawerla <jwawerla@sfu.ca>
 * @version 0.1 - 11/2007
 */
class CRangeFinderWidget : public ADeviceWidget
{
  public:
    /**
     * Default constructor
     * @param parent Qt parent widget
     */
    CRangeFinderWidget( ARangeFinder* rangefinder, QWidget* parent = NULL );
    /** Default destructor */
    ~CRangeFinderWidget();
    /** Updates the data display */
    void updateData();

  protected:
    /**
     * Creates/allocates/reallocates polygons for drawing
     * @param n number of samples to draw
     */
    void createPolygons(int n);

  private:
    /** Range finder to visualize */
    ARangeFinder* mRangeFinder;
    /** Qt graphics view */
    QGraphicsView* mGraphicsView;
    /** Qt graphics scene */
    QGraphicsScene* mGraphicsScene;
    /** Range ray polygon */
    QGraphicsPolygonItem** mPolygonItem;
    /** Robot polygon */
    QGraphicsPolygonItem* mRobotPolygonItem;
    /** Number of polygons */
    unsigned int mNumPolygon;
};

} // namespace
#endif
