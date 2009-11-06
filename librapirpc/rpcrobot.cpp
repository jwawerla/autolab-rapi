#include <sys/time.h>
#include "rpcrobot.h"
namespace Rapi
{
/** Dummy class so the console doesn't crash the GUI */
class RpcRobotCtrl : public ARobotCtrl
{
  public:
    RpcRobotCtrl( ARobot * robot) : ARobotCtrl( robot )
    {
      mRPrintfString = "Not Implemented";
    }
    ~RpcRobotCtrl() {};
    std::string mRPrintfString;
    void updateData(float dt) {};
};
/** Gives us access to variable monitor remotely */
class RpcVariableMonitor : public CVariableMonitor
{
//TODO: Implement - we need to populate the std::vector<tVarEntry> mVarList data
// structure and keep the void* ptr to the data up-to-date.  There is no
// updateData() function so this needs to be done somewhere else.
// Hmm...the mVarList is theoretically dynamic so we should actually look for
// new variables periodically...damn!

  public:
    RpcVariableMonitor( RobotRpcClient* client ) : CVariableMonitor( )
    {
      mClient = client;
    };
    ~RpcVariableMonitor() {};
  private:
    RobotRpcClient* mClient;
};

//-----------------------------------------------------------------------------
RpcRobot::RpcRobot( std::string host, int port )
    : ARobot(), mClient( host, port )
{
  mUpdateInterval = 0.1; // default update frequency is 10Hz
  mName = "RPC Robot";

  mRangeFinder = NULL;
  mDrivetrain = NULL;
  mPowerPack = NULL;
  mLastSynchronizeTime = getCurrentTime();
  mSlowRunThreshold = 10;
  mSlowRunCount = 0;
  // stop console crashing: give a fake controller
  RpcRobotCtrl ctrl( this );
  mRobotCtrl = (IRobotUpdate*) &ctrl;
}
//-----------------------------------------------------------------------------
RpcRobot::~RpcRobot()
{
  mFgRunning = false;
  if ( mRangeFinder )
    delete mRangeFinder;
  if( mDrivetrain )
    delete mDrivetrain;
  if( mPowerPack )
    delete mPowerPack;
}
//-----------------------------------------------------------------------------
int RpcRobot::init()
{
  // need some dummy pointers as we can't cast as well as pass by reference
  ARangeFinder * ranger = NULL;
  ADrivetrain2dof * drive = NULL;
  APowerPack * power = NULL;
  findDevice( ranger, "RPC Range Finder" );
  findDevice( drive, "RPC Drivetrain" );
  findDevice( power, "RPC Power Pack" );
  mFgInitialized = true;
  mFgRunning = true;
  return 1;
}
//-----------------------------------------------------------------------------
double RpcRobot::getCurrentTime() const
{
  double timeNow;

  struct timeval tv;
  gettimeofday( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  return timeNow;
}
//-----------------------------------------------------------------------------
void RpcRobot::run()
{
  if( not mFgInitialized ) {
    std::cerr << "Robot is not initialized, call init() first" << std::endl;
    return;
  }
  // we would like to simply traverse mDeviceList calling updateData for
  // each device but this function is protected for generic ADevice's
  while( mFgRunning ) {
    if( mRangeFinder )
      mRangeFinder->updateData( mUpdateInterval );
    if( mDrivetrain )
      mDrivetrain->updateData( mUpdateInterval );
    if( mPowerPack )
      mPowerPack->updateData( mUpdateInterval );
    synchronize( mUpdateInterval );
  }
}
//-----------------------------------------------------------------------------
void RpcRobot::quit()
{
  mFgRunning = false;
}
//-----------------------------------------------------------------------------
void RpcRobot::update()
{
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ARangeFinder* &device, std::string devName )
{
  if( mRangeFinder == NULL ) {
    mRangeFinder = new RpcRangeFinder( &mClient, devName );
    mDeviceList.push_back( mRangeFinder );
  }
  device = mRangeFinder;

  return 1;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ADrivetrain2dof* &device, std::string devName )
{
  if( mDrivetrain == NULL ) {
    mDrivetrain = new RpcDrivetrain2dof( &mClient, devName );
    mDeviceList.push_back( mDrivetrain );
  }
  device = mDrivetrain;
  return 1;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( APowerPack* &device, std::string devName )
{
  if( mPowerPack == NULL ) {
    mPowerPack = new RpcPowerPack( &mClient, devName );
    mDeviceList.push_back( mPowerPack );
  }
  device = mPowerPack;
  return 1;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( AFiducialFinder* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( AFiducialFinder )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ALights* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( ALights )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ATextDisplay* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( ATextDisplay )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ABinarySensorArray* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( ABinarySensorArray )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ASwitchArray* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( ASwitchArray )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( AAnalogSensorArray* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( AAnalogSensorArray )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
int RpcRobot::findDevice( ABlobFinder* &device, std::string devName )
{
  std::cout << "RPCRobot::findDevice( ABlobFinder )" << std::endl;
  return 0;
}
//-----------------------------------------------------------------------------
void RpcRobot::synchronize( double interval )
{
  double timeNow;
  double duration;

  struct timeval tv;
  gettimeofday( &tv, 0 );
  timeNow = tv.tv_sec + tv.tv_usec * 1e-6;
  duration = timeNow - mLastSynchronizeTime;
  if ( duration < interval ) {
    mSlowRunCount = ( mSlowRunCount > 0 ) ? mSlowRunCount - 1 : 0;
    usleep(( int )(( interval - duration ) * 1e6 ) );
  }
  else {
    mSlowRunCount = ( mSlowRunCount < mSlowRunThreshold ) ? mSlowRunCount + 1 :
                    mSlowRunCount;
    if ( mSlowRunCount >= mSlowRunThreshold ) {
      mSlowRunCount = 0;
      mFgRunningSlowly = true;
      PRT_WARN0( "Control loop running consistently slowly\n" );
    }
    PRT_MSG0( 8, "Control loop ran slowly\n" );
  }
  gettimeofday( &tv, 0 );
  mLastSynchronizeTime = tv.tv_sec + tv.tv_usec * 1e-6;
}
//-----------------------------------------------------------------------------
} // namespace

