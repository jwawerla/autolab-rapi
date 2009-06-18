/***************************************************************************
 * Project: autolab-wp                                                     *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: ccolor.cpp,v 1.1.1.1 2009-03-15 03:52:03 jwawerla Exp $
 ***************************************************************************
 *                                                                         *
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
 ***************************************************************************
 * $Log: $
 *
 *
 ***************************************************************************/
#include "rgbcolor.h"
#include "utilities.h"
#include "stdio.h"
#include <sstream>

namespace Rapi
{

//---------------------------------------------------------------------------
CRgbColor::CRgbColor ( unsigned char r, unsigned char g, unsigned char b )
{
  mRed = r;
  mGreen = g;
  mBlue = b;
}
//---------------------------------------------------------------------------
CRgbColor::CRgbColor ( CRgbColor const &color )
{
  mRed = color.mRed;
  mGreen = color.mGreen;
  mBlue = color.mBlue;
}
//---------------------------------------------------------------------------
CRgbColor::~CRgbColor()
{
}
//---------------------------------------------------------------------------
std::string CRgbColor::toStr() const
{
  std::ostringstream strOut;

  strOut << "r=" << mRed << " g=" << mGreen << " b=" << mBlue;
  return strOut.str();
}
//---------------------------------------------------------------------------
std::string CRgbColor::toCSV() const
{
  std::ostringstream strOut;

  strOut << mRed << "," << mGreen << "," << mBlue;
  return strOut.str();
}
//---------------------------------------------------------------------------
std::string CRgbColor::getCVSHeader() const
{
  return "red, green, blue";
}
//---------------------------------------------------------------------------
void CRgbColor::print() const
{
  printf ( "CRgbColor %d %d %d \n", mRed, mGreen, mBlue );
}
//---------------------------------------------------------------------------
CRgbColor& CRgbColor::operator = ( const CRgbColor  &color )
{
  mRed = color.mRed;
  mGreen = color.mGreen;
  mBlue = color.mBlue;

  return *this;
}
//---------------------------------------------------------------------------
bool CRgbColor::operator == ( const CRgbColor color )
{
  if ( ( mRed == color.mRed ) &&
       ( mGreen == color.mGreen ) &&
       ( mBlue == color.mBlue ) )
    return true;

  return false;
}
//---------------------------------------------------------------------------


} // namespace

