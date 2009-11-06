#ifndef RPCROBOT_H
#define RPCROBOT_H

#include <string>
#include "robot.h"
#include "robotctrl.h"
#include "printerror.h"
#include "robotrpcclient.h"
#include "rpcrangefinder.h"
#include "rpcdrivetrain.h"
#include "rpcpowerpack.h"

namespace Rapi
{

/**
 * This robot gets devices for a remote robot
 * @author Ash Charles <jac27@sfu.ca>
 */
class RpcRobot : public ARobot
{
  public:
    /** Default constructor */
    RpcRobot( std::string host, int port );
    /** Default destructor */
    virtual ~RpcRobot();
    /**
     * Initialize remote robot.
     * @return 1 if success, 0 otherwise
     */
    virtual int init();
    /**
     * Gets the current time of the robot.
     * @return [s]
     */
    virtual double getCurrentTime() const;
    /**
     * This is the main of the thread
     */
    void run();
    /**
     * Terminates the execution of the main thread
     */
    void quit();
    /**
     * Checks if the main thread is running or not
     * @return true if running, false otherwise
     */
    bool isRunning() const { return mFgRunning; };
    void update();
    /**
     * Gets a device with a given device index
     * @param devName name of device
     * @return device
     * @return 1 if successful, 0 otherwise
     */
    virtual int findDevice( ARangeFinder* &device, std::string devName );

    // TODO: maybe we should implement these
    virtual int findDevice ( ADrivetrain2dof* &device, std::string devName );
    virtual int findDevice ( APowerPack* &device, std::string devName );
    virtual int findDevice ( AFiducialFinder* &device, std::string devName );
    virtual int findDevice ( ALights* &device, std::string devName );
    virtual int findDevice ( ATextDisplay* &device, std::string devName );
    virtual int findDevice ( ABinarySensorArray* &device, std::string devName );
    virtual int findDevice ( ASwitchArray* &device, std::string devName );
    virtual int findDevice ( AAnalogSensorArray* &device, std::string devName );
    virtual int findDevice ( ABlobFinder* &device, std::string devName );

  protected:
    /**
     * This method sleeps for interval seconds since the last call
     * @param interval [s]
     */
    void synchronize( double interval ); 
    /** Network RPC service */
    RobotRpcClient mClient;
    /** Flag if main loop is running or not */
    bool mFgRunning;
  private:
    /** Infrared sensors */
    RpcRangeFinder* mRangeFinder;
    /** drivetrain */
    RpcDrivetrain2dof* mDrivetrain;
    /** power pack */
    RpcPowerPack* mPowerPack;
    /** Last time synchronize was called */
    double mLastSynchronizeTime;
};

} // namespace

#endif
