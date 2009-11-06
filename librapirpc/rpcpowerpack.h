#ifndef RPCPOWERPACK_H
#define RPCPOWERPACK_H


#include "robotrpcclient.h"
#include "powerpack.h"
#include "cbpowerpack.h" // we need tChargeSource


namespace Rapi
{
/**
 * RPC implementation of a powerpack
 * @author Ash Charles (jac27@sfu.ca)
 */
class RpcPowerPack : public APowerPack
{
  public:
    /** Default constructor
     * @param client server for remote instance of robot
     * @param devName name of device
     */
    RpcPowerPack( RobotRpcClient* client, std::string devName );
    /** Default destructor */
    ~RpcPowerPack();
    /**
     * Initializes the device
     * @param return 1 if success 0 otherwise
     */
    int init();
    /** enable/disable the device */
    void setEnabled( bool enable);
    /**
     * Checks if the robot is charging or not
     * @return true if charging, false otherwise
     */
    bool isCharging();
    /**
     * Gets the source type from which we are charging
     * @return charge source type
     */
    int getChargingSource();
    /**
     * Gets the state of the charging process
     * @return state
     */
    tChargeState getChargingState();
    /**
     * This method gets called by the framework every step to update
     * the sensor data
     * @param dt size of time step [s]
     */
    virtual void updateData( const double dt);

  private:
    /** RPC server */
    RobotRpcClient* mClient;
    /** the last transmitted charging state */
    tChargeState mChargeState;
    /** arbitrary-type charging sources */
    int mChargeSource;
    /** battery level to replace getBatteryLevel() call */
    double mBatteryLevel;
};

} // namespace

#endif
