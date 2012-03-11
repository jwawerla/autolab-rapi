#include <string.h>
#include "RapiRpc"
#include "cbrobot.h"


int main (int argc, char* argv[])
{
    if( argc != 3 ) 
    {
        std::cerr << "Arguments is a host IP and a port number" << std::endl;
        return -1;
    }
    
    Rapi::RobotRpcClient* robot;
    std::string address= argv[1];
    int port = atoi(argv[2]);
    
    robot = new Rapi::RobotRpcClient(address, port);
    //robot->init();
    
    std::cout << "Init Done ..." << std::endl;
    
    double maxCapacity;
    std::vector<float> ranges;
    while (true)
    {
        std::cout << "Testing robot " << address << std::endl;
        robot->getPowerPackDev(maxCapacity);
        ranges = robot->getRanges();
        std::cout << "Range[0] :" << ranges.at(0) << std::endl;        
    }
    //robot->quit();
    
    std::cout << "Quit!" << std::endl;
    
    return 0;
    
}
