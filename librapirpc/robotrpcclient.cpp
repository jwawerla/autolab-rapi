#include "robotrpcclient.h"

namespace Rapi
{
using namespace jsonrpc;

//------------------------------------------------------------------------------
RobotRpcClient::RobotRpcClient(std::string host, int port)
  : mServer( host, port )
{
}
//------------------------------------------------------------------------------
RobotRpcClient::~RobotRpcClient()
{
}
//------------------------------------------------------------------------------
object RobotRpcClient::call( std::string methodName, object args )
{
  // issue RPC
  object call;
  call[ "method" ] = toVariant( methodName );
  call[ "params" ] = toVariant( args );
  mServer.call( call );

  // wait for a response
  variant response;
  while( true ) {
    if( mServer.recv( response ) )
      break;
  }
  // unpack response
  object result;
  try {
    object temp = fromVariant< object >( response );
    result = fromVariant< object >( temp[ "result" ] );
   } catch( ... ) {
    std::cerr << "Problem unpacking returned RPC data" << std::endl;
    return result;
  }

  return result;
}
//------------------------------------------------------------------------------
Rapi::CVelocity2d RobotRpcClient::unpackVelocity( variant velocityVar )
{
  Rapi::CVelocity2d velocity;
  object velocityObj = fromVariant<object>( velocityVar );
  velocity.mXDot = fromVariant<double>( velocityObj [ "xDot" ] );
  velocity.mYDot = fromVariant<double>( velocityObj[ "yDot" ] );
  velocity.mYawDot = fromVariant<double>( velocityObj[ "yawDot" ] );
  return velocity;
}
//------------------------------------------------------------------------------
Rapi::CPose2d RobotRpcClient::unpackPose( variant poseVar )
{
  Rapi::CPose2d pose;
  object poseObj = fromVariant<object>( poseVar );
  pose.mX = fromVariant<double>( poseObj[ "x" ] );
  pose.mY = fromVariant<double>( poseObj[ "y" ] );
  pose.mYaw = fromVariant<double>( poseObj[ "yaw" ] );
  return pose;
}
//------------------------------------------------------------------------------
bool RobotRpcClient::getDrivetrainDev()
{
  object result = call( "getDrivetrainDev", object() );
  // TODO: checking here
  return true;
}
//------------------------------------------------------------------------------
bool RobotRpcClient::getPowerPackDev( double &maxBatteryCapacity )
{
  object result = call( "getPowerPackDev", object() );
  // TODO: checking here
  maxBatteryCapacity = fromVariant<double>( result[ "maxBatteryCapacity" ] );
  return true;
}
//------------------------------------------------------------------------------
bool RobotRpcClient::getRangeFinderDev( unsigned int &numSamples,
                                        float &minRange,
                                        float &maxRange,
                                        float &beamConeAngle,
                                        std::vector<Rapi::CPose2d> &beamPose )
{
  object result = call( "getRangeFinderDev", object() );
  // TODO: checking here
  numSamples = fromVariant<int>( result[ "numSamples" ] );
  minRange = fromVariant<double>( result[ "minRange" ] );
  maxRange = fromVariant<double>( result[ "maxRange" ] );
  beamConeAngle = fromVariant<double>( result[ "beamConeAngle" ] );
  array beamPoses = fromVariant<array>( result[ "beamPose" ] );
  for( unsigned int i = 0; i < numSamples; ++i ) {
    beamPose.push_back( unpackPose( beamPoses[i] ) );
  }

  return true;
}
//------------------------------------------------------------------------------
bool RobotRpcClient::getBumperDev(unsigned int& numSamples)
{
    object result = call("getBumperDev", object());
    numSamples = fromVariant<int>(result["numSamples"]);
    return true;
}
//------------------------------------------------------------------------------
bool RobotRpcClient::getWheelDropDev(unsigned int& numSamples)
{
    object result = call("getWheelDropDev", object());
    numSamples = fromVariant<int>(result["numSamples"]);
    return true;
}
//------------------------------------------------------------------------------
bool RobotRpcClient::getCliffDev(unsigned int& numSamples)
{
    object result = call("getCliffDev", object());
    numSamples = fromVariant<int>(result["numSamples"]);
    return true;
}

//------------------------------------------------------------------------------
bool RobotRpcClient::getPhotoDev(unsigned int& numSamples, unsigned int& maxRange)
{
    object result = call("getPhotoDev", object());
    numSamples = fromVariant<int>(result["numSamples"]);
    numSamples = fromVariant<int>(result["maxRange"]);
    return true;
}

//------------------------------------------------------------------------------
void RobotRpcClient::getDrivetrain( bool &isStalled,
                                    float &stalledSince,
                                    Rapi::CVelocity2d & measVelocity,
                                    Rapi::CVelocity2d & cmdVelocity,
                                    Rapi::CPose2d & odometry )
{
  object result = call( "getDrivetrain", object() );
  isStalled = fromVariant<bool>( result[ "isStalled" ] );
  stalledSince = (float) fromVariant<double>( result[ "stalledSince" ] );
  measVelocity = unpackVelocity( result[ "measVelocity" ] );
  cmdVelocity = unpackVelocity( result[ "cmdVelocity" ] );
  odometry = unpackPose( result[ "odometry" ] );
}
//------------------------------------------------------------------------------
void RobotRpcClient::getPowerPack( double &batteryCapacity, double &current,
                                   double &voltage, double &batteryTemperature,
                                   double &batteryLevel,
                                   double &totalEnergyDissipated,
                                   Rapi::tChargeState &chargeState,
                                   int &chargeSource )
{
  object result = call( "getPowerPack", object() );
  batteryCapacity = fromVariant<double>( result[ "batteryCapacity" ] );
  current = fromVariant<double>( result[ "current" ] );
  voltage = fromVariant<double>( result[ "voltage" ] );
  batteryTemperature = fromVariant<double>( result[ "batteryTemperature" ] );
  batteryLevel = fromVariant<double>( result[ "batteryLevel" ] );
  totalEnergyDissipated = fromVariant<double>
                            ( result[ "totalEnergyDissipated" ] );
  chargeState = (Rapi::tChargeState) fromVariant<int>( result[ "chargeState" ] );
  chargeSource = fromVariant<int>( result[ "chargeSource" ] );
}
//------------------------------------------------------------------------------
std::vector<float> RobotRpcClient::getRanges( void )
{
  std::vector<float> out;
  object result = call( "getRanges", object() );
  array ranges = fromVariant< array >( result[ "range" ] );
  for( unsigned int i = 0; i < ranges.size(); ++i ) {
    out.push_back( fromVariant< double >( ranges[i]) );
  }

  return out;
}
//------------------------------------------------------------------------------
std::vector<bool> RobotRpcClient::getBumpers( void )
{
    std::vector<bool> out;
    object result = call("getBumpers", object() );
    array bumpers = fromVariant<array>(result["bumpers"]);
    for (unsigned int i = 0; i < bumpers.size(); ++i)
    {
        out.push_back(fromVariant<bool> (bumpers[i]));
    }
    return out;
}
//------------------------------------------------------------------------------
std::vector<bool> RobotRpcClient::getWheelDrops( void )
{
    std::vector<bool> out;
    object result = call("getWheelDrops", object() );
    array wheeldrops = fromVariant<array>(result["wheelDrops"]);
    for (unsigned int i = 0; i < wheeldrops.size(); ++i)
    {
        out.push_back(fromVariant<bool> (wheeldrops[i]));
    }
    return out;
}
//------------------------------------------------------------------------------
std::vector<bool> RobotRpcClient::getCliffs( void )
{
    std::vector<bool> out;
    object result = call("getCliffs", object() );
    array cliffs = fromVariant<array>(result["cliffs"]);
    for (unsigned int i = 0; i < cliffs.size(); ++i)
    {
        out.push_back(fromVariant<bool> (cliffs[i]));
    }
    return out;
}
//------------------------------------------------------------------------------
std::vector<float> RobotRpcClient::getPhotos( void )
{
    std::vector<float> out;
    object result = call("getPhotos", object() );
    array photos = fromVariant<array>(result["photos"]);
    for (unsigned int i = 0; i < photos.size(); ++i)
    {
        out.push_back(fromVariant<float> (photos[i]));
    }
    return out;
}
//------------------------------------------------------------------------------

} // namespace
