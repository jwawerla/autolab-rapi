#include "rpcdrivetrain.h"

namespace Rapi
{
//------------------------------------------------------------------------------
/** Dummy class for odometry objects */
class RpcOdometry : public COdometry
{
  public:
    RpcOdometry( RobotRpcClient* client, std::string devName)
      : COdometry ( devName )
    {
      mClient = client;
      setToZero();
      setEnabled ( true );
    }
    ~RpcOdometry() {};
    void updateData( const double dt ) {};
  private:
    RobotRpcClient* mClient;
};
//------------------------------------------------------------------------------
RpcDrivetrain2dof::RpcDrivetrain2dof( RobotRpcClient* client, std::string devName )
    : ADrivetrain2dof( devName )
{
  mClient = client;
  if( mClient->getDrivetrainDev() == false ) {
    std::cerr << "Can't initialize drivetrain object" << std::endl;
    return;
  }
  mOdometry = new RpcOdometry( mClient, "RPC:Odometry" );
  assert( mOdometry );
  mFgStalled = false;
  mStalledTimer = 0.0;
  mClient->getDrivetrain( mFgStalled, mStalledTimer, mVelocityMeas,
                          mVelocityUserCmd, mPose );
  mOdometry->setPose( mPose );
  setEnabled ( true );
}
//-----------------------------------------------------------------------------
RpcDrivetrain2dof::~RpcDrivetrain2dof()
{
  if( mOdometry )
    delete mOdometry;
}
//-----------------------------------------------------------------------------
int RpcDrivetrain2dof::init()
{
  return 1;
}
//-----------------------------------------------------------------------------
void RpcDrivetrain2dof::setEnabled( bool enable)
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void RpcDrivetrain2dof::updateData( const double dt)
{
  mClient->getDrivetrain( mFgStalled, mStalledTimer, mVelocityMeas,
                          mVelocityUserCmd, mPose );
  mOdometry->setPose( mPose );
}
//---------------------------------------------------------------------------
void RpcDrivetrain2dof::print() const
{
  printf ( "RpcDrivetrain2dof: " );
  printf ( "\n" );
}
//-----------------------------------------------------------------------------

} // namespace
