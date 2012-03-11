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
    unsigned int numBumpers;
    unsigned int numWheels;
    unsigned int numCliffs;
    std::vector<float> ranges;
    std::vector<bool> bumpers;
    std::vector<bool> wheels;
    std::vector<bool> cliffs;
    while (true)
    {
        std::cout << "Testing robot " << address << std::endl;
        robot->getPowerPackDev(maxCapacity);
        ranges = robot->getRanges();
        std::cout << "Range[0] :" << ranges.at(0) << std::endl;   
        robot->getBumperDev(numBumpers);
        bumpers = robot->getBumpers();
        std::cout << "Bumpers Count: " << numBumpers << std::endl;
        std::cout << "Bumpers Status : " << bumpers[0] << " , " << bumpers[1] << std::endl;
        numWheels = robot->getWheelDropDev(numWheels);
        wheels = robot->getWheelDrops();
        std::cout << "WheelDrop Count: " << numWheels << std::endl;
        std::cout << "WheelDrop Status : " << wheels[0] << " , " << wheels[1] << std::endl;
        numWheels = robot->getWheelDropDev(numCliffs);
        cliffs = robot->getCliffs();
        std::cout << "Cliffs Count: " << numCliffs << std::endl;
        std::cout << "Cliffs Status : " << cliffs[0] << " , " << cliffs[1] << std::endl;
        usleep(500000); // Please don't overload robot 
    }
    //robot->quit();
    
    std::cout << "Quit!" << std::endl;
    
    return 0;
    
}
