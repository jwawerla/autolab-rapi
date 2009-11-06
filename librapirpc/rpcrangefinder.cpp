#include "rpcrangefinder.h"

namespace Rapi
{
//-----------------------------------------------------------------------------
RpcRangeFinder::RpcRangeFinder( RobotRpcClient* client, std::string devName )
    : ARangeFinder( devName )
{
  mClient = client;
  std::vector< CPose2d > beamPose;
  if( mClient->getRangeFinderDev( mNumSamples, mMinRange, mMaxRange,
                               mBeamConeAngle, beamPose) == false ) {
    std::cerr << "Can't initialize rangefinder object" << std::endl;
    return;
  }

  mRangeData = new tRangeData[mNumSamples];
  mRelativeBeamPose = new CPose2d[mNumSamples];

  for( unsigned int i = 0; i < beamPose.size(); ++i ) {
    mRelativeBeamPose[i] = beamPose[i];
    mRangeData[i].range = 0.0;
    mRangeData[i].reflectance = 0.0;
  }

  setEnabled ( true );
}
//-----------------------------------------------------------------------------
RpcRangeFinder::~RpcRangeFinder()
{
 // mRangeData and mRelativeBeamPose are deallocated by ARangeFinder destructor
 // but are not initialized there -> no memory lost :)
}
//-----------------------------------------------------------------------------
int RpcRangeFinder::init()
{
  return 1;
}
//-----------------------------------------------------------------------------
void RpcRangeFinder::setEnabled( bool enable)
{
  mFgEnabled = enable;
}
//-----------------------------------------------------------------------------
void RpcRangeFinder::updateData( const double dt)
{
  std::vector<float> ranges = mClient->getRanges();

  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    mRangeData[i].range = ranges[i];
    mRangeData[i].reflectance = 0.0;
  }
}
//---------------------------------------------------------------------------
void RpcRangeFinder::print() const
{
  printf ( "RpcRangeFinder: " );
  for ( unsigned int i = 0; i < mNumSamples; i++ ) {
    printf ( " %1.3f ", mRangeData[i].range );
  }
  printf ( "\n" );
}
//-----------------------------------------------------------------------------

} // namespace
