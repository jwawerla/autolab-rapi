#ifndef RPCDRIVETRAIN_H
#define RPCDRIVETRAIN_H

#include <string>
#include <vector>
#include "robotrpcclient.h"
#include "pose2d.h"
#include "odometry.h"
#include "drivetrain2dof.h"

namespace Rapi
{

/**
 * A rangefinder for use over RPC - specific to chatterbox
 * @author Ash Charles (jac27@sfu.ca)
 */
class RpcDrivetrain2dof : public ADrivetrain2dof
{
  public:
    /** Default constructor */
    RpcDrivetrain2dof( RobotRpcClient* client, std::string devName );
    /** Default destructor */
    virtual ~RpcDrivetrain2dof();
    /**
     * Enables or disables the device
     * @param enable = true to enable, false to disable
     */
    virtual void setEnabled( bool enable );
    /**
     * Initializes the device
     * @param return 1 if success 0 otherwise
     */
    virtual int init();
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt );
    /**
     * Prints the devices main information
     */
    virtual void print() const;
  private:
    RobotRpcClient* mClient;
    CPose2d mPose;
};

} // namespace

#endif
