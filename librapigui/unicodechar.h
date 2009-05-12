/***************************************************************************
 *   Copyright (C) 2006 by Jens Wawerla                                    *
 *   jwawerla@sfu.ca                                                       *
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
 * $Log: unicodechar.h,v $
 * Revision 1.1  2008/12/04 02:01:06  jwawerla
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2008/02/02 22:19:54  jwawerla
 * new to cvs
 *
 * Revision 1.1.1.1  2008/01/10 19:44:03  jwawerla
 * first time in cvs
 *
 * Revision 1.1  2006/11/07 01:56:38  jwawerla
 * More more on solar cell, need to fix bus current
 *
 * Revision 1.3  2006/11/04 01:19:02  jwawerla
 * Improved solar enery model, started work on solar cell
 *
 *
 *
 **************************************************************************/

#ifndef UNICODECHAR_H
#define UNICODECHAR_H


/** Unicode character code for a degree circle */
const unsigned short UC_DEGREE = 0x02da;

/** Unicode character code for power of two */
const unsigned short UC_SQUARED = 0x00b2;

/** Unicode character code for greek yaw letter */
const unsigned short UC_YAW = 0x03c8;

/** Ready to use qstring of a degree circle */
#define Q_DEGREE QString( QChar(UC_DEGREE) )

/** Ready to use qstring of a power of two symbole */
#define Q_SQUARED QString( QChar(UC_SQUARED) )

/** Ready to use qstring of a yaw angle symbole */
#define Q_YAW QString( QChar(UC_YAW) )

#endif
