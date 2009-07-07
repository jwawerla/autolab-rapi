

#ifndef CB_TYPES_H
#define CB_TYPES_H

/** Low side driver port to disable or enable laser */
#define CB_LASER_POWER                   2

/** IR constant for charging station */
#define CB_RED_BUOY                    248
#define CB_GREEN_BUOY                  244
#define CB_FORCE_FIELD                 242
#define CB_RED_GREEN_BUOY              252
#define CB_RED_BUOY_FORCE_FIELD        250
#define CB_GREEN_BUOY_FORCE_FIELD      246
#define CB_RED_GREEN_BUOY_FORCE_FIELD  254

/** Time to keep the toggle line high for power on [us] */
#define CREATE_TOGGLE_ON_TIME          400000
/** Time to keep the toggle line high for power off [us] */
#define CREATE_TOGGLE_OFF_TIME         25000
/** Number of retries to toggle power, before giving up */
#define NUM_POWER_TOGGLE_RETRY         10

/**
 * Type definition of the creates sensor data package
 */
typedef struct {
  unsigned char  bumpsWheel;              // [0..31]
  unsigned char  wall;                    // [0,1]
  unsigned char  cliffLeft;               // [0,1]
  unsigned char  cliffFrontLeft;          // [0,1]
  unsigned char  cliffFrontRight;         // [0,1]
  unsigned char  cliffRight;              // [0,1]
  unsigned char  virtualWall;             // [0,1]
  unsigned char  overCurrents;            // [0,1]
  unsigned char  aux0;                    // reserved
  unsigned char  aux1;                    // reserved
  unsigned char  ir;                      // [0..255]
  unsigned char  button;                  // [0..15]
  short          distance;                // [mm]
  short          angle;                   // [deg]
  unsigned char  chargingState;           // [0..5]
  short          voltage;                 // [mV]
  short          current;                 // [mA]
  char           batTemp;                 // [C]
  short          batCapacity;             // [mAh]
  short          batMaxCapacity;          // [mAh]
  unsigned short wallSignal;              // [0..4095]
  unsigned short cliffSignalLeft;         // [0..4095]
  unsigned short cliffSignalFrontLeft;    // [0..4095]
  unsigned short cliffSignalFrontRight;   // [0..4095]
  unsigned short cliffSignalRight;        // [0..4095]
  unsigned char  digitalInput;            // [0..31]
  unsigned short analogInput;             // [0..1023]
  unsigned char  chargingSource;          // [0..3]
  unsigned char  oiMode;                  // [0..3]
  unsigned char  songNumber;              // [0..15]
  unsigned char  songPlaying;             // [0..1]
  unsigned char  streamPackage;           // [0..42]
  short          velocity;                // [mm/s]
  short          radius;                  // [mm]
  short          rightWheelVelocity;      // [mm/s]
  short          leftWheelVelocity;       // [mm/s]
} __attribute__ ((packed)) tCreateSensorPacket;


/**
 * Enumeration of demo programs
 */
typedef enum {
  CB_DEMO_STOP = -1,
  CB_DEMO_COVER,
  CB_DEMO_DOCK,
  CB_DEMO_SPOT,
  CB_DEMO_MOUSE,
  CB_DEMO_FIGURE_EIGHT,
  CB_DEMO_WIMP,
  CB_DEMO_HOME,
  CB_DEMO_TAG,
  CB_DEMO_PACHEBLE,
  CB_DEMO_BANJO
} tDemo;

/**
 * Enumeration of cliff sensors
 */
typedef enum {
  CB_ALL_CLIFF = 0,
  CB_LEFT_CLIFF,
  CB_FRONT_LEFT_CLIFF,
  CB_FRONT_RIGHT_CLIFF,
  CB_RIGHT_CLIFF
} tCliffSensor;

/**
 * Enumeration of control mode
 */
typedef enum {
  CB_MODE_OFF = 0,
  CB_MODE_PASSIVE,
  CB_MODE_SAFE,
  CB_MODE_FULL
} tOIMode;

/**
 * Enumeration for tri-state
 */
typedef enum {
  CB_OFF = 0,
  CB_ON,
  CB_DONT_CARE
} tTriState;

//----------------------------------------------------------------------------
#endif
