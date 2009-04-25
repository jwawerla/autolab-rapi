/***************************************************************************
 * Project: RAPI                                                           *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: cbdefinitions.h,v 1.2 2008/12/04 01:41:17 jwawerla Exp $
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
 ***************************************************************************
 * $Log: cbdefinitions.h,v $
 * Revision 1.2  2008/12/04 01:41:17  jwawerla
 * Snapshot
 *
 * Revision 1.1.1.1  2008/04/10 21:26:29  jwawerla
 * rapi3
 *
 * Revision 1.2  2008/03/19 00:22:16  gumstix
 * Basic device driver functionality achived
 *
 * Revision 1.1.1.1  2008/02/02 22:19:52  jwawerla
 * new to cvs
 *
 * Revision 1.1.1.1  2008/01/10 19:43:55  jwawerla
 * first time in cvs
 *
 *
 ***************************************************************************/

#ifndef CBDEFINITIONS_H
#define CBDEFINITIONS_H

//---------------------------------------------------------------------------
// Create open interface definitions
//---------------------------------------------------------------------------

// from the IRobot website
// However, if the robot is on the charger but not charging because it is in
// Safe or Full mode, this doesn?t work. Instead, use this procedure:
//   1. Send opcode "7". This is not an official OI opcode, rather it is an
//      opcode used by Osmo (a firmware updating device) to initiate a soft
//      reset of the robot and force it to run its bootloader.
//   2. The robot resets. Wait 3 seconds for the bootloader to complete. Do
//      NOT send any opcodes while the bootloader is running.
//   3. The robot should start charging. Note that the robot spews some
//      battery-related text if it is charging and not in OI mode. Ignore
//      this text.
//   4. Send a Start command to get back into the OI (and stop the spew).
#define CREATE_OPCODE_RESET            7
#define CREATE_OPCODE_START            128
#define CREATE_OPCODE_SAFE             131
#define CREATE_OPCODE_FULL             132
#define CREATE_OPCODE_DEMO             136
#define CREATE_OPCODE_DRIVE            137
#define CREATE_OPCODE_LOW_SIDE_DRIVER  138
#define CREATE_OPCODE_LED              139
#define CREATE_OPCODE_DEFINE_SONG      140
#define CREATE_OPCODE_PLAY_SONG        141
#define CREATE_OPCODE_SENSORS          142
#define CREATE_OPCODE_SEND_IR          151

#define CREATE_DELAY_MODECHANGE_MS     20

#define CREATE_AXLE_LENGTH             0.258
#define CREATE_TVEL_MAX_MM_S           500
#define CREATE_RADIUS_MAX_MM           2000
#define CREATE_DIAMETER                0.33


// which sensor package do we want ? note changing this value might require 
// to change the message parse method.
//   0 -  package  7 - 26  size 26 bytes
//   1 -  package  7 - 16  size 10 bytes
//   2 -  package 17 - 20  size  6 bytes
//   3 -  package 21 - 26  size 10 bytes
//   4 -  package 27 - 34  size 14 bytes
//   5 -  package 35 - 42  size 12 bytes
//   6 -  package  7 - 42  size 52 bytes
#define CREATE_SENSOR_PACKAGE_ID       6
#define CREATE_SENSOR_PACKET_SIZE      52

// time of for read operations [ms]
#define READ_TIMEOUT                   5000

#define CB_OVERCURRENT_LSD_0           0x01
#define CB_OVERCURRENT_LSD_1           0x02
#define CB_OVERCURRENT_LSD_2           0x04
#define CB_OVERCURRENT_LEFT_WHEEL      0x08
#define CB_OVERCURRENT_RIGHT_WHEEL     0x10

#define CB_RIGHT_BUMPER                0x01
#define CB_LEFT_BUMPER                 0x02
#define CB_RIGHT_WHEEL_DROP            0x04
#define CB_LEFT_WHEEL_DROP             0x08
#define CB_CASTER_WHEEL_DROP           0x10

#define CB_RED_BUOY                    248
#define CB_GREEN_BUOY                  244
#define CB_FORCE_FIELD                 242
#define CB_RED_GREEN_BUOY              252
#define CB_RED_BUOY_FORCE_FIELD        250
#define CB_GREEN_BUOY_FORCE_FIELD      246
#define CB_RED_GREEN_BUOY_FORCE_FIELD  254
#define CB_NO_IR_DATA                  255

#define CB_INT_CHARGER                 0x01
#define CB_HOME_BASE                   0x02

#define CB_NOT_CHARGING                0x00
#define CB_RECONDITIONING_CHARGING     0x01
#define CB_FULL_CHARGING               0x02
#define CB_TRICK_CHARGING              0x03
#define CB_WAITING                     0x04
#define CB_CHARGING_FAULT              0x05

//---------------------------------------------------------------------------
// CB mainboard definitions
//---------------------------------------------------------------------------

#define ROBOSTIX_ADDR 0x0B
#define I2C_DEV_NAME "/dev/i2c-0"

// ADC reference voltage
#define VREF 5.0

// ports for 7 segment display
#define DDRA 58
#define PORTA 59

// Coefficients for IR voltage to distance conversion
#define K0 -0.033305
#define K1  0.384938
#define K2 -1.788040
#define K3  4.171021
#define K4 -5.079344
#define K5  2.983591

// RGB led stuff
#define CHIP_A_ADDR 0x67
#define CHIP_B_ADDR 0x6f

#define LED0_R_ADDR CHIP_A_ADDR
#define LED0_R_PORT 0x07
#define LED0_G_ADDR CHIP_A_ADDR
#define LED0_G_PORT 0x06
#define LED0_B_ADDR CHIP_B_ADDR
#define LED0_B_PORT 0x02

#define LED1_R_ADDR CHIP_B_ADDR
#define LED1_R_PORT 0x03
#define LED1_G_ADDR CHIP_B_ADDR
#define LED1_G_PORT 0x05
#define LED1_B_ADDR CHIP_B_ADDR
#define LED1_B_PORT 0x06

#define LED2_R_ADDR CHIP_B_ADDR
#define LED2_R_PORT 0x04
#define LED2_G_ADDR CHIP_B_ADDR
#define LED2_G_PORT 0x07
#define LED2_B_ADDR CHIP_B_ADDR
#define LED2_B_PORT 0x08

#define LED3_R_ADDR CHIP_A_ADDR
#define LED3_R_PORT 0x08
#define LED3_G_ADDR CHIP_A_ADDR
#define LED3_G_PORT 0x04
#define LED3_B_ADDR CHIP_A_ADDR
#define LED3_B_PORT 0x05

#define LED4_R_ADDR CHIP_A_ADDR
#define LED4_R_PORT 0x09
#define LED4_G_ADDR CHIP_A_ADDR
#define LED4_G_PORT 0x03
#define LED4_B_ADDR CHIP_A_ADDR
#define LED4_B_PORT 0x02

//----------------------------------------------------------------------------
#endif
