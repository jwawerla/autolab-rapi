#include <unistd.h>
#include <string>
#include <pthread.h>
#include "RapiGui"
//#include "rpcrobot.h"
#include "RapiRpc"

/** a robot thread container */
struct robot_t {
  Rapi::RpcRobot * robot;
  pthread_t thread;
};

/** pthread running function */
void * run( void * arg )
{
  Rapi::RpcRobot * robot = (Rapi::RpcRobot *) arg;
  robot->run();
  return 0;
}

/** This function cleans up threads and any memory */
void cleanup( std::vector<robot_t> &robotList )
{
  for( unsigned int i = 0; i < robotList.size(); ++i ) {
    robotList[i].robot->quit();
    if( pthread_join( robotList[i].thread, NULL ) )
      std::cerr << "Failed to rejoin robot thread #" << i << std::endl;
    delete robotList[i].robot;
    std::cout << "Robot #" << i << " finished!" << std::endl;
  }
  // gui thread cleaned up by CGui::destructor
}

int main( int argc, char* argv[])
{
  if( argc < 3 ) {
    std::cerr << "Arguments are a list of host IPs and port numbers"
              << std::endl;
    return -1;
  }
  Rapi::CGui * gui = Rapi::CGui::getInstance( 0, NULL );
  std::vector<robot_t> robots;
  for( int i = 1; i < argc; i = i + 2 ) {
    std::string address = argv[i];
    int port = atoi( argv[i+1] );
    robot_t tmp;
    tmp.robot = new Rapi::RpcRobot( address, port );
    tmp.robot->init();
    gui->registerRobot( tmp.robot );
    robots.push_back( tmp );
    if( pthread_create( &robots.back().thread, NULL, run, tmp.robot ) ) {
      std::cerr << "Failed to create robot thread" << std::endl;
      cleanup( robots );
      exit( EXIT_FAILURE );
    }
    std::cout << "Connected " << address << " on port " << port << std::endl;
  }

  // block until gui quits...TODO: fix this...need to wait on something else
  if( pthread_join( gui->mPThread, NULL ) ) {
    std::cerr << "Failed to rejoin gui thread.  Quitting..." << std::endl;
  }
  cleanup( robots );

  return 0;
}
