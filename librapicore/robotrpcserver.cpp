#include "robotrpcserver.h"

namespace Rapi
{
using namespace jsonrpc;
//------------------------------------------------------------------------------
RobotRpcServer::RobotRpcServer( ARobot * robot, int port,
                                ADrivetrain2dof * drivetrain,
                                APowerPack * powerpack,
                                ARangeFinder * rangefinder , ABinarySensorArray *bumper, ABinarySensorArray *wheeldrop,
                                ABinarySensorArray *cliff, AAnalogSensorArray *photo)
    : mServer( port )
{
  // initialize data members
  mRobot = robot;
  mDrivetrain = drivetrain;
  mPowerPack = powerpack;
  mRangeFinder = rangefinder;
  mBumper = bumper;
  mWheelDrop = wheeldrop;
  mCliff = cliff;
  mPhoto = photo;
  
  //initialize the lock
  pthread_mutex_init(&mRobotMutex, NULL);

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
  
  if (mBumper)
  {
      mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getBumperDev),
                                "getBumperDev");
      mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getBumpers),
                                "getBumpers");
  }
  
  if (mWheelDrop)
  {
       mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getWheelDropDev),
                                "getWheelDropDev");
      mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getWheelDrops),
                                "getWheelDrops");
  }
  
  if (mCliff)
  {
       mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getCliffDev),
                                "getCliffDev");
      mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getCliffs),
                                "getCliffs");
  }
  
  if (mPhoto)
  {
       mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getPhotoDev),
                                "getPhotoDev");
      mServer.addMethodHandler(new Server::RPCMethod<RobotRpcServer>
                                (this, &RobotRpcServer::getPhotos),
                                "getPhotos");
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
void RobotRpcServer::lockRpcMutex()
{
    pthread_mutex_lock(&mRobotMutex);
}
//------------------------------------------------------------------------------
void RobotRpcServer::unlockRpcMutex()
{
    pthread_mutex_unlock(&mRobotMutex);
}
//------------------------------------------------------------------------------
void RobotRpcServer::start()
{
  pthread_create( &mServerThread, NULL, &runThread, this );
}
//------------------------------------------------------------------------------
void * RobotRpcServer::runServer( void )
{
  //TODO: Should we add a delay in the loop?
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
    lockRpcMutex();
  double cap = ( mPowerPack->getMaxBatteryCapacity() == INFINITY ) ? 0.0 :
               mPowerPack->getMaxBatteryCapacity();
  unlockRpcMutex();
  results[ "maxBatteryCapacity" ] = toVariant<double> ( cap );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getRangeFinderDev( variant params,
                                        object& results,
                                        const std::string& ip,
                                        int port )
{
  lockRpcMutex();
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
  unlockRpcMutex();
  results[ "beamPose" ] = toVariant<array> ( beamPose );
}
//------------------------------------------------------------------------------
void RobotRpcServer::getBumperDev(jsonrpc::variant params,
                                    jsonrpc::object& results,
                                    const std::string& ip,
                                    int port)
{
    lockRpcMutex();
    results["numSamples"] = toVariant<int> (mBumper->getNumSamples());    
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getWheelDropDev(jsonrpc::variant params,
                                    jsonrpc::object& results,
                                    const std::string& ip,
                                    int port)
{
    lockRpcMutex();
    results["numSamples"] = toVariant<int> (mWheelDrop->getNumSamples());    
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getCliffDev(jsonrpc::variant params,
                                    jsonrpc::object& results,
                                    const std::string& ip,
                                    int port)
{
    lockRpcMutex();
    results["numSamples"] = toVariant<int> (mCliff->getNumSamples());    
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getPhotoDev(jsonrpc::variant params,
                                    jsonrpc::object& results,
                                    const std::string& ip,
                                    int port)
{
    lockRpcMutex();
    results["numSamples"] = toVariant<int> (mPhoto->getNumSamples());   
    // From Chatterbox Driver File
    results["maxRange"] = toVariant<double> (1024); 
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getDrivetrain( variant params,
                                    object& results,
                                    const std::string& ip,
                                    int port )
{
  lockRpcMutex();
  results[ "isStalled" ] = toVariant<bool> ( mDrivetrain->isStalled() );
  results[ "stalledSince" ] = toVariant<double> ( mDrivetrain->stalledSince() );
  results[ "measVelocity" ] = packVelocity( mDrivetrain->getVelocity() );
  results[ "cmdVelocity" ] = packVelocity( mDrivetrain->getVelocityCmd() );
  results[ "odometry" ] = packPose( mDrivetrain->getOdometry()->getPose() );
  unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getPowerPack( variant params,
                                   object& results,
                                   const std::string& ip,
                                   int port )
{
   lockRpcMutex();
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
  unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getRanges( variant params,
                                object& results,
                                const std::string& ip,
                                int port )
{
    lockRpcMutex();
  array ranges;
  for ( unsigned int i = 0; i < mRangeFinder->getNumSamples(); ++i ) {
    ranges.push_back( toVariant<double> ( mRangeFinder->mRangeData[i].range ) );
  }
  results[ "range" ] = toVariant( ranges );
  unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getBumpers(jsonrpc::variant params,
                                        jsonrpc::object& results, 
                                        const std::string& ip, int port)
{
    lockRpcMutex();
    array bumpers;
    for (unsigned int i = 0; i < mBumper->getNumSamples(); ++i)
    {
        bumpers.push_back(toVariant<bool> (mBumper->mBitData[i]) );
    }
    results["bumpers"] = toVariant( bumpers );
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getWheelDrops(jsonrpc::variant params,
                                        jsonrpc::object& results, 
                                        const std::string& ip, int port)
{
    lockRpcMutex();
    array wheeldrops;
    for (unsigned int i = 0; i < mWheelDrop->getNumSamples(); ++i)
    {
        wheeldrops.push_back(toVariant<bool> (mWheelDrop->mBitData[i]) );
    }
    results["wheelDrops"] = toVariant( wheeldrops );
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getCliffs(jsonrpc::variant params,
                                        jsonrpc::object& results, 
                                        const std::string& ip, int port)
{
    lockRpcMutex();
    array cliffs;
    for (unsigned int i = 0; i < mCliff->getNumSamples(); ++i)
    {
        cliffs.push_back(toVariant<bool> (mCliff->mBitData[i]) );
    }
    results["cliffs"] = toVariant( cliffs );
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
void RobotRpcServer::getPhotos(jsonrpc::variant params,
                                        jsonrpc::object& results, 
                                        const std::string& ip, int port)
{
    lockRpcMutex();
    array photos;
    for (unsigned int i = 0; i < mPhoto->getNumSamples(); ++i)
    {
        photos.push_back(toVariant<double> (mPhoto->mData[i]) );
    }
    results["photos"] = toVariant( photos );
    unlockRpcMutex();
}
//------------------------------------------------------------------------------
} // namespace
