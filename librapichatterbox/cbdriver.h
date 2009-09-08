/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

#ifndef RAPICBDRIVER_H
#define RAPICBDRIVER_H

#include "cbtypes.h"
#include "velocity2d.h"
#include "rgbcolor.h"

namespace Rapi
{

/**
 * Driver class for the chatterbox hardware
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */

class CCBDriver
{
  public:
    /**
     * Default constructor
     */
    CCBDriver();
    /** Default destructor */
    ~CCBDriver();
    /**
     * Ininitializes the driver
     * @return 1 if successfull, 0 otherwise
     */
    int init();
    /**
     * Opens the serial port
     * @param port to open
     * @return 1 if successfull, 0 otherwise
     */
    int openPort(const char* port);
    /**
     * Reset create robot
     * @return 1 if successfull, 0 otherwise
     */
    int resetCreate();
    /**
     * Sets the open interface mode of create, this can be off, passive,
     * safe or full. Note if you select full, all safty features are disabled
     * so if you break it's your fault.
     * @param mode to use
     * @return 1 successfull, 0 otherwise
     */
    int setOIMode(tOIMode mode);
    /**
     * Gets the open interface mode of create as a string
     * @return mode in text form
     */
    const char* getOiModeString() const;
    /**
     * Gets the open interface mode of create
     * @return mode
     */
    tOIMode getOIMode() const;
    /**
     * Closes the conection to the create
     * @return 1 successfull, 0 otherwise
     */
    int closePort();
    /**
     * Set the velocity for the robot
     * @param vel velocity to be set
     * @return 1 successfull, 0 otherwise
     */
    int setSpeed(CVelocity2d vel);
    /**
     * Sets the low side driver with id to on
     * @param id of low side driver [0..2]
     * @param on well...
     * @return 1 if successfull, 0 otherwise
     */
    int setLowSideDriver(unsigned char id, bool on);
    /**
     * Reads sensor data from Create. This data gets stored in the data
     * structures of this class
     * @return 1 if successfull, 0 otherwise
     */
    int readSensorData();
    /**
     * Causes Create to play a predefined song
     * @param id of song to play [0..15]
     * @return 1 if successfull, 0 otherwise
     */
    int playSong(unsigned char id);
    /**
     * Gets the reading of a cliff sensor with a given id
     * @param id of cliff sensor
     * @return reading true if cliff false otherwise
     */
    bool getCliffSensor(tCliffSensor id) const;
    /**
     * Gets the raw reading of a cliff sensor with a given id.
     * If argument is 0 (CB_ALL_CLIFF), the sum of the sensors is returned.
     * @param id of cliff sensor
     * @return [0..4095]
     */
    unsigned short getRawCliffSensor(tCliffSensor id) const;
    /**
     * Checks if the Creates play button is pressed
     * @return true if pressed, false otherwise
     */
    bool isPlayButtonPressed() const;
    /**
     * Checks if the Creates advance button is pressed
     * @return true if pressed, false otherwise
     */
    bool isAdvanceButtonPressed() const;
    /**
     * Checks if any wheel was dropped
     * @return true if any wheel was dropped
     */
    bool wheelDrop() const;
    /**
     * Checks if the left wheel was dropped
     * @return true if the left wheel was dropped
     */
    bool leftWheelDrop() const;
    /**
     * Checks if the right wheel was dropped
     * @return true if the right wheel was dropped
     */
    bool rightWheelDrop() const;
    /**
     * Checks if the caster wheel was dropped
     * @return true if the caster wheel was dropped
     */
    bool casterWheelDrop() const;
    /**
     * Checks if any bumper was hit
     * @return true if bumper activated
     */
    bool bumper() const;
    /**
     * Checks if left bumper was hit
     * @return true if left bumper activated
     */
    bool leftBumper() const;
    /**
     * Checks if right bumper was hit
     * @return true if right bumper activated
     */
    bool rightBumper() const;
    /**
     * Checks if the left wheel has an over current condition. Note
     * the result is based on the last call of readSensorData()
     * @return true if over current, false other wise
     */
    bool leftWheelOverCurrent() const;
    /**
     * Checks if the right wheel has an over current condition. Note
     * the result is based on the last call of readSensorData()
     * @return true if over current, false other wise
     */
    bool rightWheelOverCurrent() const;
    /**
     * Checks if the id low side driver an over current condition. Note
     * the result is based on the last call of readSensorData()
     * @return true if over current, false other wise
     */
    bool lowSideDriverOverCurrent(unsigned char id) const;
    /**
     * Sets the Create leds
     * @param advance true = on, false = off
     * @param play true = on, false = off
     * @param powerColor color of power led [0..255]
     * @param powerIntensity intensity of power led [0..255]
     * @return 1 if successfull, 0 otherwise
     */
    int setCreateLed(bool advance, bool play, unsigned char powerColor,
                     unsigned char powerIntensity);
    /**
     * Actives one of the preprogrammed demos of Create
     * @param demo to activate
     * @return 1 if successfull, 0 otherwise
     */
    int activateDemo(tDemo demo);
    /**
     * Defines a sequence of sounds as a song. It is possible to define 16 
     * different songs, each with a length of upto 16 tones.
     * @param id of sound to be defined [0..15]
     * @param sequence to be defined [note1][length1][note2][length2]
     * @param len length of sequence (number of sounds) [1..16]
     * @return 1 if successfull, 0 otherwise
     */
    int defineSoundSequence(unsigned char id, unsigned char* sequence,
                            unsigned char len);
    /**
     * Sends a byte via the ir led
     * @param byte to send
     * @return 1 if successfull, 0 otherwise
     */
    int sendIr(unsigned char byte);
    /**
     * Set the 7 segment display to a given byte
     * @param value [0..255]
     * @return 1 if successful, 0 in case of an error
     */
    int set7SegDisplay(unsigned char value);
    /**
     * Set the rgb value for a given led. The layout is
     *  0 - front right
     *  1 - front left
     *  2 - left
     *  3 - back
     *  4 - right
     * @param id of led to set
     * @param color to be set
     * @return 1 successfull, 0 error
     */
    int setRgbLed(unsigned char id, CRgbColor color);
    /**
     * Set the intensity of the top IR led
     * @param intensity of led [0..255]
     * @return 1 successfull, 0 error
     */
    int setIrLed(unsigned char intensity);
    /**
     * Sets the 7 segment display to show a given decimal and the dot
     * @param h hexadecimal to display [0..F]
     * @param dot on, off don't care
     * @return 1 if successful, 0 in case of an error
     */
    int set7SegHexNumber(unsigned char h, tTriState dot = CB_DONT_CARE);
    /**
     * Sets just the dot
     * @param dot enable/disable
     * @return 1 if successful, 0 otherwise
     */
    int set7SegDot(bool dot);
    /**
     * Shows a rotating segment bar and progresses it one position
     * @return 1 if successful, 0 in case of an error
     */
    int rotateStep();
    /**
     * Enables the Create robot
     * @param on true to enable, false to disable
     * @return 1 if successful, 0 otherwise
     */
    int createPowerEnable(bool on);
    /**
     * Checks if power is enabled on the Create
     * @return true if enabled, false otherwise
     */
    bool isCreatePowerEnabled();
    /**
     * Enables or disables the ir sensors
     * @param enable = true enable, disable otherwise
     * @return 1 if successful, 0 in case of an error
     */
    int enableIr(bool enable);
    /**
     * Checks if the IR sensors are enabled
     * @return 1 enabled, -1 disabled, 0 error
     */
    int isIrEnabled();
    /**
     * Reads the distance from an ir sensor. The layout is:
     *  0 - front center
     *  1 - front left
     *  2 - left
     *  3 - back center
     *  4 - right
     *  5 - front right
     * @param id of ir sensor [0..5]
     * @return distance [m] or 0 in case of an error
     */
    float readDistance(unsigned char id);
    /**
     * Reads the photo sensor
     * @return [0..1024]
     */
    float readPhotoSensor();
    /**
     * Set the time constant for a channel of the adc low pass filter
     * @param tau [s]
     * @param channel [0-7]
     * @return 1 if successful, 0 in case of an error
     */
    int setLpfAdcTau( float tau, unsigned char channel );
    /**
     * Package with the latest data from Create
     */
    tCreateSensorPacket mCreateSensorPackage;

  protected:
    /**
     * Starts the robot by sending the start opcode 128
     * @return 1 if successfull, 0 otherwise
     */
    int startCreate();
    /**
     * Initializes the I2C driver and hardware. Note this function has to be
     * called first to things to work properly
     * @return 1 if successful, 0 in case of an error
     */
    int initI2c();
    /**
     * Initializes a pca9634 chip
     * @param addr i2c address of chip
     * @return 1 if successful, 0 otherwise
     */
    int initPCA9634(unsigned char addr);
    /**
     * Resets all PCA9634 chips on the i2c bus
     * @return 1 if successful, 0 otherwise
     */
    int resetPCA9634();
    /**
     * Reads the 10bits from the adc
     * @param id of ADC [0..7]
     * @return 10 bit value or 0 in case of an error
     */
    int readAdc(unsigned char id);
    /**
     * Reads the 10bits from the low pass filtered adc
     * @param id of ADC [0..7]
     * @return 10 bit value or 0 in case of an error
     */
    float readLpfAdc(unsigned char id);
    /**
     * Toggles the creates power pin
     * @param delay toggle time [ms]
     * @return 1 if successfull, 0 otherwise
     */
    int createPowerToggle( unsigned int delay );
    /**
     * Hardware reset of robostix atmega cpu
     * @return 1 if successfull, 0 otherwise
     */
    int resetRobostix();

    /** Type definition for led data address stucture */
    typedef struct  {
       /** Address of chip to use to communicate with mLed */
       unsigned char chipAddr;
       /** Pin address of led */
       unsigned char ledAddr;
    } tLed;

    /**
     * We set up a seg fault handler, that puts the robot into
     * a safe state in case of a software crash
     */
    friend void segFaultHandler(int sig);

  private:
    /** File descriptor for serial port */
    int mFd;
    /** State of low side driver */
    unsigned char mLowSideDriverStatus;
    /** i2c device */
    int mI2cDev;
    /** Address data structure  for rgb leds */
    tLed mLed[5][3];
    /** Byte value of 7 seg display */
    char m7SegByteValue;
};

} // namespace
#endif
