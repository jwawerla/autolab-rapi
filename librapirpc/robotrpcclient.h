#ifndef ROBOTRPCCLIENT_H
#define ROBOTRPCCLIENT_H

#include <string>
#include <vector>
#include <pose2d.h>
#include <velocity2d.h>
#include <powerpack.h>  // tChargeState
#include <jsonrpc_server.h>

namespace Rapi
{
/**
 * @author Ash Charles <jac27@sfu.ca>
 * @author Mani Monajjemi <mmonajje@sfu.ca>
 */
class RobotRpcClient
{
  public:
    /**
     * Connect to a Robot RPC Server at the specified host and port
     * @param host
     * @param port
     */
    RobotRpcClient ( std::string host, int port );
    /** Default destructor: close connection */
    ~RobotRpcClient();
  //---- device configuration calls --------------------------------------------
    /**
     * Get the parameters of a drivetrain device if one exists
     * @return 1 if successful, otherwise 0
     */
    bool getDrivetrainDev();
    /**
     * Get the parameters of a power pack if one exists
     * @param maxBatteryCapacity
     * @param maxCurrent
     * @return 1 if successful, otherwise 0
     */
    bool getPowerPackDev( double &maxBatteryCapacity );
    /**
     * Get the parameters of a range finder device if one exists
     * @param numSamples
     * @param minRange
     * @param maxRange
     * @param beamConeAngle
     * @param beamPose
     * @return 1 if successful, otherwise 0
     */
    bool getRangeFinderDev( unsigned int &numSamples,
                            float &minRange,
                            float &maxRange,
                            float &beamConeAngle,
                             std::vector<CPose2d> &beamPose );
    
    /**
     * Get the parameters of bumper device if exists
     * @param numSamples
     * @return 1 if successful, otherwise 0
     */
    bool getBumperDev( unsigned int &numSamples);
    
    /**
     * Get the parameters of wheeldrop device if exists
     * @param numSamples
     * @return 1 if successful, otherwise 0
     */
    bool getWheelDropDev( unsigned int &numSamples);
    
    /**
     * Get the parameters of cliff device if exists
     * @param numSamples
     * @return 1 if successful, otherwise 0
     */
    bool getCliffDev( unsigned int &numSamples);
    
  //---- device get/set calls --------------------------------------------------
    /** get position and velocity information */
    void getDrivetrain( bool &isStalled,
                        float &stalledSince,
                        CVelocity2d & measVelocity,
                        CVelocity2d & cmdVelocity,
                        CPose2d & odometry );
    /** get voltage and power information */
    void getPowerPack( double &batteryCapacity, double &current,double &voltage,
                       double &batteryTemperature, double &batteryLevel,
                       double &totalEnergyDissipated,
                       tChargeState &chargeState,
                       int &chargeSource );
    /** 
     * Get range data from a range finder device
     * @return vector of ranges
     */
    std::vector<float> getRanges ( void );
    
    /**
     * Get bumpers status
     * @return vector of booleans (status)
     */
    std::vector<bool> getBumpers(void);
    
    
    /**
     * Get wheeldrops status
     * @return vector of booleans (status)
     */
    std::vector<bool> getWheelDrops(void);

    /**
     * Get cliffs status
     * @return 
     */
    std::vector<bool> getCliffs(void);
    
  private:
    /** helper method for performing RPC calls */
    jsonrpc::object call( std::string methodName, jsonrpc::object args );
    /** utility routine to unpack a CPose2d object from a variant */
    CPose2d unpackPose( jsonrpc::variant poseObj );
    /** utility routine to unpack a CVelocity2d object from a variant */
    CVelocity2d unpackVelocity( jsonrpc::variant velocityObj );
    /** the json server to which we make calls */
    jsonrpc::TCPClient mServer;
};

} // namespace
#endif
