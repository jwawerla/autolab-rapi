/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Ash Charles (jac27@sfu.ca)                                     *
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
#ifndef ROBOTRPCSERVER_H
#define ROBOTRPCSERVER_H

#include "robot.h"
#include "rangefinder.h"
#include "drivetrain2dof.h"
#include "powerpack.h"
#include "binarysensorarray.h"
#include "pose2d.h"
#include "velocity2d.h"
#include "printerror.h"
#include "jsonrpc_server.h"
#include <pthread.h>

namespace Rapi
{
/**
 *  @author Ash Charles <jac27@sfu.ca>
 *  @author Mani Monajjemi <mmmonajje@sfu.ca>
 */
class RobotRpcServer
{
  public:
    /**
     * Constructs a JSON RPC server to export the robot data.  At the moment,
     * drivetrain, rangefinder, powerpack and bumper data is exported.  Note that the
     * server runs in a separate thread from the robot controller so access to
     * robot data should be controller.  Typically, you create a pthread mutex
     * when you initialize your controller and lock it during your updateData()
     * function.
     * @param robot pointer to the robot to be served
     * @param port server launched a host ip on specified port
     * @param dataMutex a mutex that protects access to robot data
     * @param
     */
    RobotRpcServer( ARobot * robot, int port, pthread_mutex_t * dataMutex,
                    ADrivetrain2dof * drivetrain, APowerPack * powerpack,
                    ARangeFinder * rangefinder, ABinarySensorArray* bumper, ABinarySensorArray *wheeldrop );
    /** default destructor */
    ~RobotRpcServer();
    /** start the server in its own thread */
    void start();
    /** A little thunk to allow pthread to run within the class */
    static void * runThread( void * arg )
    { return (( RobotRpcServer * ) arg )->runServer(); }
  private:
    /** pthread function: listen for incoming requests */
    void * runServer( void );
    //---- device configuration calls ------------------------------------------
    /** sends the current drivetrain configuration */
    void getDrivetrainDev( jsonrpc::variant params,
                           jsonrpc::object& results,
                           const std::string& ip,
                           int port );
    /** send the current powerpack configuration */
    void getPowerPackDev( jsonrpc::variant params,
                          jsonrpc::object& results,
                          const std::string& ip,
                          int port );
    /** send the current rangefinder configuration */
    void getRangeFinderDev( jsonrpc::variant params,
                            jsonrpc::object& results,
                            const std::string& ip,
                            int port );
    
    /** Send the current bumper configuration */
    void getBumperDev(jsonrpc::variant params,
                                    jsonrpc::object& results,
                                    const std::string& ip,
                                    int port);
    
    /** Send the current wheeldrop configuration */
    void getWheelDropDev(jsonrpc::variant params,
                                    jsonrpc::object& results,
                                    const std::string& ip,
                                    int port);
    //---- device get/set calls ------------------------------------------------
    /** send position and velocity information */
    void getDrivetrain( jsonrpc::variant params,
                        jsonrpc::object& results,
                        const std::string& ip,
                        int port );
    /** send voltage and power information */
    void getPowerPack( jsonrpc::variant params,
                       jsonrpc::object& results,
                       const std::string& ip,
                       int port );
    /** send a rangefinder data */
    void getRanges( jsonrpc::variant params,
                    jsonrpc::object& results,
                    const std::string& ip,
                    int port );
    /** send bumpers data */
    void getBumpers(jsonrpc::variant params,
                    jsonrpc::object& results,
                    const std::string& ip,
                    int port);
    
     /** send wheeldrop data */
    void getWheelDrops(jsonrpc::variant params,
                    jsonrpc::object& results,
                    const std::string& ip,
                    int port);
    
    /** utility routine to pack a CVelocity2d object into a jsonrpc::variant */
    jsonrpc::variant packVelocity( CVelocity2d velocity );
    /** utility routine to pack a CPose2d object into a jsonrpc::variant */
    jsonrpc::variant packPose( CPose2d pose );
    /** json server */
    jsonrpc::TCPServer mServer;
    /** thread for server */
    pthread_t mServerThread;
    /** mutex to access the robot's data */
    pthread_mutex_t * mRobotMutex;
    /** a pointer to the robot object we are serving */
    ARobot * mRobot;
    /** the drivetrain object we are serving (if it exists) */
    ADrivetrain2dof * mDrivetrain;
    /** the powerpack object we are serving (if it exists) */
    APowerPack * mPowerPack;
    /** the rangefinder object we are serving (if it exists) */
    ARangeFinder * mRangeFinder;
    /** the bumper object we are serving (if it exists) */
    ABinarySensorArray *mBumper;
    /** the wheel drop object we are serving (if it exists) */
    ABinarySensorArray *mWheelDrop;
};

} // namespace
#endif // ROBOTRPCSERVER_H
