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

#ifndef RAPIBOOLEANSTATEVARIABLE_H
#define RAPIBOOLEANSTATEVARIABLE_H

#include "statevariable.h"

namespace Rapi {

/**
 * A boolean state variable that keeps track of changes over time
 * @author Jens Wawerla
 */
class CBooleanStateVariable : public CStateVariable
{
  public:
    /**
     * Default constructor
     */
    CBooleanStateVariable();
    /** Default destructor */
    virtual ~CBooleanStateVariable();
    /** Overloaded = operator */
    bool operator= ( const bool b );
    /** Overloaded != operator */
    bool operator== ( const bool b );
    /** Overloaded != operator */
    bool operator!= ( const bool b );
    /**
     * Called by the frame work to update the state variable
     * @param dt time since last call [s]
     */
    virtual void updateData(float dt);

  protected:
    /** Actual variable */
    bool mValue;

};

} // namespace

#endif
