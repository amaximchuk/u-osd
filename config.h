/*u-osd - A simple open source osd for G-OSD
Copyright (C) 2013 Alex Maximchuk

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Credit to Carl Ljungström.
*/

#ifndef CONFIG_H_
#define CONFIG_H_

//NOTE: Most options can be disabled by commenting them out.
//This is done by inserting // at the start of the line.
//Some options depend on others so you might get an error if you mess around too much. :-)

//-----------------------------------------------------------------------------
// CPU speed
#define F_CPU 24000000UL

//-----------------------------------------------------------------------------
// MAIN FEATURES 
#define ALARM_ENABLED

//-----------------------------------------------------------------------------
// GPS
// select only one GPS sensor
#define GPS_MTK3229
//#	define GPS_NEO6M
//#	define GPS_GOSDII

//#define GPS_DIYD //Use DIYD/APM binary protocol (Thanks to David Collett)

#define GPS_MAX_CHARS 16

#if (defined(GPS_MTK3229) || defined(GPS_NEO6M))
#	define GPS_BAUD 9600
#	define GPS_CORR_DM_LAT 0 
#	define GPS_CORR_DM_LON 0 
#else
#	define GPS_BAUD 4800
//	Local calibration in decimal part of Minutes (only +)
#	define GPS_CORR_DM_LAT 2139 
#	define GPS_CORR_DM_LON 4337 
#endif

#define GPS_UBRR (F_CPU/16/GPS_BAUD-1)

//-----------------------------------------------------------------------------
// ALARMS
// Alarms (Comment to disable)
#define ALARM_BATT1_LOW 7.50		//Warn if below this level (in volt)
#define ALARM_BATT2_LOW 7.50		//Warn if below this level (in volt)
#define ALARM_RSSI_LOW 50			//Warn if below this level (in %)
#define ALARM_SPEED_HIGH 100		//Warn if above this level (in kmph/mph)
#define ALARM_ALTITUDE_LOW 10		//Warn if below this level (in meters/feet)
#define ALARM_ALTITUDE_HIGH 1000	//Warn if above this level (in meters/feet)
#define ALARM_DISTANCE_HIGH 2000	//Warn if below this level (in meters/feet)

//-----------------------------------------------------------------------------
// SENSORS
// Enabled sensors
//#define SENSOR_BATTERY_PERCENTAGE_ENABLED
#define SENSOR_RSSI_ENABLED
//#define SENSOR_COMPASS_ENABLED
#define SENSOR_CURRENT_ENABLED

// Battery percentage sensor
#ifdef SENSOR_BATTERY_PERCENTAGE_ENABLED
#	define SENSOR_BATTERY_PERCENTAGE_INPUT ANALOG_IN_1
#	define SENSOR_CELL_LOW_VOLTAGE 3.3 //(Max two digits after the dot)
#	define SENSOR_CELL_HIGH_VOLTAGE 4.2
#	define SENSOR_CELL_COUNT 2
#endif //SENSOR_BATTERY_PERCENTAGE_ENABLED

// RSSI sensor
#ifdef SENSOR_RSSI_ENABLED
#	define RSSI_INPUT ANALOG_IN_4
#	define RSSI_MAX_VOLTAGE 5.0		//(Max two digits after the dot)
#	define RSSI_MIN_VOLTAGE 1.2
#	define RSSI_REVERSED 0			// If a low voltage value means high signal strength and vice versa, then enable this. (Thanks BlueAngel2000)
#endif //SENSOR_RSSI_ENABLED

// Compass sensor
#ifdef SENSOR_COMPASS_ENABLED
#	define COMPASS_INPUT ANALOG_IN_2
#	define COMPASS_MIN_VOLTAGE 0.00 //(Max two digits after the dot)
#	define COMPASS_MAX_VOLTAGE 5.00
#endif

// Current sensor 
#ifdef SENSOR_CURRENT_ENABLED
#	define CURRENT_INPUT ANALOG_IN_3
#	define CURRENT_MIN_VOLTAGE 0.00 //(Max two digits after the dot)
#	define CURRENT_MAX_VOLTAGE 5.00
#	define CURRENT_MAX_AMPS 38 //AMP
#endif

//-----------------------------------------------------------------------------
// Unit system
//#define IMPERIAL_SYSTEM		//Feet, mph etc...
#define METRIC_SYSTEM			//Meter, hm/h etc...

//-----------------------------------------------------------------------------
// HOME POS
// Note: Use at least one or you will never get a home pos!
// Comment out unwanted to disable
//#define HOME_SET_AT_FIX		// Home position is set when GPS gets satellited fix.
//#define HOME_AUTO_SET			//Home position is set when a certain speed is exceeded.
#define HOME_SET_WITH_BUTTON	//Home position is set when the little button on OSD is long pressed.
//#define HOME_SET_FIX_COUNT 10 //Config for _SET_AT_FIX: After 10 successfully fixes, home is set.
//#define HOME_FIX_MIN_SPEED 10 //Config for _AUTO_SET: More than 10 km/h sets home.
//#define HOME_SET_MIN_SATS 4	//Config for _SET_AT_FIX: Set home only when more than 4 satellites. (Thanks to Yury Smirnov)

//-----------------------------------------------------------------------------
// User data
#define TEXT_CALL_SIGN "ALEX"	//Set this to your call sign.

//-----------------------------------------------------------------------------
// Time
//#define TIME_HOUR_ENABLED		//Enable time to show and count hours.

//-----------------------------------------------------------------------------
// Color System
#define COLORSYSTEM_NTSC

#ifdef COLORSYSTEM_NTSC
#	define TIME_FRAMES_PER_SEC 60
#else //PAL
#	define TIME_FRAMES_PER_SEC 50
#endif

//-----------------------------------------------------------------------------
// Screen and sensor refresh rate
#define SCREEN_UPDATES_PER_SEC 10
#define SENSOR_UPDATES_PER_SEC 1

// ----------- TEXT --------------
#ifdef COLORSYSTEM_NTSC
#	define TEXT_ROW_MAX_CHARS 35
#else	// PAL
#	define TEXT_ROW_MAX_CHARS 33
#endif	// COLORSYSTEM_NTSC

#define TEXT_CHAR_HEIGHT		8

#define TEXT_ROWS				17
#define TEXT_SCALE_HEIGHT		12

#define TEXT_RADAR_SEGM_MAX		5
#define TEXT_RADAR_DIST_1		10
#define TEXT_RADAR_DIST_2		50
#define TEXT_RADAR_DIST_3		100
#define TEXT_RADAR_DIST_4		200
#define TEXT_RADAR_DIST_5		500

//-----------------------------------------------------------------------------
// UPDATE LINE
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#ifdef COLORSYSTEM_NTSC
#	define FRAME_SYNC_LINE 170
#else //PAL
#	define FRAME_SYNC_LINE 110
#endif //COLORSYSTEM_NTSC

//-----------------------------------------------------------------------------
// TEXT UNITS
#ifdef METRIC_SYSTEM
#	define TEXT_LENGTH_UNIT 'm'
#else
#	define TEXT_LENGTH_UNIT 'F'
#endif //METRIC_SYSTEM

#ifdef METRIC_SYSTEM
#	define TEXT_SPEED_UNIT "KM/H"
#else 
#	define TEXT_SPEED_UNIT "MPH"
#endif //METRIC_SYSTEM

//-----------------------------------------------------------------------------
// SENSORS
// Battery
#define BATT_MIN_VOLTAGE SENSOR_CELL_LOW_VOLTAGE*SENSOR_CELL_COUNT
#define BATT_MAX_VOLTAGE SENSOR_CELL_HIGH_VOLTAGE*SENSOR_CELL_COUNT
#define BATT_MIN_VOLTAGE_INT (u16)(BATT_MIN_VOLTAGE*100)
#define BATT_MAX_VOLTAGE_INT (u16)(BATT_MAX_VOLTAGE*100)

// RSSI conversion
#define RSSI_MIN_VOLTAGE_INT (u16)(RSSI_MIN_VOLTAGE*100)
#define RSSI_MAX_VOLTAGE_INT (u16)(RSSI_MAX_VOLTAGE*100)

// Compass
#define COMPASS_MIN_VOLTAGE_INT (u16)(COMPASS_MIN_VOLTAGE*100)
#define COMPASS_MAX_VOLTAGE_INT (u16)(COMPASS_MAX_VOLTAGE*100)

// Current sensor
#define CURRENT_MIN_VOLTAGE_INT (u16)(CURRENT_MIN_VOLTAGE*100)
#define CURRENT_MAX_VOLTAGE_INT (u16)(CURRENT_MAX_VOLTAGE*100)
#define CURRENT_MAX_AMPS_INT	(u16)(CURRENT_MAX_AMPS*100)

// Alarm conversion
#define ALARM_BATT1_LOW_INT (u16)(ALARM_BATT1_LOW*100)
#define ALARM_BATT2_LOW_INT (u16)(ALARM_BATT2_LOW*100)

//-----------------------------------------------------------------------------
// HARDWARE
#define KEY (1<<PD4)
#define LED (1<<PD3)

#define BLACK_OUT PB1
#define WHITE_OUT PB3
#define LTRIG_IN PD2

#define BLACK_MASK (1<<BLACK_OUT)
#define WHITE_MASK (1<<WHITE_OUT)

#define LTRIG_MASK (1<<LTRIG_IN) //INT0
#define SS_MASK (1<<PB2)

//-----------------------------------------------------------------------------
// ADC
#define ANALOG_IN_1 0 // Voltage 1				= ADC0
#define ANALOG_IN_2 1 // Voltage 2				= ADC1
#define ANALOG_IN_3 2 // RSSI / Current Sensor	= ADC2 (0-5 Volt!)
#define ANALOG_IN_4 3 // unused					= ADC3 (0-5 Volt!)

#define ADC_OFFSET 0
#define ANALOG_IN_SCALE 31,31,5,5

#define ANALOG_IN_NUMBERS 4

//-----------------------------------------------------------------------------
// SANITY CHECK 
#if (defined(IMPERIAL_SYSTEM) && defined(METRIC_SYSTEM)) || (!defined(IMPERIAL_SYSTEM) && !defined(METRIC_SYSTEM))
#	error "Select one and only one unit system!"
#endif

#if (defined(SENSOR_VOLTAGE_2_ENABLED) && (ANALOG_IN_NUMBER <= 2) && defined(SENSOR_RSSI_ENABLED))
#	error "Can't use both RSSI and voltage 2 at the same time on this board."
#endif

#endif /* CONFIG_H_ */