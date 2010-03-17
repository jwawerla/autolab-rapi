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
#ifndef RAPIDATALOGGER_H
#define RAPIDATALOGGER_H

#include "rapitypesystem.h"
#include "rapitypes.h"
#include <stdio.h>

namespace Rapi
{

/**
 * This class provides a means to log data in a comma separated value (CSV)
 * file. Use addVar to register variables with this class and then call write
 * to write an entry to file. The first entry will be the header line, naming
 * entries.
 * @author Jens Wawerla
 */
class CDataLogger : public CRapiTypeSystem
{
  public:
    /** Default destructor */
    ~CDataLogger();
    /**
     * Gets the only instance of this class
     * @param filename of log file
     * @param overwrite overwrite or append a possible existing log file
     * @param commentSymbole symbole for a comment line, e.g. % for Matlab
     * @return instance of this class
     */
    static CDataLogger* getInstance( std::string filename, tWriteMode overwrite = APPEND,
                                     std::string commentSymbole = "%" );
    /**
     * Gets the filename of the log file
     * @return filename
     */
    std::string getFilename() const { return mFilename; };
    /**
     * Writes an entry to the log file with a given time stamp
     * @param timestamp of entry [s]
     */
    void write( const float timestamp );
    /**
     * Writes a header line with the names of the variables to the log file
     */
    void writeHeader() const;
    /**
     * Sets the logging interval. At least dt seconds has to pass between two
     * write() calls to be written to file
     * @param dt [s]
     */
    void setInterval( float dt );

  protected:
    /**
     * Default constructor
     * @param filename of log file
     * @param overwrite  overwrite or append a possible existing log file
     * @param commentSymbole symbole for a comment line, e.g. % for Matlab
     */
    CDataLogger( std::string filename, tWriteMode overwrite, std::string commentSymbole );
    /** File name of log file */
    std::string mFilename;
    /** Symbole for a comment line, e.g. % for Matlab */
    std::string mCommentSymbole;
    /** File pointer */
    FILE* mFp;
    /** Flags if the header has been write or not */
    bool mFgHeader;
    /** Interval of write call to actual file entries [s] */
    float mLogInterval;
    /** Time stamp of the last write to file [s] */
    float mLastWriteTimestamp;
};

} // namespace

#endif
