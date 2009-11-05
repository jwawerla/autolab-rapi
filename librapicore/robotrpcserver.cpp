#include "robotrpcserver.h"

namespace Rapi
{
using namespace jsonrpc;
//------------------------------------------------------------------------------
RobotRpcServer::RobotRpcServer( ARobot * robot, int port,
                                pthread_mutex_t * dataMutex,
                                ADrivetrain2dof * drivetrain,
                                APowerPack * powerpack,
                                ARangeFinder * rangefinder )
    : mServer( port )
{
  // initialize data members
  mRobot = robot;
  mRobotMutex = dataMutex;
  mDrivetrain = drivetrain;
  mPowerPack = powerpack;
  mRangeFinder = rangefinder;

  // setup handlers as appropriate
  if ( mDrivetrain ) {
    mServer.addMethodHandler( new Server::RPCMethod< RobotRpcServer >
                              ( this, &RobotRpcServer::getDrivetrainDev ),
                              "getDrivetrainDev" );
    mServer.addMethodHandler( new Server::RPCMethod< RobotRpcServer >
                              ( this, &RobotRpcServer::getDrivetrain ),
                              "getDrivetrain" );
  }
  if ( mPowerPack ) {
    mServer.addMethodHandler( new Server::RPCMethod< RobotRpcServer >
                              ( this, &RobotRpcServer::getPowerPackDev ),
                              "getPowerPackDev" );
    mServer.addMethodHandler( new Server::RPCMethod< RobotRpcServer >
                              ( this, &RobotRpcServer::getPowerPack ),
                              "getPowerPack" );
  }
  if ( mRangeFinder ) {
    mServer.addMethodHandler( new Server::RPCMethod< RobotRpcServer >
                              ( this, &RobotRpcServer::getRangeFinderDev ),
                              "getRangeFinderDev" );
    mServer.addMethodHandler( new Server::RPCMethod< RobotRpcServer >
                              ( this, &RobotRpcServer::getRanges ),
                              "getRanges" );
  }
}
//------------------------------------------------------------------------------
RobotRpcServer::~RobotRpcServer()
{
  if ( pthread_cancel( mServerThread ) != 0 ) {
    PRT_ERR0( "RobotRpcServer destructor: couldn't kill server thread!\n \
              Perhaps you didn't start it?  Otherwise...we got a prob\n" );
  }
}
//------------------------------------------------------------------------------
void RobotRpcServer::start()
{
  pthread_create( &mServerThread, NULL, &runThread, this );
}
//------------------------------------------------------------------------------
void * RobotRpcServer::runServer( void )
{
  while ( true ) { mServer.recv(); }
  return 0; // we shouldn't hit this line
}
//------------------------------------------------------------------------------
variant RobotRpcServer::packVelocity( CVelocity2d velocity )
{
  object velocityObj;
  velocityObj[ "xDot" ] = toVariant<double> ( velocity.mXDot );
  velocityObj[ "yDot" ] = toVariant<double> ( velocity.mYDot );
  velocityObj[ "yawDot" ] = toVariant<double> ( velocity.mYawDot );
  return toVariant( velocityObj );
}
//------------------------------------------------------------------------------
variant RobotRpcServer::packPose( CPose2d pose )
{
  object poseObj;
  poseObj[ "x" ] = toVariant<double> ( pose.mX );
  poseObj[ "y" ] = toVariant<double> ( pose.mY );
  poseObj[ "yaw" ] = toVariant<double> ( pose.mYaw );
  return toVariant( poseObj );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getDrivetrainDev( variant params,
                                       object& results,
                                       const std::string& ip,
                                       int port )
{
  // This method would fetch drivetrain parameters but there aren't any at the
  // moment so it is not needed
}
//------------------------------------------------------------------------------
void RobotRpcServer::getPowerPackDev( variant params,
                                      object& results,
                                      const std::string& ip,
                                      int port )
{
  pthread_mutex_lock( mRobotMutex );
  double cap = ( mPowerPack->getMaxBatteryCapacity() == INFINITY ) ? 0.0 :
               mPowerPack->getMaxBatteryCapacity();
  pthread_mutex_unlock( mRobotMutex );
  results[ "maxBatteryCapacity" ] = toVariant<double> ( cap );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getRangeFinderDev( variant params,
                                        object& results,
                                        const std::string& ip,
                                        int port )
{
  pthread_mutex_lock( mRobotMutex );
  results[ "numSamples" ] = toVariant<int> ( mRangeFinder->getNumSamples() );
  double minRange = ( mRangeFinder->getMinRange() == INFINITY ) ? 0.0 :
                    mRangeFinder->getMinRange();
  results[ "minRange" ] = toVariant<double> ( minRange );
  results[ "maxRange" ] = toVariant<double> ( mRangeFinder->getMaxRange() );
  results[ "beamConeAngle" ] = toVariant<double>
                               ( mRangeFinder->getBeamConeAngle() );

  array beamPose;
  for ( unsigned int i = 0; i < mRangeFinder->getNumSamples(); ++i )
    beamPose.push_back( packPose( mRangeFinder->mRelativeBeamPose[i] ) );
  pthread_mutex_unlock( mRobotMutex );
  results[ "beamPose" ] = toVariant<array> ( beamPose );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getDrivetrain( variant params,
                                    object& results,
                                    const std::string& ip,
                                    int port )
{
  pthread_mutex_lock( mRobotMutex );
  results[ "isStalled" ] = toVariant<bool> ( mDrivetrain->isStalled() );
  results[ "stalledSince" ] = toVariant<double> ( mDrivetrain->stalledSince() );
  results[ "measVelocity" ] = packVelocity( mDrivetrain->getVelocity() );
  results[ "cmdVelocity" ] = packVelocity( mDrivetrain->getVelocityCmd() );
  results[ "odometry" ] = packPose( mDrivetrain->getOdometry()->getPose() );
  pthread_mutex_unlock( mRobotMutex );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getPowerPack( variant params,
                                   object& results,
                                   const std::string& ip,
                                   int port )
{
  pthread_mutex_lock( mRobotMutex );
  results[ "batteryCapacity" ] = toVariant<double>
                                 ( mPowerPack->getBatteryCapacity() );
  results[ "current" ] = toVariant<double> ( mPowerPack->getCurrent() );
  results[ "voltage" ] = toVariant<double> ( mPowerPack->getVoltage() );
  results[ "batteryTemperature" ] = toVariant<double>
                                    ( mPowerPack->getBatteryTemperature() );
  results[ "batteryLevel" ] = toVariant<double>
                              ( mPowerPack->getBatteryLevel() );
  results[ "totalEnergyDissipated" ] = toVariant<double>
                                     ( mPowerPack->getTotalEnergyDissipated() );
  results[ "chargeState" ] = toVariant<int>
                             (( int ) mPowerPack->getChargingState() );
  results[ "chargeSource" ] = toVariant<int>( mPowerPack->getChargingSource() );
  pthread_mutex_unlock( mRobotMutex );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getRanges( variant params,
                                object& results,
                                const std::string& ip,
                                int port )
{
  pthread_mutex_lock( mRobotMutex );
  array ranges;
  for ( unsigned int i = 0; i < mRangeFinder->getNumSamples(); ++i ) {
    ranges.push_back( toVariant<double> ( mRangeFinder->mRangeData[i].range ) );
  }
  results[ "range" ] = toVariant( ranges );
  pthread_mutex_unlock( mRobotMutex );
}
//------------------------------------------------------------------------------

} // namespace
