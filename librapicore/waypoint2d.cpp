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
#include "waypoint2d.h"
#include "utilities.h"

namespace Rapi
{

//-----------------------------------------------------------------------------
CWaypoint2d::CWaypoint2d ( double x, double y, double yaw, std::string label,
CRgbColor color )
{
  mPose = CPose2d ( x,y,yaw );
  mLabel = label;
  mColor = color;
}
//-----------------------------------------------------------------------------
CWaypoint2d::CWaypoint2d ( CPose2d pose, std::string label, CRgbColor color)
{
  mPose = pose;
  mLabel = label;
  mColor = color;
}
//-----------------------------------------------------------------------------
CWaypoint2d::~CWaypoint2d()
{
}
//-----------------------------------------------------------------------------
void CWaypoint2d::print()
{
  printf("CWaypoint2d: %s at", mLabel.c_str());
  mPose.print();
}
//-----------------------------------------------------------------------------
void CWaypoint2d::operator= ( const CPose2d pose )
{
  mPose = pose;
}
//-----------------------------------------------------------------------------
void CWaypoint2d::operator= ( const CWaypoint2d wp )
{
  mPose = wp.mPose;
  mLabel = wp.mLabel;
  mColor = wp.mColor;
}
//-----------------------------------------------------------------------------
CPose2d CWaypoint2d::getPose()
{
  return mPose;
}
//-----------------------------------------------------------------------------
std::string CWaypoint2d::getLabel()
{
  return mLabel;
}
//-----------------------------------------------------------------------------
void CWaypoint2d::setPose(CPose2d pose)
{
  mPose = pose;
}
//-----------------------------------------------------------------------------
void CWaypoint2d::setPose(CPoint2d point, double heading )
{
  mPose = point;
  mPose.mYaw = NORMALIZE_ANGLE( heading );
}
//-----------------------------------------------------------------------------
void CWaypoint2d::setLabel( std::string label)
{
  mLabel = label;
}
//-----------------------------------------------------------------------------
CRgbColor CWaypoint2d::getColor()
{
  return mColor;
}
//-----------------------------------------------------------------------------
void CWaypoint2d::setColor(CRgbColor color)
{
  mColor = color;
}
//-----------------------------------------------------------------------------
} // namespace
