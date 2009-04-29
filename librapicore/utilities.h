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

#ifndef RAPIUTILITIES_H
#define RAPIUTILITIES_H

#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <limits>
#include <stdlib.h>

namespace Rapi
{

#ifndef INFINITY
  #define INFINITY infinity()
#endif

#ifndef SEC_PER_HOUR
  #define SEC_PER_HOUR 3600.0
#endif

/** Figure out the sign of a number */
#ifndef SIGN
  #define SIGN(x) (((x) == 0) ? 0 : (((x) > 0) ? 1 : -1))
#endif

/** Well... ?! */
#ifndef PI
  #define PI 3.14159265358979323846
#endif

/** pi/2 */
#ifndef HALFPI
  #define HALFPI PI/2.0
#endif

/** 2 times pi = 6.28... */
#ifndef TWOPI
  #define TWOPI 6.28318530717958
#endif

/** Converts from joules to watt hours */
#ifndef JOULE_TO_WATTHOURS
  #define JOULE_TO_WATTHOURS(x) x / 3600.0f
#endif

/**
 * Conversts x from radians to degree
 * @param x value [rad]
 * @return [deg]
 */
#ifndef R2D
  #define R2D(x) x*57.2957795
#endif

/**
 * Calculates euclidian distance
 * @return distance
 */
#ifndef EUCLIDIAN
  #define EUCLIDIAN(x, y, a, b) sqrt( (x-a)*(x-a) + (y-b)*(y-b))
#endif

/**
 * Conversts x from degree to radians
 * @param x value [deg]
 * @return [rad]
 */
#ifndef D2R
  #define D2R(x) x*0.01745329
#endif

/**
 * Maximum value of a and b
 * @param a
 * @param b
 * @return max of a and b
 */
#ifndef MAX
  #define MAX(a,b) ((a > b) ? (a) : (b))
#endif

/**
 * Minimum value of a and b
 * @param a
 * @param b
 * @return min of a and b
 */
#ifndef MIN
  #define MIN(a,b) ((a < b) ? (a) : (b))
#endif

/**
 * Normalizes the value z to be in the interval [-pi, pi]
 * @param z to be normalized
 * @return normalized value
 */
#ifndef NORMALIZE_ANGLE
  #define NORMALIZE_ANGLE(z) atan2(sin(z), cos(z))
#endif

/**
 * Limits the value x to be in the interval [a, b]
 * @param a lower limit
 * @param b upper limit
 * @return limited value
 */
#ifndef LIMIT
  #define LIMIT(x,a,b) MIN(MAX(x,a), b)
#endif

/**
 * Rounds a number
 */
#define ROUND(x) floor(x+0.5)

/**
 * Checks if a x is infinity or not
 * @return true is x is infinity, false otherwise
 */
template<typename T>
inline bool isNan ( T value )
{
  return value != value;
}

/**
 * Checks if a x is nan or not
 * @return true is x is nan, false otherwise
 */
template<typename T>
inline bool isInf ( T value )
{
  return std::numeric_limits<T>::has_infinity &&
         value == std::numeric_limits<T>::infinity();
}

/**
 * Checks if x and y are about equal, that is within epsilon
 * @param x
 * @param y
 * @param epslion
 * @return true if about equal, false otherwise
 */
inline bool epsilonEqual(double x, double y, double epsilon)
{
  if ( fabs ( x - y ) < epsilon )
    return true;

  return false;
}

/**
 * Calculates the square of x
 * @param x
 * @return x^2
 */
template<typename T>
inline T pow2(T x) { return x*x; };

/**
 * Generates a time stamp
 * @return [s]
 */
inline double timeStamp()
{
  struct timeval tv;
  gettimeofday ( &tv, 0 );
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

/**
 * Generates a uniform random number in the interval
 * [minimum, maximum]
 * @param minimum
 * @param maximum
 * @return random number
 */
inline double randNo ( double minimum, double maximum )
{
  double r;

  r = drand48();  // r = [0,1]
  return minimum + r * ( maximum - minimum );
}

/**
 * \fn void initRandomNumberGenerator()
 * Initializes the random number generator
 */
void initRandomNumberGenerator();


//-----------------------------------------------------------------------------

} // namespace
#endif

